#include "testrenderer.hpp"
#include "threadpool.hpp"

#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <random>
#include <numbers>

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
static void DrawColourToCanvas(Canvas::PixelRef& pixel_ref, const RGB& colour)
{
    pixel_ref.r = std::clamp(colour.r, 0.0F, 1.0F);
    pixel_ref.g = std::clamp(colour.g, 0.0F, 1.0F);
    pixel_ref.b = std::clamp(colour.b, 0.0F, 1.0F);
}

static Vector3f InterpolateNormals(const RTCGeometry& rtcg, const RTCHit& hit)
{
    // Interpolate normals
    std::array<float, 3> interp_P;
    rtcInterpolate0(rtcg, hit.primID, hit.u, hit.v, RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, 0, interp_P.data(), interp_P.size());
    Vector3f hit_normal(interp_P[0], interp_P[1], interp_P[2]);
    hit_normal.normalize();
    return hit_normal;
}

static RTCRay CastShadowRay(const Vector3f& ray_hit_ws, const Vector3f& light_dir_ws, float distance_to_light)
{
    RTCRay ret {
        .org_x = ray_hit_ws.x(),
        .org_y = ray_hit_ws.y(),
        .org_z = ray_hit_ws.z(),
        .tnear = 0.1F,
        .dir_x = light_dir_ws.x(),
        .dir_y = light_dir_ws.y(),
        .dir_z = light_dir_ws.z(),
        .tfar  = distance_to_light,
        .mask  = std::numeric_limits<unsigned int>::max() };
    return ret;
}

static Lights GetLights()
{
    Lights ret;
    AmbientLight testamb     { .colour = RGB{ 0.5F, 0.5F, 0.5F }    * 0.10F };
    DirectionalLight testdir { .colour = RGB{ 0.99F, 0.85F, 0.21F } * 0.40F, .direction = Vector3f{ 1.0F, 0.0F, 0.0F } };
    PointLight testpoint     { .colour = RGB{ 1.0F, 1.0F, 1.0F }    * 0.80F, .position = Vector3f{ 0.0F, 5.0F, 10.0F }, .attenuation = 1.0F };
    ret.ambient.emplace_back(testamb);
    ret.directional.emplace_back(testdir);
    ret.point.emplace_back(testpoint);
    return ret;
}

struct CWHData
{
    Vector3f dir;
    float pdf;
};

static CWHData SampleCosineWeightedHemisphere(const Vector3f& n)
{
    float u           = RandomRange(0.0F, 1.0F);
    float v           = RandomRange(0.0F, 1.0F);
    float psi         = 2.0F * std::numbers::pi_v<float> * u;
    float sin_veriphi = std::sqrt(1.0F - v);

    Vector3f hemisphere_dir 
    {
        std::cos(psi) * sin_veriphi,
        std::sin(psi) * sin_veriphi,
        sin_veriphi 
    };
    
    float x = n.x();
    float y = n.y();
    float z = n.z();

    Matrix3f oigno;
    oigno << 1.0F, 0.0F, ((-z * x) + (2.0F * x)),
             0.0F, 1.0F, ((-z * y) + (2.0F * y)),
             (z * x - 2.0F * x), (z * y - 2.0F * y), 1.0F;
    
    CWHData ret
    {
        .dir = -(oigno * hemisphere_dir).normalized(),
        .pdf = std::sqrt(1.0F - v) / std::numbers::pi_v<float>
    };

    return { ret };
}

/// @brief Handle ray hits and calculate the colour of the pixel from environment
/// @param ray 
/// @return CT::RGB colour of the pixel
static RGB HandleHit(const RTCRayHit& rh)
{
    RGB pixel_colour = BLACK;
    const ConfigSingleton& cs = ConfigSingleton::GetInstance();
    const EmbreeSingleton& es = EmbreeSingleton::GetInstance();

    // Find the object hit by the ray
    const RTCGeometry rtcg = rtcGetGeometry(EmbreeSingleton::GetInstance().scene, rh.hit.geomID);
    const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(rtcg));

    // Interpolate normals and get ray direction
    Vector3f shading_normal = InterpolateNormals(rtcg, rh.hit);
    if (cs.visualise_normals) // Visualise normals
        return FromNormal(shading_normal);

    // Calculate rays
    Vector3f raydir = Vector3f(rh.ray.dir_x, rh.ray.dir_y, rh.ray.dir_z);
    Vector3f incident_reflection = (raydir - 2.0F * shading_normal * shading_normal.dot(raydir)).normalized();
    Vector3f reflection = (2.0F * shading_normal - raydir);
    reflection.normalize();

    Vector3f ray_hit_ws = Vector3f(
        rh.ray.org_x + rh.ray.dir_x * rh.ray.tfar, 
        rh.ray.org_y + rh.ray.dir_y * rh.ray.tfar, 
        rh.ray.org_z + rh.ray.dir_z * rh.ray.tfar);

    size_t samples = 1000;
    RGB sample_colour{ 0.0F, 0.0F, 0.0F };
#if 1
    for (size_t i = 0; i < samples; i++)
    {
        CWHData hem_sample = SampleCosineWeightedHemisphere(shading_normal);

        const Lights lights = GetLights();
        // for ambient lights
        for (const auto& amb_light : lights.ambient)
            sample_colour += amb_light.colour * obj->material->ka;

        // for directional lights
        for (const auto& dir_light : lights.directional)
        {
            Vector3f light_dir = dir_light.direction;
            float cos_theta = -light_dir.dot(hem_sample.dir);
            float cosphi = std::max(0.0F, -light_dir.dot(hem_sample.dir));
            if (cos_theta > 0.0F)
            {
                sample_colour += dir_light.colour * obj->material->kd * cos_theta * hem_sample.pdf;
                sample_colour += dir_light.colour * obj->material->ks * std::pow(cosphi, obj->material->shininess) * hem_sample.pdf;
            }
        }

        // for point lights
        for (const auto& point : lights.point)
        {
            Vector3f light_dir = point.position - ray_hit_ws;
            float distance = light_dir.norm();
            light_dir /= distance; // normalize

            float cos_theta = -light_dir.dot(hem_sample.dir);
            float cosphi = std::max(0.0F, -light_dir.dot(hem_sample.dir));
            if (cos_theta > 0.0F)
            {
                sample_colour += point.colour * obj->material->kd * cos_theta * hem_sample.pdf / distance;
                sample_colour += point.colour * obj->material->ks * std::pow(cosphi, obj->material->shininess) * hem_sample.pdf / distance;
            }
        }

        sample_colour  = sample_colour / static_cast<float>(samples);
        pixel_colour  += sample_colour;
    }

#else
    for (size_t i = 0; i < samples; i++)
    {
        float u         = RandomRange(0.0F, 1.0F);
        float v         = RandomRange(0.0F, 1.0F);
        auto phi        = static_cast<float>(2.0F * M_PI * u);
        float cos_theta = std::sqrt(1.0F - v);
        float sin_theta = std::sqrt(v);
        //float pdf = 1.0F / (2.0F * M_PI);
        auto pdf       = static_cast<float>(cos_theta / M_PI);

        Vector3f hemishpere_dir {
            std::cos(phi) * sin_theta,
            std::sin(phi) * sin_theta,
            cos_theta };

        Vector3f tangent = shading_normal.cross(Vector3f(0.0F, 0.0F, 1.0F));
        Vector3f bitangent = shading_normal.cross(tangent);
        Vector3f sample_dir = Vector3f(
            hemishpere_dir.x() * tangent.x() + hemishpere_dir.y() * bitangent.x() + hemishpere_dir.z() * shading_normal.x(),
            hemishpere_dir.x() * tangent.y() + hemishpere_dir.y() * bitangent.y() + hemishpere_dir.z() * shading_normal.y(),
            hemishpere_dir.x() * tangent.z() + hemishpere_dir.y() * bitangent.z() + hemishpere_dir.z() * shading_normal.z());
        
        // Weight sample_dir by incident_reflection to bias towards the direction of the incident ray based on the shininess of the material
        sample_dir = (sample_dir + incident_reflection * obj->material->shininess).normalized();

        const Lights lights = GetLights();
        // for ambient lights
        for (const auto& amb_light : lights.ambient)
            sample_colour += amb_light.colour * obj->material->ka;

        // for directional lights
        for (const auto& dir_light : lights.directional)
        {
            RTCRay shadow_ray = CastShadowRay(ray_hit_ws, dir_light.direction, std::numeric_limits<float>::max());
            RTCIntersectContext context;
            rtcInitIntersectContext(&context);
            rtcOccluded1(es.scene, &context, &shadow_ray);

            if (shadow_ray.tfar < 0) { continue; }

            // Calculate the diffuse component
            float costheta = std::max(0.0F, shading_normal.dot(dir_light.direction)); // Calculate the diffuse component
            sample_colour += obj->material->kd * dir_light.colour * costheta;

            // Calculate the specular component
            float cosphi = std::max(0.0F, sample_dir.dot(dir_light.direction)); // Calculate the specular component
            sample_colour += obj->material->ks * dir_light.colour * std::pow(cosphi, obj->material->shininess);
        }

        // for point lights
        for (const auto& point : lights.point)
        {
            Vector3f light_dir_ws = point.position - ray_hit_ws;
            float distance_to_light = light_dir_ws.norm();
            light_dir_ws /= distance_to_light;
            RTCRay shadow_ray = CastShadowRay(ray_hit_ws, light_dir_ws, distance_to_light);
            RTCIntersectContext context;
            rtcInitIntersectContext(&context);
            rtcOccluded1(es.scene, &context, &shadow_ray);

            if (shadow_ray.tfar < 0) { continue; }

            // Calculate the diffuse component
            float costheta = std::max(0.0F, shading_normal.dot(light_dir_ws));
            sample_colour += obj->material->kd * point.colour * costheta;

            // Calculate the specular component
            float cosphi = std::max(0.0F, sample_dir.dot(light_dir_ws));
            sample_colour += obj->material->ks * point.colour * std::pow(cosphi, obj->material->shininess);
        }
        
        sample_colour  = sample_colour / static_cast<float>(samples);
        pixel_colour  += sample_colour * pdf;
    }
#endif

    // pixel_colour += CalculateAmbientLighting(lights.ambient, obj->material->ka);
    // pixel_colour += CalculateDirectionalLighting(lights.directional, obj->material, shading_normal, incident_reflection, ray_hit_ws);
    // pixel_colour += CalculatePointLighting(lights.point, obj->material, shading_normal, incident_reflection, ray_hit_ws);

    return pixel_colour;
}

static void RenderCanvas(Canvas& canvas, const Camera& camera)
{
    for (size_t y = 0; y < canvas.rect.GetHeight(); y++)
    {
        for (size_t x = 0; x < canvas.rect.GetWidth(); x++)
        {
            auto pixel_ref = canvas(x, y);
            EmbreeSingleton& es = EmbreeSingleton::GetInstance();
            ConfigSingleton& cs = ConfigSingleton::GetInstance();
            RTCRayHit ray { camera.GetRayForPixel(canvas, Vector2i(x, y)) };

            RTCIntersectContext context;
            rtcInitIntersectContext(&context);
            context.flags = RTC_INTERSECT_CONTEXT_FLAG_COHERENT;

            rtcIntersect1(es.scene, &context, &ray);           
            if (ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)  // If the ray hit something, handle the hit
                DrawColourToCanvas(pixel_ref, HandleHit(ray));
            else // Draw black background if no hit
                DrawColourToCanvas(pixel_ref, BLACK);

            // Visualise the canvases if enabled
            if (cs.visualise_canvases)
                if (x == 0 || y == 0) { DrawColourToCanvas(pixel_ref, PURPLE); }
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