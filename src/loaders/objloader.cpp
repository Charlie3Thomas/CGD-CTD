#include "objloader.hpp"

#include <cassert>
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "embree/embreedevice.hpp"
#include "config/options.hpp"
#include "bvh/bvh.hpp"
#include "transform.hpp"
#include "utils/timer.hpp"

namespace CT
{
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
    (*mesh) *= Eigen::Matrix3f::Identity() * 1.0F; // Scale
    (*mesh) *= MakeRotation(180.0F, 0.0F, 180.0F);  // Rotate
    (*mesh) += Eigen::Vector3f(0.0F, 0.0F, 10.0F);   // Translate

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
}