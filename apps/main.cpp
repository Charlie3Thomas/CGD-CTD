#include <iostream>
#include <limits>
#include <cassert>

#include <embree3/rtcore.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "utils/ppm.hpp"
#include "utils/utils.hpp"

using namespace CT;

void EmbreeTriangle();
bool LoadObj();
bool Test();

int main()
{
    Test();
    return EXIT_SUCCESS;
}

void EmbreeTriangle()
{
    // Image size
    size_t width = 100;
    size_t height = 100;

    //PPM header
    PPMWriteHeader(std::cout, width, height);

    int half_width = static_cast<int>(width) / 2;
    int half_height = static_cast<int>(height) / 2;    

    RTCDevice device = rtcNewDevice(nullptr);
    assert(device != nullptr);
    RTCScene scene   = rtcNewScene(device);
    assert(scene != nullptr);

    for (int i = 1; i < 3; i++)
    {
        RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

        auto* vb = static_cast<float*> (rtcSetNewGeometryBuffer(geom,
            RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3*sizeof(float), 3));

        vb[0] = 0.F;     vb[1] = 0.F;     vb[2] = 0.F; // 1st vertex
        vb[3] = 1.F * i; vb[4] = 0.F;     vb[5] = 0.F; // 2nd vertex
        vb[6] = 0.F;     vb[7] = 1.F * i; vb[8] = 0.F; // 3rd vertex

        auto* ib = static_cast<unsigned*> (rtcSetNewGeometryBuffer(geom,
            RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3*sizeof(unsigned), 1));

        ib[0] = 0; ib[1] = 1; ib[2] = 2;

        rtcCommitGeometry(geom);
        rtcAttachGeometry(scene, geom);
        rtcReleaseGeometry(geom);
    }

    rtcCommitScene(scene);

    // PPM body
    for (int vertical = -half_height; vertical < half_height; vertical++)
    {
        for (int horizontal = -half_width; horizontal < half_width; horizontal++)
        {
            // Create ray
            RTCRayHit ray;
            ray.ray.org_x =  0.0F;
            ray.ray.org_y =  0.0F;
            ray.ray.org_z =  -1.0F;

            //TODO: Adjust angle of ray based on pixel position
            float pixel_width  = 2.0F / static_cast<float>(width);
            float pixel_height = 2.0F / static_cast<float>(height);
            ray.ray.dir_x = static_cast<float>(horizontal)  * pixel_width;
            ray.ray.dir_y = static_cast<float>(vertical)    * pixel_height;
            ray.ray.dir_z = 1.0F;

            ray.ray.tnear = 0.001F; // Set the minimum distance to start tracing
            ray.ray.tfar  = std::numeric_limits<float>::infinity();
            ray.ray.mask  = -1;

            // default geomID to invalid
            ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;

            // Create context
            RTCIntersectContext context;
            rtcInitIntersectContext(&context);

            // Trace the ray against the scene
            rtcIntersect1(scene, &context, &ray);

            RGB final_colour = (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID ? CT::WHITE : CT::BLACK);

            PPMWritePixel(std::cout, final_colour);
        }
    }

    rtcReleaseScene(scene);
    rtcReleaseDevice(device);
}

bool LoadObj()
{
    Assimp::Importer importer;

    // Define the post-processing steps to be used during the import
    //unsigned int post_process_flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;

    // Load the mesh file
    const aiScene* scene = importer.ReadFile("/home/Charlie/CGD-CTD/obj/teapot.obj", 0);

    // Check if the mesh was loaded successfully
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "Failed to load mesh: " << importer.GetErrorString() << std::endl;

        return false;
    }

    // Print the number of meshes in the scene
    std::cout << "Number of meshes: " << scene->mNumMeshes << std::endl;

    // Print number of verticies in mesh
    std::cout << "Number of vertices in mesh: " << scene->mMeshes[0]->mNumVertices << std::endl;

    aiMesh* mesh = scene->mMeshes[0];

    // Print the verticies of the mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        std::cout << "Vertex " << i << ": " 
            << mesh->mVertices[i].x << ", " 
            << mesh->mVertices[i].y << ", " 
            << mesh->mVertices[i].z << std::endl;
    }

    // // Print the faces of the mesh
    // for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    // {
    //     aiFace face = mesh->mFaces[i];
    //     std::cout << "Face " << i << ": ";

    //     for (unsigned int j = 0; j < face.mNumIndices; j++)
    //     {
    //         std::cout << face.mIndices[j] << " ";
    //     }
        
    //     std::cout << std::endl;
    // }


    return true;
}

bool Test()
{
    // OBJ Importer
    Assimp::Importer importer;
    assert(importer != nullptr);

    // Load the mesh file
    const aiScene* s = importer.ReadFile("/home/Charlie/CGD-CTD/obj/teapot.obj", 0);
    assert(s != nullptr);

    // Check if the mesh was loaded successfully
    if (!s || s->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !s->mRootNode) 
    {
        std::cout << "Failed to load mesh: " << importer.GetErrorString() << std::endl;

        return false;
    }

    // Embree device and scene
    RTCDevice device = rtcNewDevice(nullptr);
    assert(device != nullptr);
    RTCScene scene = rtcNewScene(device);
    assert(scene != nullptr);

    aiMesh* mesh = s->mMeshes[0];
    assert(mesh != nullptr);

    for (size_t i = 0; i < mesh->mNumVertices / 3; i++)
    {
        RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);

        auto* vb = static_cast<float*> (rtcSetNewGeometryBuffer(geom,
            RTC_BUFFER_TYPE_VERTEX, 0, RTC_FORMAT_FLOAT3, 3*sizeof(float), 3));

        for (size_t j = 0; j < 3; j++)
        {
            vb[j*3 + 0] = mesh->mVertices[3*i+j].x;
            vb[j*3 + 1] = mesh->mVertices[3*i+j].y;
            vb[j*3 + 2] = mesh->mVertices[3*i+j].z;
        }

        auto* ib = static_cast<unsigned*> (rtcSetNewGeometryBuffer(geom,
            RTC_BUFFER_TYPE_INDEX, 0, RTC_FORMAT_UINT3, 3*sizeof(unsigned), 1));

        ib[0] = 0; ib[1] = 1; ib[2] = 2;
        

        rtcCommitGeometry(geom);
        rtcAttachGeometry(scene, geom);
        rtcReleaseGeometry(geom);        
    }

    rtcCommitScene(scene);

    // Image size
    size_t width = 1000;
    size_t height = 1000;

    //PPM header
    PPMWriteHeader(std::cout, width, height);

    int half_width = static_cast<int>(width) / 2;
    int half_height = static_cast<int>(height) / 2;  

    // PPM body
    for (int vertical = -half_height; vertical < half_height; vertical++)
    {
        for (int horizontal = -half_width; horizontal < half_width; horizontal++)
        {
            // Create ray
            RTCRayHit ray;
            ray.ray.org_x =  0.0F;
            ray.ray.org_y =  0.0F;
            ray.ray.org_z =  -10.0F;

            //TODO: Adjust angle of ray based on pixel position
            float pixel_width  = 2.0F / static_cast<float>(width);
            float pixel_height = 2.0F / static_cast<float>(height);
            ray.ray.dir_x = static_cast<float>(horizontal)  * pixel_width;
            ray.ray.dir_y = static_cast<float>(vertical)    * pixel_height;
            ray.ray.dir_z = 1.0F;

            ray.ray.tnear = 0.001F; // Set the minimum distance to start tracing
            ray.ray.tfar  = std::numeric_limits<float>::infinity();
            ray.ray.mask  = -1;

            // default geomID to invalid
            ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;

            // Create context
            RTCIntersectContext context;
            rtcInitIntersectContext(&context);

            // Trace the ray against the scene
            rtcIntersect1(scene, &context, &ray);

            RGB final_colour = (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID ? CT::WHITE : CT::BLACK);

            PPMWritePixel(std::cout, final_colour);
        }
    }

    rtcReleaseScene(scene);
    rtcReleaseDevice(device);

    return true;
}