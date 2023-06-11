#include "bvh.hpp"
#include "embree/embreedevice.hpp"
#include "utils/timer.hpp"
#include <array>


namespace CT
{

/// @brief Computes the surface area of a bounding box
/// @param b 
/// @return 
float Area(const RTCBounds& b)
{
    return 2.0F * (b.upper_x - b.lower_x) * (b.upper_y - b.lower_y) * (b.upper_z - b.lower_z);
}

/// @brief Merges two bounding boxes
/// @param a 
/// @param b 
/// @return 
RTCBounds Merge(const RTCBounds& a, const RTCBounds& b)
{
    RTCBounds ret;
        ret.lower_x = std::min(a.lower_x, b.lower_x);
        ret.lower_y = std::min(a.lower_y, b.lower_y);
        ret.lower_z = std::min(a.lower_z, b.lower_z);

        ret.upper_x = std::max(a.upper_x, b.upper_x);
        ret.upper_y = std::max(a.upper_y, b.upper_y);
        ret.upper_z = std::max(a.upper_z, b.upper_z);

    return ret;
}

struct Node
{
    virtual float sah() = 0;
    virtual ~Node() = default;
};

struct InnerNode : public Node
{
    // AABB 
    std::array<RTCBounds, 2> bounds;

    // Node children
    std::array<Node*, 2> children;

    InnerNode()
    {
        bounds[0] = RTCBounds();
        bounds[1] = RTCBounds();
        children[0] = nullptr;
        children[1] = nullptr;
    }

    float sah() override
    {
        return 1.0F+ (Area(bounds[0]) * children[0]->sah() + Area(bounds[1])*children[1]->sah()) / Area(Merge(bounds[0], bounds[1]));
    }

    static void* Create(RTCThreadLocalAllocator alloc, unsigned int num_children, void* usrptr)
    {
        assert(num_children == 2);
        void* ptr = rtcThreadLocalAlloc(alloc, sizeof(InnerNode), 16);
        return static_cast<void*> (new (ptr) InnerNode());
    }

    static void SetChildren(void* nodeptr, void** childptr, unsigned int num_children, void* usrptr)
    {
        assert(num_children == 2);
        for(size_t i = 0; i < 2; i++)
        {
            static_cast<InnerNode*>(nodeptr)->children[i] = static_cast<Node*>(childptr[i]);
        }            
    }

    static void SetBounds(void* nodeptr, const RTCBounds** bounds, unsigned int num_children, void* usrptr)
    {
        assert(num_children == 2);
        for(size_t i = 0; i < 2; i++)
        {
            static_cast<InnerNode*>(nodeptr)->bounds[i] = *(const RTCBounds*) bounds[i];
        }   
    }
};

struct LeafNode : public Node
{
    unsigned id;
    RTCBounds bounds;

    LeafNode(unsigned id, const RTCBounds& bounds) : id(id), bounds(bounds) {}

    float sah() override
    {
        return 1.0F;
    }

    static void* Create(RTCThreadLocalAllocator alloc, const RTCBuildPrimitive* prims, size_t num_prims, void* user_ptr)
    {
        assert(num_prims == 1);
        void* ptr = rtcThreadLocalAlloc(alloc, sizeof(LeafNode), 16);
        return static_cast<void*> (new (ptr) LeafNode(prims->primID, reinterpret_cast<const RTCBounds&>(*prims)));
    }
};


void SplitPrimitive(const RTCBuildPrimitive* prim, unsigned int dim, float pos, RTCBounds* lprim, RTCBounds* rprim, void* userPtr)
{
    assert(dim < 3);
    assert(prim->geomID == 0);
    *lprim = reinterpret_cast<const RTCBounds&>(*prim);
    *rprim = reinterpret_cast<const RTCBounds&>(*prim);
    (&lprim->upper_x)[dim] = pos;
    (&rprim->lower_x)[dim] = pos;
}

void BuildProgress()
{
    throw std::runtime_error("BuildProgress not implemented");
}

void ErrCallback(void* user_ptr, RTCError code, const char* str)
{
    std::cout << "EMBREE ERROR CODE " << code << ": " << str << std::endl;
}

void BuildBVH(RTCBuildQuality quality, std::vector<RTCBuildPrimitive>& prims_i, char* cfg, size_t extra_space)
{
    rtcSetDeviceMemoryMonitorFunction(EmbreeSingleton::GetInstance().device, nullptr, nullptr);

    RTCBVH bvh = rtcNewBVH(EmbreeSingleton::GetInstance().device);
    assert(bvh != nullptr);

    std::vector<RTCBuildPrimitive> prims;
    prims.reserve(prims_i.size() + extra_space);
    prims.resize(prims_i.size());

    // Settings for bvh build
    RTCBuildArguments arguments = rtcDefaultBuildArguments();
        arguments.byteSize               = sizeof(arguments);
        arguments.buildFlags             = RTC_BUILD_FLAG_NONE; // Build flag DYNAMIC?
        arguments.buildQuality           = quality;
        arguments.maxBranchingFactor     = 2;
        arguments.maxDepth               = 1024;
        arguments.sahBlockSize           = 1;
        arguments.minLeafSize            = 1;
        arguments.maxLeafSize            = 1;
        arguments.traversalCost          = 1.0F;
        arguments.intersectionCost       = 1.0F;
        arguments.bvh                    = bvh;
        arguments.primitives             = prims.data();
        arguments.primitiveCount         = prims.size();
        arguments.primitiveArrayCapacity = prims.capacity();
        arguments.createNode             = InnerNode::Create;
        arguments.setNodeChildren        = InnerNode::SetChildren;
        arguments.setNodeBounds          = InnerNode::SetBounds;
        arguments.createLeaf             = LeafNode::Create;
        arguments.splitPrimitive         = SplitPrimitive;
        arguments.buildProgress          = nullptr;        
        arguments.userPtr                = nullptr;

    Timer t = Timer("BVH built in ");

    for (size_t i = 0; i < 10; i++) //TODO : Why does the example limit this to 10?
    {
        for (size_t j = 0; j < prims.size(); j++) prims[j] = prims_i[j];
        rtcBuildBVH(&arguments);
    }

    std::cout << "Built for " << prims.size() << " primitives" << std::endl;

    // Check for error in build arguments
    RTCErrorFunction error_function = ErrCallback;
    rtcSetDeviceErrorFunction(EmbreeSingleton::GetInstance().device, error_function, nullptr);

    rtcReleaseBVH(bvh);
}
}