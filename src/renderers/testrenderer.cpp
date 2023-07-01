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
#include "materials/mat.hpp"
#include "materials/phong.hpp"
#include "textures/texture.hpp"
#include "utils/rgb.hpp"
#include "utils/exr.hpp"
#include "utils/ppm.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"

#include  "lights/light.hpp"

using namespace Eigen;

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
static Vector3f InterpolateNormals(const RTCGeometry& rtcg, const RTCHit& hit)
{
    // Interpolate normals
    std::array<float, 3> interp_P;
    rtcInterpolate0(rtcg, hit.primID, hit.u, hit.v, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, interp_P.data(), interp_P.size());
    Vector3f hit_normal(interp_P[0], interp_P[1], interp_P[2]);
    hit_normal.normalize();
    return hit_normal;
}

static RTCRay GetShadowRay(const Vector3f& ray_hit_ws, const Vector3f& light_dir_ws, float distance_to_light)
{
    // Make new ray from geom hit position to light position
    RTCRay ret
    {
        .org_x = ray_hit_ws.x(),
        .org_y = ray_hit_ws.y(),
        .org_z = ray_hit_ws.z(),
        .tnear = 0.1F,
        .dir_x = light_dir_ws.x(),
        .dir_y = light_dir_ws.y(),
        .dir_z = light_dir_ws.z(),
        .tfar = distance_to_light,
        .mask  = std::numeric_limits<unsigned int>::max()
    };

    return ret;
}

static Lights GetLights()
{
    Lights ret;
    // Test ambient light
    AmbientLight testamb
    {
        .colour = RGB{ 0.5F, 0.5F, 0.5F } * 0.04F
    };
    ret.ambient.emplace_back(testamb);

    // Test DirectionalLight
    DirectionalLight testdir
    {
        .colour = RGB{ 0.99F, 0.85F, 0.21F } * 0.08F,
        .direction = Vector3f{ 1.0F, 0.0F, 0.0F }
    };
    ret.directional.emplace_back(testdir);

    // Test PointLight
    PointLight testpoint
    { 
        .colour = RGB{ 1.0F, 1.0F, 1.0F } * 0.8F,
        .position = Vector3f{ 0.0F, 5.0F, 10.0F },
        .attenuation = 1.0F
    };
    ret.point.emplace_back(testpoint);

    return ret;
}

/// @brief Handles a ray hit and draws colours appropriately to the canvas
/// @param config The ConfigSingleton&
/// @param embree The EmbreeSingleton&
/// @param ray The incoming RTCRayHit& ray
/// @param pixel_ref The CT::Canvas::PixelRef& pixel to draw to
static RGB HandleHit(const RTCRayHit& ray)
{
    RGB pixel_colour = RGB{ 0.0F, 0.0F, 0.0F };

    // Retrieve singleton instances
    EmbreeSingleton& es = EmbreeSingleton::GetInstance();
    ConfigSingleton& cs = ConfigSingleton::GetInstance();

    // Find the object hit by the ray
    const RTCGeometry rtcg = rtcGetGeometry(EmbreeSingleton::GetInstance().scene, ray.hit.geomID);
    const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(rtcg));

    // Set up lights:
    const Lights lights = GetLights();

    // Interpolate normals and get ray direction
    //Vector3f geom_normal = Vector3f(ray.hit.Ng_x, ray.hit.Ng_y, ray.hit.Ng_z);
    Vector3f shading_normal = InterpolateNormals(rtcg, ray.hit);

    if (cs.visualise_normals) // Visualise normals
        return FromNormal(shading_normal);

    Vector3f raydir = Vector3f(ray.ray.dir_x, ray.ray.dir_y, ray.ray.dir_z);
    Vector3f incident_reflection = (raydir - 2.0F * shading_normal * shading_normal.dot(raydir)).normalized();

    // Calculate reflected ray direction
    Vector3f reflection = (2.0F * shading_normal - raydir);
    reflection.normalize();

    Vector3f ray_hit_ws = Vector3f(
        ray.ray.org_x + ray.ray.dir_x * ray.ray.tfar, 
        ray.ray.org_y + ray.ray.dir_y * ray.ray.tfar, 
        ray.ray.org_z + ray.ray.dir_z * ray.ray.tfar);

    // for ambient lights
    for (const auto& amb_light : lights.ambient)
    {
        pixel_colour += amb_light.colour * obj->material->ka;
    }

    // for directional lights
    for (const auto& dir_light : lights.directional)
    {
        RTCRay shadow_ray = GetShadowRay(ray_hit_ws, dir_light.direction, std::numeric_limits<float>::max());

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);
        rtcOccluded1(es.scene, &context, &shadow_ray);

        if (shadow_ray.tfar > 0.0F)
        {
            // Calculate the diffuse component
            float costheta = std::max(0.0F, shading_normal.dot(dir_light.direction));
            pixel_colour += obj->material->kd * dir_light.colour * costheta;

            // Calculate the specular component
            float cosphi = std::max(0.0F, incident_reflection.dot(dir_light.direction));
            pixel_colour += obj->material->ks * dir_light.colour * std::pow(cosphi, obj->material->shininess);
        }        
    }

    // for point lights
    for (const auto& point : lights.point)
    {
        Vector3f light_dir_ws = point.position - ray_hit_ws;

        // Calculate the distance to the light (vector magnitude)
        float distance_to_light = light_dir_ws.norm();
        light_dir_ws /= distance_to_light;

        RTCRay shadow_ray = GetShadowRay(ray_hit_ws, light_dir_ws, distance_to_light);

        RTCIntersectContext context;
        rtcInitIntersectContext(&context);
        rtcOccluded1(es.scene, &context, &shadow_ray);

        if (shadow_ray.tfar > 0.0F)
        {
            // Calculate the diffuse component
            float costheta = std::max(0.0F, shading_normal.dot(light_dir_ws));
            pixel_colour += obj->material->kd * point.colour * costheta;

            // Calculate the specular component
            float cosphi = std::max(0.0F, incident_reflection.dot(light_dir_ws));
            pixel_colour += obj->material->ks * point.colour * std::pow(cosphi, obj->material->shininess);
        }
    }


    // if (cs.visualise_normals) // Visualise normals as colours if enabled
    //     pixel_colour = FromNormal(shading_normal);
    // else 
    // {
    //     if (obj->texture == nullptr) // If the object has no texture, use the base colour
    //     {
    //         if (shadow_ray.tfar > 0)
    //         {
    //             //DrawColourToCanvas(pixel_ref, obj->material->base_colour);
                
    //             pixel_colour = Evaluate(obj->material, shading_normal, light_dir_ws, incident_reflection, l_intensity, a_intensity, attenuation);
    //         }            
    //     }
    //     else
    //     {
    //         assert(obj->tex_coords.contains(ray.hit.primID)); // Ensure that the object has texture coordinates
    //         pixel_colour = FromTexture(ray.hit, obj->texture, obj->tex_coords.at(ray.hit.primID));
    //     }
    // }

    return pixel_colour;
}

static void RenderCanvas(Canvas& canvas, const Camera& camera)
{
    for (size_t y = 0; y < canvas.rect.GetHeight(); y++)
    {
        for (size_t x = 0; x < canvas.rect.GetWidth(); x++)
        {
            // Retrieve singleton instances
            EmbreeSingleton& es = EmbreeSingleton::GetInstance();
            ConfigSingleton& cs = ConfigSingleton::GetInstance();

            // Get a reference to the pixel
            auto pixel_ref = canvas(x, y);

            // Get the ray for the pixel
            RTCRayHit ray = camera.GetRayForPixel(canvas, Vector2i(x, y));

            RTCIntersectContext context;            
            rtcInitIntersectContext(&context);
            context.flags = RTC_INTERSECT_CONTEXT_FLAG_COHERENT;

            // Intersect the ray with the scene
            rtcIntersect1(es.scene, &context, &ray);

            // If the ray hit something, handle the hit
            if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
                DrawColourToCanvas(pixel_ref, HandleHit(ray));
            else // Draw black background if no hit
                DrawColourToCanvas(pixel_ref, RGB(0.0F, 0.0F, 0.0F));

            // Visualise the canvases if enabled
            if (cs.visualise_canvases)
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