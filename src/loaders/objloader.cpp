#include "objloader.hpp"

#include <cassert>
#include <iostream>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Eigen/Core>

#include "embree/embreesingleton.hpp"
#include "config/options.hpp"
#include "bvh/bvh.hpp"
#include "transform.hpp"
#include "utils/timer.hpp"

using namespace Eigen;

namespace CT
{

struct Vertex
{
    Vector3f pos;
};

struct Triangle
{
    std::array<uint32_t, 3> v;
};

//std::unordered_map<const char*, aiMesh> meshes;

unsigned int LoadObj(const char* p_file, std::vector<RTCBuildPrimitive>& prims, const Eigen::Matrix3f& transformation, const Eigen::Vector3f& translation)
{
    // Retrieve embree singleton instance
    EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

    // OBJ Importer
    Assimp::Importer importer;

    // Load the mesh file
    const aiScene* s = importer.ReadFile(p_file, 0);
    assert(s != nullptr);
    assert(s->mFlags ^ AI_SCENE_FLAGS_INCOMPLETE);
    assert(s->mRootNode != nullptr);

    // assimp mesh data
    aiMesh* aimesh = s->mMeshes[0];
    assert(aimesh != nullptr);

    // // Cache mesh data
    // assert(meshes.find(p_file) == meshes.end());
    // meshes[p_file] = *aimesh;

    // Transform mesh
    (*aimesh) *= transformation;
    (*aimesh) += translation;

    // Embree mesh data
    RTCGeometry mesh = rtcNewGeometry(embree.device, RTC_GEOMETRY_TYPE_TRIANGLE);
    assert(mesh != nullptr);

    // BVH bounds
    RTCBounds bounds;

    // Vertex positions
    auto* vertices = static_cast<Vertex*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), aimesh->mNumVertices));

    for (size_t tris = 0; tris < aimesh->mNumVertices / 3; tris++)
    {
        vertices[tris * 3 + 0].pos = Vector3f(aimesh->mVertices[tris * 3 + 0].x, aimesh->mVertices[tris * 3 + 0].y, aimesh->mVertices[tris * 3 + 0].z);
        vertices[tris * 3 + 1].pos = Vector3f(aimesh->mVertices[tris * 3 + 1].x, aimesh->mVertices[tris * 3 + 1].y, aimesh->mVertices[tris * 3 + 1].z);
        vertices[tris * 3 + 2].pos = Vector3f(aimesh->mVertices[tris * 3 + 2].x, aimesh->mVertices[tris * 3 + 2].y, aimesh->mVertices[tris * 3 + 2].z);

        // BVH Bounds
        // Determine bounds
        bounds.lower_x = std::min(bounds.lower_x, vertices[tris].pos.x());
        bounds.lower_y = std::min(bounds.lower_y, vertices[tris].pos.y());
        bounds.lower_z = std::min(bounds.lower_z, vertices[tris].pos.z());
        bounds.upper_x = std::max(bounds.upper_x, vertices[tris].pos.x());
        bounds.upper_y = std::max(bounds.upper_y, vertices[tris].pos.y());
        bounds.upper_z = std::max(bounds.upper_z, vertices[tris].pos.z());

        // Build primitive
        RTCBuildPrimitive prim;
            prim.lower_x = bounds.lower_x;
            prim.lower_y = bounds.lower_y;
            prim.lower_z = bounds.lower_z;
            prim.geomID = 0;
            prim.upper_x = bounds.upper_x;
            prim.upper_y = bounds.upper_y;
            prim.upper_z = bounds.upper_z;
            prim.primID = tris;

        prims.push_back(prim); 
    }

    // Face indices
    auto* triangles = static_cast<Triangle*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(uint32_t), aimesh->mNumVertices / 3));
    for (size_t tris = 0; tris < aimesh->mNumVertices / 3; tris++)
    {
        triangles[tris].v[0] = tris * 3 + 0;
        triangles[tris].v[1] = tris * 3 + 1;
        triangles[tris].v[2] = tris * 3 + 2;
    }

    rtcSetGeometryVertexAttributeCount(mesh, 1);
    rtcSetSharedGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3, nullptr, 0, sizeof(RGB), 8);

    //rtcSetGeometryBuildQuality(mesh, RTC_BUILD_QUALITY_HIGH);
    rtcCommitGeometry(mesh);
    unsigned int geomID = rtcAttachGeometry(embree.scene, mesh);
    rtcReleaseGeometry(mesh);

    rtcCommitScene(embree.scene);

    return geomID;


    // // Vertex positions
    // auto* vertices = static_cast<Vertex*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), 8));
    //     vertices[0].pos = (Vector3f(-1,-1,-1)); vertices[0].pos = (transformation * vertices[0].pos) + translation;
    //     vertices[1].pos = (Vector3f(-1,-1, 1)); vertices[1].pos = (transformation * vertices[1].pos) + translation;
    //     vertices[2].pos = (Vector3f(-1, 1,-1)); vertices[2].pos = (transformation * vertices[2].pos) + translation;
    //     vertices[3].pos = (Vector3f(-1, 1, 1)); vertices[3].pos = (transformation * vertices[3].pos) + translation;
    //     vertices[4].pos = (Vector3f( 1,-1,-1)); vertices[4].pos = (transformation * vertices[4].pos) + translation;
    //     vertices[5].pos = (Vector3f( 1,-1, 1)); vertices[5].pos = (transformation * vertices[5].pos) + translation;
    //     vertices[6].pos = (Vector3f( 1, 1,-1)); vertices[6].pos = (transformation * vertices[6].pos) + translation;
    //     vertices[7].pos = (Vector3f( 1, 1, 1)); vertices[7].pos = (transformation * vertices[7].pos) + translation;

    // Face indices
    //int tri = 0;
    //Triangle* triangles = (Triangle*)rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(uint32_t), 12);
    // auto* triangles = static_cast<Triangle*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(uint32_t), 12));
    //     triangles[tri].v = { 0, 1, 2 }; tri++;
    //     triangles[tri].v = { 1, 3, 2 }; tri++;

    //     triangles[tri].v = { 4, 6, 5 }; tri++;
    //     triangles[tri].v = { 5, 6, 7 }; tri++;

    //     triangles[tri].v = { 0, 4, 1 }; tri++;
    //     triangles[tri].v = { 1, 4, 5 }; tri++;

    //     triangles[tri].v = { 2, 3, 6 }; tri++;
    //     triangles[tri].v = { 3, 7, 6 }; tri++;

    //     triangles[tri].v = { 0, 2, 4 }; tri++;
    //     triangles[tri].v = { 2, 6, 4 }; tri++;

    //     triangles[tri].v = { 1, 5, 3 }; tri++;
    //     triangles[tri].v = { 3, 5, 7 }; tri++;
}

/*
// unsigned int LoadObj(const char* p_file, const Eigen::Matrix3f& transformation, const Eigen::Vector3f& translation)
// {
//     // Retrieve embree singleton instance
//     EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

//     RTCGeometry mesh = rtcNewGeometry(embree.device, RTC_GEOMETRY_TYPE_TRIANGLE);
//     assert(mesh != nullptr);

//     // Vertex positions
//     auto* vertices = static_cast<Vertex*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), 8));
//         vertices[0].pos = (Vector3f(-1,-1,-1)); vertices[0].pos = (transformation * vertices[0].pos) + translation;
//         vertices[1].pos = (Vector3f(-1,-1, 1)); vertices[1].pos = (transformation * vertices[1].pos) + translation;
//         vertices[2].pos = (Vector3f(-1, 1,-1)); vertices[2].pos = (transformation * vertices[2].pos) + translation;
//         vertices[3].pos = (Vector3f(-1, 1, 1)); vertices[3].pos = (transformation * vertices[3].pos) + translation;
//         vertices[4].pos = (Vector3f( 1,-1,-1)); vertices[4].pos = (transformation * vertices[4].pos) + translation;
//         vertices[5].pos = (Vector3f( 1,-1, 1)); vertices[5].pos = (transformation * vertices[5].pos) + translation;
//         vertices[6].pos = (Vector3f( 1, 1,-1)); vertices[6].pos = (transformation * vertices[6].pos) + translation;
//         vertices[7].pos = (Vector3f( 1, 1, 1)); vertices[7].pos = (transformation * vertices[7].pos) + translation;

//     // Face indices
//     int tri = 0;
//     //Triangle* triangles = (Triangle*)rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(uint32_t), 12);
//     auto* triangles = static_cast<Triangle*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(uint32_t), 12));
//         triangles[tri].v = { 0, 1, 2 }; tri++;
//         triangles[tri].v = { 1, 3, 2 }; tri++;

//         triangles[tri].v = { 4, 6, 5 }; tri++;
//         triangles[tri].v = { 5, 6, 7 }; tri++;

//         triangles[tri].v = { 0, 4, 1 }; tri++;
//         triangles[tri].v = { 1, 4, 5 }; tri++;

//         triangles[tri].v = { 2, 3, 6 }; tri++;
//         triangles[tri].v = { 3, 7, 6 }; tri++;

//         triangles[tri].v = { 0, 2, 4 }; tri++;
//         triangles[tri].v = { 2, 6, 4 }; tri++;

//         triangles[tri].v = { 1, 5, 3 }; tri++;
//         triangles[tri].v = { 3, 5, 7 }; tri++;


//     rtcSetGeometryVertexAttributeCount(mesh, 1);
//     rtcSetSharedGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3, nullptr, 0, sizeof(RGB), 8);

//     //rtcSetGeometryBuildQuality(mesh, RTC_BUILD_QUALITY_HIGH);
//     rtcCommitGeometry(mesh);
//     unsigned int geomID = rtcAttachGeometry(embree.scene, mesh);
//     rtcReleaseGeometry(mesh);

//     rtcCommitScene(embree.scene);

//     return geomID;
// }
*/


/*
bool LoadObj()
{
    Timer t = Timer("LoadObj");

    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // OBJ Importer
    Assimp::Importer importer;

    // Load the mesh file
    const aiScene* s = importer.ReadFile(config.input_model_filename.c_str(), 0);
    assert(s != nullptr);
    assert(s->mFlags ^ AI_SCENE_FLAGS_INCOMPLETE);
    assert(s->mRootNode != nullptr);

    aiMesh* mesh = s->mMeshes[0];
    assert(mesh != nullptr);

    // TODO: Not here
    (*mesh) *= Eigen::Matrix3f::Identity() * 1.0F;          // Scale
    (*mesh) *= MakeRotation(0.0F, 0.0F, 0.0F);              // Rotate
    (*mesh) += Eigen::Vector3f(0.0F, 0.0F, 10.0F);          // Translate

    // // woody
    // (*mesh) *= Eigen::Matrix3f::Identity() * 0.1F;   // Scale
    // (*mesh) *= MakeRotation(0.0F, 180.0F, 0.0F);              // Rotate
    // (*mesh) += Eigen::Vector3f(15.0F, -15.0F, 50.0F);          // Translate

    std::vector<RTCBuildPrimitive> prims;

    for (size_t tris = 0; tris < mesh->mNumVertices / 3; tris++)
    {
        RTCGeometry geom = rtcNewGeometry(EmbreeSingleton::GetInstance().device, RTC_GEOMETRY_TYPE_TRIANGLE);
        RTCBounds bounds;

        auto* vb = static_cast<float*> (rtcSetNewGeometryBuffer(geom,
            RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3*sizeof(float), 3));

        for (size_t verts = 0; verts < 3; verts++)
        {
            vb[verts * 3 + 0] = mesh->mVertices[3 * tris + verts].x;
            vb[verts * 3 + 1] = mesh->mVertices[3 * tris + verts].y;
            vb[verts * 3 + 2] = mesh->mVertices[3 * tris + verts].z;

            // Determine bounds
            bounds.lower_x = std::min(bounds.lower_x, vb[verts * 3 + 0]);
            bounds.lower_y = std::min(bounds.lower_y, vb[verts * 3 + 1]);
            bounds.lower_z = std::min(bounds.lower_z, vb[verts * 3 + 2]);
            bounds.upper_x = std::max(bounds.upper_x, vb[verts * 3 + 0]);
            bounds.upper_y = std::max(bounds.upper_y, vb[verts * 3 + 1]);
            bounds.upper_z = std::max(bounds.upper_z, vb[verts * 3 + 2]);
        }

        auto* ib = static_cast<unsigned*> (rtcSetNewGeometryBuffer(geom,
            RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3*sizeof(unsigned), 1));

        ib[0] = 0; ib[1] = 1; ib[2] = 2;

        // Build primitive
        RTCBuildPrimitive prim;
            prim.lower_x = bounds.lower_x;
            prim.lower_y = bounds.lower_y;
            prim.lower_z = bounds.lower_z;
            prim.geomID = 0;
            prim.upper_x = bounds.upper_x;
            prim.upper_y = bounds.upper_y;
            prim.upper_z = bounds.upper_z;
            prim.primID = tris;

        prims.push_back(prim);        

        rtcSetGeometryBuildQuality(geom, RTC_BUILD_QUALITY_HIGH);
        rtcCommitGeometry(geom);
        rtcAttachGeometry(EmbreeSingleton::GetInstance().scene, geom);
        rtcReleaseGeometry(geom);
    }

    if (config.use_bvh){ BuildBVH(RTCBuildQuality::RTC_BUILD_QUALITY_HIGH, prims, nullptr, 1024); }
    

    rtcCommitScene(EmbreeSingleton::GetInstance().scene);

    return true;
}
*/


}