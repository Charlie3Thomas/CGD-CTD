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
//#include "materials/mat.hpp"
#include "textures/texture.hpp"
#include "utils/depthcounter.hpp"
#include "utils/rgb.hpp"
#include "utils/exr.hpp"
#include "utils/ppm.hpp"
#include "utils/timer.hpp"
#include "utils/utils.hpp"

//#include  "lights/light.hpp"
#include  "loaders/scene.hpp"

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

static RTCRayHit CastRay(const Vector3f& origin, const Vector3f& direction, float tfar, RTCIntersectContext& context)
{
    RTCRayHit ret;
    ret.ray.org_x  = origin.x();
    ret.ray.org_y  = origin.y();
    ret.ray.org_z  = origin.z();
    ret.ray.dir_x  = direction.x();
    ret.ray.dir_y  = direction.y();
    ret.ray.dir_z  = direction.z();
    ret.ray.tnear  = 0.01F;
    ret.ray.tfar   = tfar;
    ret.ray.mask   = 0xFFFFFFFF;
    ret.hit.geomID = RTC_INVALID_GEOMETRY_ID;

    rtcIntersect1(EmbreeSingleton::GetInstance().scene, &context, &ret);

    return ret;    
}

struct CWHData
{
    Vector3f dir;
    float pdf;
};

static CWHData SampleCosineWeightedHemisphere(const Vector3f& n)
{
    // Generate random point on hemisphere
    float u           = RandomRange(0.0F, 1.0F);
    float v           = RandomRange(0.0F, 1.0F);
    float psi         = 2.0F * std::numbers::pi_v<float> * u;
    float cos_veriphi = std::sqrt(1.0F - v);
    float sin_veriphi = std::sqrt(1.0F - cos_veriphi * cos_veriphi);

    // Convert to cartesian coordinates
    Vector3f hemisphere_dir 
    {
        std::cos(psi) * sin_veriphi,
        std::sin(psi) * sin_veriphi,
        cos_veriphi 
    };
    
    // Convert to world space
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
        .pdf = hemisphere_dir.z() / std::numbers::pi_v<float>
    };

    assert(ret.dir.norm() > 0.0F);

    return { ret };
}


/// @brief Handle ray hits and calculate the colour of the pixel from environment
/// @param ray 
/// @return CT::RGB colour of the pixel
static RGB HandleHit(const RTCRayHit& rh, RTCIntersectContext& context)
{
    const EmbreeSingleton& es = EmbreeSingleton::GetInstance();

    RGB returned_pixel_colour_value = BLACK;
    const RTCGeometry incident_geometry = rtcGetGeometry(es.scene, rh.hit.geomID);
    const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(incident_geometry));
    Vector3f incident_shading_normal = InterpolateNormals(incident_geometry, rh.hit);
    if (ConfigSingleton::GetInstance().visualise_normals)
        return FromNormal(incident_shading_normal);
    Vector3f incident_direction { rh.ray.dir_x, rh.ray.dir_y, rh.ray.dir_z };
    Vector3f incident_reflection = (incident_direction - 2.0F * incident_shading_normal * incident_shading_normal.dot(incident_direction)).normalized();
    Vector3f incident_hit_worldspace { rh.ray.org_x + rh.ray.dir_x * rh.ray.tfar, rh.ray.org_y + rh.ray.dir_y * rh.ray.tfar, rh.ray.org_z + rh.ray.dir_z * rh.ray.tfar };
    //const Lights lights = double_dragon.lights;
    const Lights lights = testscene2.lights;

    // TODO: Only contribute the light's colour if the last material was specular

    size_t samples = 16;
    RGB sample_light = BLACK;
    for (size_t sample = 0; sample < samples; sample++)
    {
        // Calculate direct lighting
        sample_light += EvaluateLighting(incident_hit_worldspace, incident_shading_normal, incident_reflection, obj, lights, context);

        // Calculate indirect lighting with cosine weighted hemisphere sampling
        CWHData hemisphere_sample = SampleCosineWeightedHemisphere(incident_shading_normal);
        RTCRayHit hemisphere_sample_ray = CastRay(incident_hit_worldspace, hemisphere_sample.dir, std::numeric_limits<float>::infinity(), context);
        if (hemisphere_sample_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            const RTCGeometry hemisphere_sample_geometry = rtcGetGeometry(es.scene, hemisphere_sample_ray.hit.geomID);
            const auto* hemisphere_sample_obj = static_cast<const Object*>(rtcGetGeometryUserData(hemisphere_sample_geometry));
            Vector3f hemisphere_sample_shading_normal = InterpolateNormals(hemisphere_sample_geometry, hemisphere_sample_ray.hit);
            Vector3f hemisphere_sample_reflection = (hemisphere_sample.dir - 2.0F * hemisphere_sample_shading_normal * hemisphere_sample_shading_normal.dot(hemisphere_sample.dir)).normalized();
            Vector3f hemisphere_sample_hit_worldspace {
                hemisphere_sample_ray.ray.org_x + hemisphere_sample_ray.ray.dir_x * hemisphere_sample_ray.ray.tfar,
                hemisphere_sample_ray.ray.org_y + hemisphere_sample_ray.ray.dir_y * hemisphere_sample_ray.ray.tfar,
                hemisphere_sample_ray.ray.org_z + hemisphere_sample_ray.ray.dir_z * hemisphere_sample_ray.ray.tfar };
            sample_light += EvaluateLighting(hemisphere_sample_hit_worldspace, hemisphere_sample_shading_normal, hemisphere_sample_reflection, hemisphere_sample_obj, lights, context);
        }


        sample_light = sample_light / static_cast<float>(samples);
        returned_pixel_colour_value += sample_light;
    }

    static thread_local DepthCounter counter;
    auto increment = counter.Increment();
    if (counter.GetDepth() < 3)
    {
        Vector3f offset_reflection = (incident_reflection + Vector3f::Random() * (1.0F - obj->material->shininess)).normalized();
        RTCRayHit refl_ray = CastRay(incident_hit_worldspace, offset_reflection, std::numeric_limits<float>::infinity(), context);
        if (refl_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
            returned_pixel_colour_value += HandleHit(refl_ray, context);
    }

    return returned_pixel_colour_value / static_cast<float>(counter.GetDepth());
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
                DrawColourToCanvas(pixel_ref, HandleHit(ray, context));
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