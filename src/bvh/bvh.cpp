#include "bvh.hpp"
#include "embree/embreedevice.hpp"

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

    //TODO: Implement commented section

    //  arguments.createNode             = InnerNode::create;
    //  arguments.setNodeChildren        = InnerNode::setChildren;
    //  arguments.setNodeBounds          = InnerNode::setBounds;
    //  arguments.createLeaf             = LeafNode::create;
    //  arguments.splitPrimitive         = SplitPrimitive();
    //  arguments.buildProgress          = BuildProgress();
    arguments.userPtr = nullptr;

    RTCPointQuery query;

    // Check for error in build arguments
    RTCErrorFunction error_function = ErrCallback;
    rtcSetDeviceErrorFunction(EmbreeSingleton::GetInstance().device, error_function, nullptr);

    rtcReleaseBVH(bvh);
}