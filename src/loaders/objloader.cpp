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
    CumTimer calculate_bvh_bounds("calculate_bvh_bounds");

    for(auto& object : objects)
    {
        // Otherwise, load it and cache it
        auto timer = read_file.IncreaseCum();

        // Load the mesh file and then cache it
        importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0F);
        const aiScene* s = importer.ReadFile(object.p_file, aiProcess_GenSmoothNormals | aiProcess_FixInfacingNormals);

        assert(s != nullptr);
        assert(s->mFlags ^ AI_SCENE_FLAGS_INCOMPLETE);
        assert(s->mRootNode != nullptr);

        for (size_t m = 0; m < s->mNumMeshes; m++)
        {
            // assimp mesh data
            aiMesh* aimesh = s->mMeshes[m];
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

            // Load vertex buffer
            auto* vertex_buffer = static_cast<Vector3f*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), aimesh->mNumVertices));
            for (size_t i = 0; i < aimesh->mNumVertices; i++)
                vertex_buffer[i] = Vector3f(aimesh->mVertices[i].x, aimesh->mVertices[i].y, aimesh->mVertices[i].z);

            // Load index buffer
            auto* index_buffer = static_cast<std::array<unsigned int, 3>*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned int), aimesh->mNumFaces));
            for (size_t i = 0; i < aimesh->mNumFaces; i++)
                for (size_t j = 0; j < aimesh->mFaces[i].mNumIndices; j++)
                    index_buffer[i][j] = aimesh->mFaces[i].mIndices[j];


            // Load vertex normals into buffer
            rtcSetGeometryVertexAttributeCount(mesh, 1);
            auto* vertex_normal_buffer = static_cast<Vector3f*>(rtcSetNewGeometryBuffer(mesh, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), aimesh->mNumVertices));

            for (size_t i = 0; i < aimesh->mNumVertices; i++)
                vertex_normal_buffer[i] = Vector3f(aimesh->mNormals[i].x, aimesh->mNormals[i].y, aimesh->mNormals[i].z);

            rtcSetGeometryBuildQuality(mesh, RTC_BUILD_QUALITY_LOW);
            rtcCommitGeometry(mesh);
            const unsigned int geomID = rtcAttachGeometry(embree.scene, mesh);

            for (size_t i = 0; i < aimesh->mNumFaces; i++)
            {
                auto timer = calculate_bvh_bounds.IncreaseCum();

                RTCBuildPrimitive prim
                {
                    .lower_x = std::numeric_limits<float>::max(),
                    .lower_y = std::numeric_limits<float>::max(),
                    .lower_z = std::numeric_limits<float>::max(),
                    .geomID  = geomID,
                    .upper_x = std::numeric_limits<float>::min(),
                    .upper_y = std::numeric_limits<float>::min(),
                    .upper_z = std::numeric_limits<float>::min(),
                    .primID  = static_cast<unsigned int>(i)
                };

                for (size_t j = 0; j < aimesh->mFaces[i].mNumIndices; j++)
                {
                    prim.lower_x = std::min(prim.lower_x, aimesh->mVertices[aimesh->mFaces[i].mIndices[j]].x);
                    prim.lower_y = std::min(prim.lower_y, aimesh->mVertices[aimesh->mFaces[i].mIndices[j]].y);
                    prim.lower_z = std::min(prim.lower_z, aimesh->mVertices[aimesh->mFaces[i].mIndices[j]].z);
                    prim.upper_x = std::max(prim.upper_x, aimesh->mVertices[aimesh->mFaces[i].mIndices[j]].x);
                    prim.upper_y = std::max(prim.upper_y, aimesh->mVertices[aimesh->mFaces[i].mIndices[j]].y);
                    prim.upper_z = std::max(prim.upper_z, aimesh->mVertices[aimesh->mFaces[i].mIndices[j]].z);
                }

                this->prims.push_back(prim);
            }

            rtcSetGeometryUserData(mesh, &object);
            rtcReleaseGeometry(mesh);
    }
    }

    rtcCommitScene(embree.scene);
    importer.FreeScene();
}

std::vector<RTCBuildPrimitive>& ObjectLoader::GetPrims()
{
    assert(this->prims.size() > 0);
    return this->prims;
}
}