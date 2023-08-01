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
    
    Vector3f u_basis;
    if (std::abs(n.x()) > std::abs(n.y()))
    {
        float ilen = 1.0F / sqrtf(n.x() * n.x() + n.z() * n.z());
        u_basis = Vector3f(-n.z() * ilen, 0, n.x() * ilen);
    } 
    else
    {
        float ilen = 1.0F / sqrtf(n.y() * n.y() + n.z() * n.z());
        u_basis = Vector3f(0, n.z() * ilen, -n.y() * ilen);
    }
    Vector3f v_basis = n.cross(u_basis);
    u_basis = n.cross(v_basis);

    Vector3f retdir = Vector3f(
        hemisphere_dir.x() * u_basis.x() + hemisphere_dir.y() * v_basis.x() + hemisphere_dir.z() * n.x(),
        hemisphere_dir.x() * u_basis.y() + hemisphere_dir.y() * v_basis.y() + hemisphere_dir.z() * n.y(),
        hemisphere_dir.x() * u_basis.z() + hemisphere_dir.y() * v_basis.z() + hemisphere_dir.z() * n.z());
 
    CWHData ret
    {
        .dir = retdir,
        .pdf = hemisphere_dir.z() / std::numbers::pi_v<float>
    };

    assert(ret.pdf > 0.0F && ret.pdf <= 1.0F);
    assert(ret.dir.norm() > 0.0F);

    return { ret };
}

static RGB PerformSample(const RTCRayHit& rh, RTCIntersectContext& context, size_t recursion_depth, RGB path_throughput = WHITE)
{   
    // Initialise return value
    RGB returned_pixel_colour_value = BLACK;
    // Get singletons
    const EmbreeSingleton& es = EmbreeSingleton::GetInstance();
    const ConfigSingleton& cs = ConfigSingleton::GetInstance();
    // Get environment
    const Lights lights = cs.environment.lights;
    const RTCGeometry incident_geometry = rtcGetGeometry(es.scene, rh.hit.geomID);
    const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(incident_geometry));
    // Calculate vectors on hit object
    Vector3f incident_shading_normal = InterpolateNormals(incident_geometry, rh.hit);
    Vector3f incident_direction { rh.ray.dir_x, rh.ray.dir_y, rh.ray.dir_z };
    Vector3f incident_reflection = (incident_direction - 2.0F * incident_shading_normal * incident_shading_normal.dot(incident_direction)).normalized();
    Vector3f incident_hit_worldspace { rh.ray.org_x + rh.ray.dir_x * rh.ray.tfar, rh.ray.org_y + rh.ray.dir_y * rh.ray.tfar, rh.ray.org_z + rh.ray.dir_z * rh.ray.tfar };
    if (ConfigSingleton::GetInstance().visualise_normals)
        return FromNormal(incident_shading_normal); 

    // Calculate direct lighting
    if(recursion_depth > 0)
    {
        RGB direct_sample = BLACK;
        direct_sample += path_throughput * EvaluateLighting(incident_hit_worldspace, incident_shading_normal, incident_reflection, obj, lights, context);
        returned_pixel_colour_value += direct_sample;
    }

    // Sample indirect - THIS SHOULD BE ON THE MATERIAL AND BE DONE FOR DIFFUSE MATERIALS ONLY
    RGB indirect_sum = BLACK;
    for (size_t i = 0; i < (recursion_depth == 0 ? cs.indirect_samples : 1); i++) // Do N hemisphere samples if depth is 0, otherwise do 1
    {
        RGB indirect = BLACK;
        CWHData hemisphere_sample = SampleCosineWeightedHemisphere(incident_shading_normal);
        RTCRayHit hemisphere_sample_ray = CastRay(incident_hit_worldspace, hemisphere_sample.dir, std::numeric_limits<float>::infinity(), context);
        if (hemisphere_sample_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
        	const RTCGeometry hemisphere_sample_geometry = rtcGetGeometry(es.scene, hemisphere_sample_ray.hit.geomID);

            // Compute hemisphere sample reflection vectors
        	Vector3f hemisphere_sample_shading_normal = InterpolateNormals(hemisphere_sample_geometry, hemisphere_sample_ray.hit);
        	Vector3f hemisphere_sample_reflection = (hemisphere_sample.dir - 2.0F * hemisphere_sample_shading_normal * hemisphere_sample_shading_normal.dot(hemisphere_sample.dir)).normalized();
        	//Vector3f hemisphere_sample_hit_worldspace {
        	//	hemisphere_sample_ray.ray.org_x + hemisphere_sample_ray.ray.dir_x * hemisphere_sample_ray.ray.tfar,
        	//	hemisphere_sample_ray.ray.org_y + hemisphere_sample_ray.ray.dir_y * hemisphere_sample_ray.ray.tfar,
        	//	hemisphere_sample_ray.ray.org_z + hemisphere_sample_ray.ray.dir_z * hemisphere_sample_ray.ray.tfar };

        	// Update paththrought
            float cosphi = std::max(0.0F, incident_reflection.dot(hemisphere_sample.dir));
            RGB bsdf = (obj->material->kd / std::numbers::pi_v<float>) + (obj->material->ks * ((obj->material->shininess + 2.0F) / (2.0F * std::numbers::pi_v<float>)) * std::pow(cosphi, obj->material->shininess));
        	// path_throughput *= (bsdf * std::abs((hemisphere_sample_shading_normal.dot(hemisphere_sample.dir)) / hemisphere_sample.pdf));
        	path_throughput *= (bsdf * std::abs((incident_shading_normal.dot(hemisphere_sample.dir)) / hemisphere_sample.pdf));

        	// Recurse for N indirect samples
        	if (recursion_depth < cs.recursion_depth)
        	{
                RTCRayHit refl_ray = CastRay(incident_hit_worldspace, hemisphere_sample_reflection, std::numeric_limits<float>::infinity(), context);
                if (refl_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
                {
                    indirect = PerformSample(refl_ray, context, recursion_depth + 1, path_throughput);
                    indirect_sum += indirect;
                }
        	}
        }
    }

    returned_pixel_colour_value += indirect_sum / static_cast<float>(cs.indirect_samples);

    // Recurse for reflections
    if (obj->material->mirror && recursion_depth < cs.recursion_depth)
    {        
        Vector3f offset_reflection = (incident_reflection + Vector3f::Random() * (1.0F - obj->material->shininess)).normalized();
        RTCRayHit refl_ray = CastRay(incident_hit_worldspace, offset_reflection, std::numeric_limits<float>::infinity(), context);

        // Get reflected ray direction
        Vector3f refl_direction { refl_ray.ray.dir_x, refl_ray.ray.dir_y, refl_ray.ray.dir_z };

        //RTCRayHit refl_ray = CastRay(incident_hit_worldspace, incident_reflection, std::numeric_limits<float>::infinity(), context);
        // Vector3f refl_direction { refl_ray.ray.dir_x, refl_ray.ray.dir_y, refl_ray.ray.dir_z };
        if (refl_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
        {
            float cosphi = std::max(0.0F, incident_reflection.dot(incident_reflection));
            RGB bsdf = (obj->material->kd / std::numbers::pi_v<float>) + (obj->material->ks * ((obj->material->shininess + 2.0F) / (2.0F * std::numbers::pi_v<float>)) * std::pow(cosphi, obj->material->shininess));
            returned_pixel_colour_value += (PerformSample(refl_ray, context, recursion_depth + 1, WHITE) * bsdf * std::abs((incident_shading_normal.dot(refl_direction)))); /* This likely shouldn't be the hemisphere sample pdf */
            //returned_pixel_colour_value += (PerformSample(refl_ray, context, ++recursion_depth, WHITE) * bsdf * std::abs((incident_shading_normal.dot(refl_direction)))); /* ???? */
        }
    }
    
    return (returned_pixel_colour_value);
}

    /* THIS SHOULD BE IN THE MATERIAL AND BE DONE FOR MIRROR MATERIALS ONLY */
    // // Recurse for reflections ** THIS IS THE CAUSE OF THE COLOUR BLEED, CHECK NEW APPROACH WITH TOM **
    // if (recursion_depth < cs.recursion_depth)
    // {        
    //     Vector3f offset_reflection = (incident_reflection + Vector3f::Random() * (1.0F - obj->material->shininess)).normalized();
    //     RTCRayHit refl_ray = CastRay(incident_hit_worldspace, offset_reflection, std::numeric_limits<float>::infinity(), context);
    //     //RTCRayHit refl_ray = CastRay(incident_hit_worldspace, incident_reflection, std::numeric_limits<float>::infinity(), context);
    //     // Vector3f refl_direction { refl_ray.ray.dir_x, refl_ray.ray.dir_y, refl_ray.ray.dir_z };
    //     if (refl_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
    //     {
    //         float cosphi = std::max(0.0F, incident_reflection.dot(incident_reflection));
    //         RGB bsdf = (obj->material->kd / std::numbers::pi_v<float>) + (obj->material->ks * ((obj->material->shininess + 2.0F) / (2.0F * std::numbers::pi_v<float>)) * std::pow(cosphi, obj->material->shininess));
    //         returned_pixel_colour_value += (PerformSample(refl_ray, context, recursion_depth + 1, WHITE) * bsdf * std::abs((incident_shading_normal.dot(hemisphere_sample.dir)) / hemisphere_sample.pdf)); /* This likely shouldn't be the hemisphere sample pdf */
    //         //returned_pixel_colour_value += (PerformSample(refl_ray, context, ++recursion_depth, WHITE) * bsdf * std::abs((incident_shading_normal.dot(refl_direction)))); /* ???? */
    //     }
    // }

/*
/// @brief Handle ray hits and calculate the colour of the pixel from environment
/// @param ray 
/// @return CT::RGB colour of the pixel
static RGB HandleHit(const RTCRayHit& rh, RTCIntersectContext& context, RGB path_throughput)
{
    // Initialise return value
    //RGB returned_pixel_colour_value = path_throughput;

    static thread_local DepthCounter counter;
    auto increment = counter.Increment();

    // Get singletons
    const EmbreeSingleton& es = EmbreeSingleton::GetInstance();
    const ConfigSingleton& cs = ConfigSingleton::GetInstance();

    // Get environment lights
    const Lights lights = cs.environment.lights;
    
    // Get incident ray hit geometry and object
    const RTCGeometry incident_geometry = rtcGetGeometry(es.scene, rh.hit.geomID);
    const auto* obj = static_cast<const Object*>(rtcGetGeometryUserData(incident_geometry));

    // If hit object has light material, return light material colour
    if (obj->material == EmbreeSingleton::GetInstance().materials["light"].get())
        return obj->material->kd;

    // Calculate vectors on hit object
    Vector3f incident_shading_normal = InterpolateNormals(incident_geometry, rh.hit);
    if (ConfigSingleton::GetInstance().visualise_normals)
        return FromNormal(incident_shading_normal);
    Vector3f incident_direction { rh.ray.dir_x, rh.ray.dir_y, rh.ray.dir_z };
    Vector3f incident_reflection = (incident_direction - 2.0F * incident_shading_normal * incident_shading_normal.dot(incident_direction)).normalized();
    Vector3f incident_hit_worldspace { rh.ray.org_x + rh.ray.dir_x * rh.ray.tfar, rh.ray.org_y + rh.ray.dir_y * rh.ray.tfar, rh.ray.org_z + rh.ray.dir_z * rh.ray.tfar };

    // Calculate direct lighting
    RGB direct_sample = BLACK;
    //direct_sample += path_throughput * EvaluateLighting(incident_hit_worldspace, incident_shading_normal, incident_reflection, obj, lights, context);
    direct_sample += EvaluateLighting(incident_hit_worldspace, incident_shading_normal, incident_reflection, obj, lights, context);
    //returned_pixel_colour_value += direct_sample;

	// Sample indirect
	RGB indirect = BLACK;

	CWHData hemisphere_sample = SampleCosineWeightedHemisphere(incident_shading_normal);
	RTCRayHit hemisphere_sample_ray = CastRay(incident_hit_worldspace, hemisphere_sample.dir, std::numeric_limits<float>::infinity(), context);
	if (hemisphere_sample_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		const RTCGeometry hemisphere_sample_geometry = rtcGetGeometry(es.scene, hemisphere_sample_ray.hit.geomID);
		const auto* hemisphere_sample_obj = static_cast<const Object*>(rtcGetGeometryUserData(hemisphere_sample_geometry));

        // Compute hemisphere sample reflection vectors
		Vector3f hemisphere_sample_shading_normal = InterpolateNormals(hemisphere_sample_geometry, hemisphere_sample_ray.hit);
		Vector3f hemisphere_sample_reflection = (hemisphere_sample.dir - 2.0F * hemisphere_sample_shading_normal * hemisphere_sample_shading_normal.dot(hemisphere_sample.dir)).normalized();
		Vector3f hemisphere_sample_hit_worldspace{
			hemisphere_sample_ray.ray.org_x + hemisphere_sample_ray.ray.dir_x * hemisphere_sample_ray.ray.tfar,
			hemisphere_sample_ray.ray.org_y + hemisphere_sample_ray.ray.dir_y * hemisphere_sample_ray.ray.tfar,
			hemisphere_sample_ray.ray.org_z + hemisphere_sample_ray.ray.dir_z * hemisphere_sample_ray.ray.tfar };

		// Update paththrought
        float cosphi = std::max(0.0F, incident_reflection.dot(hemisphere_sample.dir));
        //RGB bsdf = (obj->material->kd / std::numbers::pi_v<float>) + (obj->material->ks * ((obj->material->shininess + 2.0F) / (2.0F * std::numbers::pi_v<float>)) * std::pow(cosphi, obj->material->shininess));
        RGB bsdf = (hemisphere_sample_obj->material->kd / std::numbers::pi_v<float>) + (hemisphere_sample_obj->material->ks * ((hemisphere_sample_obj->material->shininess + 2.0F) / (2.0F * std::numbers::pi_v<float>)) * std::pow(cosphi, hemisphere_sample_obj->material->shininess));
		path_throughput *= bsdf * std::abs((hemisphere_sample_shading_normal.dot(hemisphere_sample.dir)) / hemisphere_sample.pdf);

        // Recurse

        if (counter.GetDepth() < cs.recursion_depth)
        {
            // This is a hack for reflection fuzziness
            //Vector3f offset_reflection = (incident_reflection + Vector3f::Random() * (1.0F - obj->material->shininess)).normalized();

            // Cast the reflection ray and handle the hit
            RTCRayHit refl_ray = CastRay(incident_hit_worldspace, hemisphere_sample_reflection, std::numeric_limits<float>::infinity(), context);
            if (refl_ray.hit.geomID != RTC_INVALID_GEOMETRY_ID)
                indirect += HandleHit(refl_ray, context, path_throughput);
        }

		//// Recurse
		//if (counter.GetDepth() < cs.recursion_depth)
		//{
        //    RTCRayHit refl_ray = CastRay(incident_hit_worldspace, hemisphere_sample_reflection, std::numeric_limits<float>::infinity(), context);
		//	//indirect = HandleHit(refl_ray, context, path_throughput);
		//	indirect = HandleHit(refl_ray, context, WHITE);
		//}
	}




    return (direct_sample + indirect) / static_cast<float>(counter.GetDepth());
    //return returned_pixel_colour_value / static_cast<float>(counter.GetDepth());
}
*/

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
            {
                RGB col = BLACK;
                for (size_t i = 0; i < cs.direct_samples; i++)
                    col += (PerformSample(ray, context, 0) / static_cast<float>(cs.direct_samples));

                DrawColourToCanvas(pixel_ref, col);
            }
                
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

    const ConfigSingleton& cs = ConfigSingleton::GetInstance();
    std::cout << "Rendering film with " << threads             << " threads"          << std::endl;
    std::cout << "Rendering film with " << cs.direct_samples   << " direct samples"   << std::endl;
    std::cout << "Rendering film with " << cs.indirect_samples << " indirect samples" << std::endl;
    std::cout << "Rendering film with " << cs.recursion_depth  << " recursion depth"  << std::endl;
 
    ThreadPool pool(threads);               // Create a thread pool    
    std::vector<std::future<void>> futures; // Create a vector of futures    
    futures.reserve(film.canvases.size());  // Reserve space for the futures

    for (auto& canvas : film.canvases) // Enqueue the task for each canvas
        futures.emplace_back(pool.enqueue(RenderCanvas, std::ref(canvas), std::ref(camera)));
}
}