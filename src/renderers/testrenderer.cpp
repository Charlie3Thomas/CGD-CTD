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

#include "materials/brdfdata.hpp"


namespace CT
{

/// @brief Takes a pixel reference and a colour and draws the colour to the pixel reference
/// @param pixel_ref The Canvas::PixelRef& pixel to draw to
/// @param colour The RGB colour to draw to the pixel
static void DrawColourToCanvas(Canvas::PixelRef& pixel_ref, const RGB& colour)
{
    pixel_ref.r = std::clamp(colour.r, 0.0F, 1.0F);
    pixel_ref.g = std::clamp(colour.g, 0.0F, 1.0F);
    pixel_ref.b = std::clamp(colour.b, 0.0F, 1.0F);
}

/// @brief Interpolates the normals at the hit point
/// @param rtcg The geometry object
/// @param hit The incoming ray hit information
/// @return Returns the interpolated normal
static Vector3f InterpolateNormals(RTCGeometry& rtcg, RTCHit& hit)
{
    // Interpolate normals
    std::array<float, 3> interp_P;
    rtcInterpolate0(rtcg, hit.primID, hit.u, hit.v, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, interp_P.data(), interp_P.size());
    Vector3f hit_normal(interp_P[0], interp_P[1], interp_P[2]);
    hit_normal.normalize();
    return hit_normal;
}

/// @brief Handles a ray hit and draws colours appropriately to the canvas
/// @param config The ConfigSingleton&
/// @param embree The EmbreeSingleton&
/// @param ray The incoming RTCRayHit& ray
/// @param pixel_ref The CT::Canvas::PixelRef& pixel to draw to
static void HandleHit(const ConfigSingleton& config, EmbreeSingleton& embree, RTCRayHit& ray, CT::Canvas::PixelRef& pixel_ref)
{
    // Find the object hit by the ray
    RTCGeometry rtcg = rtcGetGeometry(embree.scene, ray.hit.geomID);
    const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(rtcg)); 

    // Interpolate normals and get ray direction
    Vector3f geom_normal = Vector3f(ray.hit.Ng_x, ray.hit.Ng_y, ray.hit.Ng_z).normalized();
    Vector3f shading_normal = InterpolateNormals(rtcg, ray.hit).normalized();
    Vector3f raydir = Vector3f(ray.ray.dir_x, ray.ray.dir_y, ray.ray.dir_z).normalized();

    // Calculate reflected ray direction
    Vector3f reflection = (2.0F * shading_normal - raydir);
    reflection.normalize();

    Vector3f base_colour = Vector3f{ obj->material->base_colour.r, obj->material->base_colour.g, obj->material->base_colour.b };
    float metalness = 0.0F;
    Vector3f emissive = Vector3f{ 0.0F, 0.0F, 0.0F };
    float roughness = 0.2F;
    float transmissivness = 1.0F;
    float opacity = 1.0F;

    MaterialProperties testmat { base_colour, metalness, emissive, roughness, transmissivness, opacity };
    // RGB colour_avg = RGB{ 0.0F, 0.0F, 0.0F };
    // for (size_t i = 0; i < 100; i++)
    // {
    //     Vector2f u = Vector2f{ RandomRange(0.0F, 1.0F), RandomRange(0.0F, 1.0F) };
    //     Vector3f sample_weight = Vector3f{ 1.0F, 1.0F, 1.0F };
    //     if (EvalIndirectCombinedBDRF(u, shading_normal, geom_normal, -raydir, testmat, 0, raydir, sample_weight))
    //         colour_avg += EvalCombined(shading_normal, -raydir, reflection, testmat);
    // }

    // DrawColourToCanvas(pixel_ref, (colour_avg / 100.0F));

    if (config.visualise_normals) // Visualise normals as colours if enabled
        DrawColourToCanvas(pixel_ref, FromNormal(shading_normal));
    else 
    {
        if (obj->texture == nullptr) // If the object has no texture, use the base colour
        {
#if 1
            Vector2f u = Vector2f{ RandomRange(0.0F, 1.0F), RandomRange(0.0F, 1.0F) };
            Vector3f sample_weight = Vector3f{ RandomRange(0.0F, 1.0F), RandomRange(0.0F, 1.0F), RandomRange(0.0F, 1.0F) };
            if (EvalIndirectCombinedBDRF(u, shading_normal, geom_normal, -raydir, testmat, raydir, sample_weight))
                DrawColourToCanvas(pixel_ref, EvalCombined(shading_normal, -raydir, reflection, testmat));
#else
            DrawColourToCanvas(pixel_ref, EvalCombined(shading_normal, -raydir, reflection, testmat));
#endif
        }
        else
        {
            assert(obj->tex_coords.contains(ray.hit.primID)); // Ensure that the object has texture coordinates
            DrawColourToCanvas(pixel_ref, FromTexture(ray.hit, obj->texture, obj->tex_coords.at(ray.hit.primID)));
        }
    }
}

static void RenderCanvas(Canvas& canvas, const Camera& camera)
{
    // Retrieve config singleton instance
    const ConfigSingleton& config = ConfigSingleton::GetInstance();

    // Retrieve embree singleton instance
    EmbreeSingleton& embree = EmbreeSingleton::GetInstance();

    for (size_t y = 0; y < canvas.rect.GetHeight(); y++)
    {
        for (size_t x = 0; x < canvas.rect.GetWidth(); x++)
        {
            // Get a reference to the pixel
            auto pixel_ref = canvas(x, y);

            // Get the ray for the pixel
            RTCRayHit ray = camera.GetRayForPixel(canvas, Vector2i(x, y));
            RTCIntersectContext context;

            // Intersect the ray with the scene
            rtcInitIntersectContext(&context);
            rtcIntersect1(embree.scene, &context, &ray);            

            // If the ray hit something, handle the hit
            if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
                HandleHit(config, embree, ray, pixel_ref);
            else // Draw black background if no hit
                DrawColourToCanvas(pixel_ref, RGB(0.0F, 0.0F, 0.0F));

            // Visualise the canvases if enabled
            if (config.visualise_canvases)
                if (x == 0 || y == 0) { DrawColourToCanvas(pixel_ref, RGB(0.0F, 1.0F, 1.0F)); }
        }
    }
}

void TestRenderer::RenderFilm(Film& film, Camera& camera, size_t threads)
{ 
    Timer t("RenderFilm");
    // Assert that the number of threads is valid
    assert(threads > 0 && threads <= std::thread::hardware_concurrency());

    std::cout << "Rendering film with " << threads << " threads" << std::endl;
 
    ThreadPool pool(threads);               // Create a thread pool    
    std::vector<std::future<void>> futures; // Create a vector of futures    
    futures.reserve(film.canvases.size());  // Reserve space for the futures

    for (auto& canvas : film.canvases) // Enqueue the task for each canvas
        futures.emplace_back(pool.enqueue(RenderCanvas, std::ref(canvas), std::ref(camera)));
}
}