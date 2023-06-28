#include "testrenderer.hpp"
#include "threadpool.hpp"

#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <Eigen/Dense>

#include "bvh/bvh.hpp"
#include "camera/camera.hpp"
#include "camera/film.hpp"
#include "config/options.hpp"
#include "embree/embreesingleton.hpp"
#include "loaders/objloader.hpp"
#include "materials/material.hpp"
#include "textures/texture.hpp"
#include "utils/rgb.hpp"
#include "utils/exr.hpp"
#include "utils/ppm.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"


namespace CT
{

Vector3f InterpolateNormals(const RTCRayHit rtcray, const Object* obj)
{
    // Triangle Vertices
    std::array<Vector3f, 3> vertices;

    // Vertex Normals
    std::array<Vector3f, 3> normals;

    // Get triangle vertices
    const auto& triangle = obj->triangles[rtcray.hit.primID];
    for (int i = 0; i < 3; i++)
    {
        vertices[i] = triangle.v[i].pos;
        normals[i] = triangle.v[i].vertex_normal;
    }

    // Barycentric coordinates
    Vector3f bary = Vector3f(rtcray.hit.u, rtcray.hit.v, 1.0F - rtcray.hit.u - rtcray.hit.v);

    Vector3f ret = Vector3f(0.0F, 0.0F, 0.0F);

    // Interpolate vertex normals using bary coords
    ret = Vector3f::Zero();

    for (int i = 0; i < 3; i++)
        ret += bary[i] * normals[i];

    ret.normalize();

    return ret;
}

static void RenderCanvas(Canvas& canvas, const Camera& camera)
{
    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // Retrieve embree singleton instance
    EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

    // Light direction
    Eigen::Vector3f light_dir(1.0F, 1.0F, 1.0F);
    light_dir.normalize();

    for (size_t y = 0; y < canvas.rect.GetHeight(); y++)
    {
        for (size_t x = 0; x < canvas.rect.GetWidth(); x++)
        {
            RTCRayHit ray = camera.GetRayForPixel(canvas, Eigen::Vector2i(x, y));

            // default geomID to invalid
            ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;

            // Create context
            RTCIntersectContext context; //TODO: get things
            rtcInitIntersectContext(&context);

            // Trace the ray against the scene
            rtcIntersect1(embree.scene, &context, &ray);

            auto pixel_ref = canvas(x, y);

            if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
            {
                // Find the object hit by the ray
                RTCGeometry rtcg = rtcGetGeometry(embree.scene, ray.hit.geomID);
                const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(rtcg));                
                
                std::array<float, 3> interp_P;
                rtcInterpolate0(rtcg, ray.hit.primID, ray.hit.u, ray.hit.v, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, interp_P.data(), interp_P.size());

                Vector3f hit_normal(interp_P[0], interp_P[1], interp_P[2]);
                hit_normal.normalize();
                //float cos_theta = std::max(0.0F, std::abs(hit_normal.dot(light_dir)));

                Vector3f raydir = Vector3f(ray.ray.dir_x, ray.ray.dir_y, ray.ray.dir_z);

                // Calculate reflected ray direction
                Vector3f reflection = (2.0F * hit_normal - raydir);
                reflection.normalize();

                //float scale = -hit_normal.dot(raydir) * 1.0F * std::abs(light_dir.dot(reflection));
                float scale = -hit_normal.dot(raydir);

                if (config.visualise_normals)
                {
                    // // Draw a colour depending on the normals
                    //RGB colour = FromIntersectNormal(ray.hit);
                    RGB colour = FromNormal(hit_normal);
                    pixel_ref.r = colour.r;
                    pixel_ref.g = colour.g;
                    pixel_ref.b = colour.b;
                }
                else
                {
                    if (obj->texture != nullptr)
                    {
                        // Do a wonky lil assertion, don't keep it like this
                        assert(obj->tex_coords.contains(ray.hit.primID));
                        RGB colour = FromTexture(ray.hit, obj->texture, obj->tex_coords.at(ray.hit.primID));
                        pixel_ref.r = colour.r;
                        pixel_ref.g = colour.g;
                        pixel_ref.b = colour.b;
                    }
                    else
                    {
                        pixel_ref.r = std::clamp(obj->material->base_colour.r * scale, 0.0F, 1.0F);
                        pixel_ref.g = std::clamp(obj->material->base_colour.g * scale, 0.0F, 1.0F);
                        pixel_ref.b = std::clamp(obj->material->base_colour.b * scale, 0.0F, 1.0F);
                    }                    
                }
            }
            else
            {
                pixel_ref.r = 0.0F;
                pixel_ref.g = 0.0F;
                pixel_ref.b = 0.0F;
            }

            assert(pixel_ref.r >= 0.0F && pixel_ref.r <= 1.01F);
            assert(pixel_ref.g >= 0.0F && pixel_ref.g <= 1.01F);
            assert(pixel_ref.b >= 0.0F && pixel_ref.b <= 1.01F);

            if (config.visualise_canvases)
            {
                // Overwrite the pixel in the canvas
                if (x == 0 || y == 0)
                {
                    pixel_ref.r = 0.0F;
                    pixel_ref.g = 1.0F;
                    pixel_ref.b = 1.0F;
                }
            }
        }
    }
}

void TestRenderer::RenderFilm(Film& film, Camera& camera, size_t threads)
{ 
    // Assert that the number of threads is valid
    assert(threads > 0 && threads <= std::thread::hardware_concurrency());

    std::cout << "Rendering film with " << threads << " threads" << std::endl;

    // Lambda function to measure time
    Timer t("RenderFilm");

    // Create a thread pool
    ThreadPool pool(threads);

    // Create a vector of futures
    std::vector<std::future<void>> futures;

    // Reserve space for the futures
    futures.reserve(film.canvases.size());

    for (auto& canvas : film.canvases)
    {   
        // Enqueue the task for each canvas
        futures.emplace_back(pool.enqueue(RenderCanvas, std::ref(canvas), std::ref(camera)));
    }
}
}