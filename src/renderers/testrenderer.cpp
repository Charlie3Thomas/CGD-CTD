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

static void RenderCanvas(Canvas& canvas, const Camera& camera)
{
    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // Retrieve embree singleton instance
    EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

    // Light direction
    Eigen::Vector3f light_dir(-1.0F, 1.0F, -1.0F);
    light_dir.normalize();

    // Specify material
    //Material mat { RGB{1.0F, 0.0F, 0.0F}, 0.5F, 0.5F, 0.5F };

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
            //rtcIntersect1(EmbreeSingleton::GetInstance().scene, &context, &ray);
            rtcIntersect1(embree.scene, &context, &ray);

            auto pixel_ref = canvas(x, y);

            if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
            {
                // Find the object hit by the ray
                RTCGeometry rtcg = rtcGetGeometry(embree.scene, ray.hit.geomID);
                const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(rtcg));

                Eigen::Vector3f hit_normal(ray.hit.Ng_x, ray.hit.Ng_y, ray.hit.Ng_z);
                hit_normal.normalize();

                float cos_theta = std::max(0.0F, std::abs(hit_normal.dot(light_dir)));

                Eigen::Vector3f raydir = Eigen::Vector3f(ray.ray.dir_x, ray.ray.dir_y, ray.ray.dir_z);

                // Calculate reflected ray direction
                Eigen::Vector3f reflection = (2.0F * hit_normal - raydir);
                reflection.normalize();

                float scale = -hit_normal.dot(raydir) * 1.0F * std::abs(light_dir.dot(reflection));

                if (config.visualise_normals)
                {
                    // // Draw a colour depending on the normals
                    RGB colour = FromIntersectNormal(ray.hit);
                    pixel_ref.r = colour.r;
                    pixel_ref.g = colour.g;
                    pixel_ref.b = colour.b;
                }
                else
                {
                    if (obj->texture != nullptr)
                    {
                        RGB colour = FromTexture(ray.hit, obj->texture);
                        pixel_ref.r = colour.r * scale * cos_theta;
                        pixel_ref.g = colour.g * scale * cos_theta;
                        pixel_ref.b = colour.b * scale * cos_theta;
                    }
                    else
                    {
                        pixel_ref.r = obj->material->base_colour.r * scale * cos_theta;
                        pixel_ref.g = obj->material->base_colour.g * scale * cos_theta;
                        pixel_ref.b = obj->material->base_colour.b * scale * cos_theta;
                    }
                    // //Draw a colour representing the texture
                    // RGB colour = FromTexture(ray.hit, embree.GetTri(ray.hit.primID), tex);
                    // pixel_ref.r = colour.r;
                    // pixel_ref.g = colour.g;
                    // pixel_ref.b = colour.b;

                    

                    // // Draw a colour representing bary coords
                    // RGB colour = FromBaryCoords(ray.hit);
                    // pixel_ref.r = colour.r;
                    // pixel_ref.g = colour.g;
                    // pixel_ref.b = colour.b;
                }
            }
            else
            {
                pixel_ref.r = 0.0F;
                pixel_ref.g = 0.0F;
                pixel_ref.b = 0.0F;
            }

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


// for (size_t i = 0; i < film.canvases.size(); i+=8)
// {
//     auto f1 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 0]), std::ref(camera));
//     auto f2 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 1]), std::ref(camera));
//     auto f3 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 2]), std::ref(camera));
//     auto f4 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 3]), std::ref(camera));
//     auto f5 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 4]), std::ref(camera));
//     auto f6 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 5]), std::ref(camera));
//     auto f7 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 6]), std::ref(camera));
//     auto f8 = std::async(std::launch::async, RenderCanvas, std::ref(film.canvases[i + 7]), std::ref(camera));
// }
// // for each canvas
// for (auto& canvas : film.canvases)
// {
//     // Render the canvas
//     RenderCanvas(canvas, camera);
// }
}