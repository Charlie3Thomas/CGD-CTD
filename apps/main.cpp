#include <iostream>
#include <limits>
#include <cassert>

#include <embree3/rtcore.h>

#include "utils/ppm.hpp"
#include "utils/utils.hpp"

using namespace CT;

void EmbreeTriangle();

int main()
{
    EmbreeTriangle();
    return EXIT_SUCCESS;
}

void EmbreeTriangle()
{
    // Image size
    size_t width = 100;
    size_t height = 100;
    int half_width = static_cast<int>(width) / 2;
    int half_height = static_cast<int>(height) / 2;   

    //PPM header
    PPMWriteHeader(std::cout, width, height);

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

        // struct RTCQuaternionDecomposition qd {  };
        // rtcInitQuaternionDecomposition(&qd);
        // qd.quaternion_i *= i * 90.0F;

        // rtcSetGeometryTransformQuaternion(geom, 0, &qd);

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