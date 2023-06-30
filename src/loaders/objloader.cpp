#include "objloader.hpp"

#include <cassert>
#include <iostream>
#include <memory>

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
void ObjectLoader::LoadObjects(std::vector<Object>& objects)
{
    Timer t = Timer("Load objects");
    // Retrieve embree singleton instance
    EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // OBJ Importer
    Assimp::Importer importer;

    CumTimer read_file("read_file");
    CumTimer transform_mesh("transform_mesh");
    CumTimer alloc_geom("alloc_geom");
    CumTimer fill_geom("fill_geom");
    CumTimer calculate_bvh_bounds("calculate_bvh_bounds");
    CumTimer face_indices("face_indices");
    CumTimer commit_geom("commit_geom");
    CumTimer add_scene("add_scene");
    CumTimer add_material("add_material");

    for(auto& object : objects)
    {
        // Otherwise, load it and cache it
        auto timer = read_file.IncreaseCum();

        // Load the mesh file and then cache it
#if 1
        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0F);
        const aiScene* s = importer.ReadFile(object.p_file, aiProcess_GenSmoothNormals | aiProcess_FixInfacingNormals);
#else
        const aiScene* s = importer.ReadFile(object.p_file, 0);
#endif

        assert(s != nullptr);
        assert(s->mFlags ^ AI_SCENE_FLAGS_INCOMPLETE);
        assert(s->mRootNode != nullptr);

        std::cout << "Number of sub-meshes: " << s->mNumMeshes << std::endl;
for (size_t i = 0; i < s->mNumMeshes; i++)
{
        // assimp mesh data
        aiMesh* aimesh = s->mMeshes[i];
        assert(s->mNumMeshes > 0);       

        assert(aimesh != nullptr);

        {
            auto timer = transform_mesh.IncreaseCum();        
            (*aimesh) *= object.transformation;
            (*aimesh) += object.translation;
        }
        
        // Embree mesh data
        RTCGeometry mesh = rtcNewGeometry(embree.device, RTC_GEOMETRY_TYPE_TRIANGLE);
        assert(mesh != nullptr);        

        // BVH bounds
        RTCBounds bounds = RTCBounds();

        Vector3f* vertices = nullptr;
        {
            auto timer = alloc_geom.IncreaseCum();
            // Vertex positions
            vertices = static_cast<Vector3f*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), aimesh->mNumVertices));
        }

        // Load vertex normals into buffer
        Vector3f* vn = nullptr;
        rtcSetGeometryVertexAttributeCount(mesh, 1);
        vn = static_cast<Vector3f*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), aimesh->mNumVertices));
        
        if (aimesh->HasNormals())
        {
            for (size_t i = 0; i < aimesh->mNumVertices; i++)
            {
                float x = aimesh->mNormals[i].x;
                float y = aimesh->mNormals[i].y;
                float z = aimesh->mNormals[i].z;

                vn[i] = Vector3f(x, y, z).normalized();
            }
        }
        else
        {
            memset(static_cast<void*>(vn), 0, sizeof(Vector3f) * aimesh->mNumVertices);

            for (size_t i = 0; i < aimesh->mNumFaces; i++)
            {
                const aiFace& face = aimesh->mFaces[i];
                const Vector3f v0 = Vector3f(aimesh->mVertices[face.mIndices[0]].x, aimesh->mVertices[face.mIndices[0]].y, aimesh->mVertices[face.mIndices[0]].z);
                const Vector3f v1 = Vector3f(aimesh->mVertices[face.mIndices[1]].x, aimesh->mVertices[face.mIndices[1]].y, aimesh->mVertices[face.mIndices[1]].z);
                const Vector3f v2 = Vector3f(aimesh->mVertices[face.mIndices[2]].x, aimesh->mVertices[face.mIndices[2]].y, aimesh->mVertices[face.mIndices[2]].z);
                const Vector3f normal = (v1 - v0).cross(v2 - v0);

                for (size_t j = 0; j < face.mNumIndices; j++)
                {
                    size_t index = face.mIndices[j];
                    vn[index] += normal;
                }
            }

            for (size_t i = 0; i < aimesh->mNumVertices; i++)
            {
                vn[i].normalize();
            }
        }        
        
        {
        auto timer = fill_geom.IncreaseCum();
        for (size_t tris = 0; tris < aimesh->mNumVertices / 3; tris++) // TODO: tris != verts/3
        {
            vertices[tris * 3 + 0] = Vector3f(aimesh->mVertices[tris * 3 + 0].x, aimesh->mVertices[tris * 3 + 0].y, aimesh->mVertices[tris * 3 + 0].z);
            vertices[tris * 3 + 1] = Vector3f(aimesh->mVertices[tris * 3 + 1].x, aimesh->mVertices[tris * 3 + 1].y, aimesh->mVertices[tris * 3 + 1].z);
            vertices[tris * 3 + 2] = Vector3f(aimesh->mVertices[tris * 3 + 2].x, aimesh->mVertices[tris * 3 + 2].y, aimesh->mVertices[tris * 3 + 2].z);

            if (config.use_bvh)
            {
                auto timer = calculate_bvh_bounds.IncreaseCum();
                // Determine BVH bounds
                bounds.lower_x = std::min(bounds.lower_x, vertices[tris].x());
                bounds.lower_y = std::min(bounds.lower_y, vertices[tris].y());
                bounds.lower_z = std::min(bounds.lower_z, vertices[tris].z());
                bounds.upper_x = std::max(bounds.upper_x, vertices[tris].x());
                bounds.upper_y = std::max(bounds.upper_y, vertices[tris].y());
                bounds.upper_z = std::max(bounds.upper_z, vertices[tris].z());

                // Make BVH Build primitive
                RTCBuildPrimitive prim;
                    prim.lower_x = bounds.lower_x;
                    prim.lower_y = bounds.lower_y;
                    prim.lower_z = bounds.lower_z;
                    prim.geomID = 0;
                    prim.upper_x = bounds.upper_x;
                    prim.upper_y = bounds.upper_y;
                    prim.upper_z = bounds.upper_z;
                    prim.primID = tris;

                this->prims.push_back(prim);
            }

            // If loaded .obj has texture coordinates, add them to the bjobect
            if (aimesh->HasTextureCoords(0))
            {
                UVTextureCoords tex_coords;

                for (size_t i = 0; i < 3; i++)
                {
                    float x = aimesh->mTextureCoords[0][tris * 3 + i].x;
                    assert(x >= 0.0F && x <= 1.0F);
                    float y = aimesh->mTextureCoords[0][tris * 3 + i].y;
                    assert(y >= 0.0F && y <= 1.0F);

                    tex_coords.coords[i] = Vector2f(x, y);
                }

                assert(object.tex_coords.find(tris) == object.tex_coords.end());
                object.tex_coords.emplace(tris, tex_coords);
            }  
        }
        }

        {
            auto timer = face_indices.IncreaseCum();
            // Face indices
            auto* faces = static_cast<std::array<uint32_t, 3>*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(uint32_t), aimesh->mNumVertices / 3));
            for (size_t i = 0; i < aimesh->mNumFaces; i++)
            {
                const aiFace& face = aimesh->mFaces[i];

                for (size_t j = 0; j < face.mNumIndices; j++)
                {
                    size_t index = face.mIndices[j];
                    faces[i][j] = index;
                }
            }
        }
        
        unsigned int geomID = i;
        {
            auto timer = commit_geom.IncreaseCum();
            rtcSetGeometryVertexAttributeCount(mesh, 1);
            //rtcSetSharedGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3, nullptr, 0, sizeof(RGB), 8);

            rtcSetGeometryBuildQuality(mesh, RTC_BUILD_QUALITY_LOW);
            rtcCommitGeometry(mesh);
            geomID = rtcAttachGeometry(embree.scene, mesh);
            rtcSetGeometryUserData(mesh, &object);
            rtcReleaseGeometry(mesh);
        }        
    }
}

    {
    auto timer = add_scene.IncreaseCum();
    rtcCommitScene(embree.scene);
    }

    importer.FreeScene();
}

std::vector<RTCBuildPrimitive>& ObjectLoader::GetPrims()
{
    assert(this->prims.size() > 0);
    return this->prims;
}
}