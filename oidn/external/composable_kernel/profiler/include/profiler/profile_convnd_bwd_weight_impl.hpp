#pragma once

#include "ck/ck.hpp"
#include "ck/tensor_operation/gpu/device/tensor_layout.hpp"
#include "ck/tensor_operation/gpu/device/device_conv_backward_weight.hpp"
#include "ck/tensor_operation/gpu/element/element_wise_operation.hpp"

#include "ck/library/utility/check_err.hpp"
#include "ck/library/utility/conv_util.hpp"
#include "ck/library/host_tensor/device_memory.hpp"
#include "ck/library/host_tensor/host_tensor.hpp"
#include "ck/library/host_tensor/host_tensor_generator.hpp"
#include "ck/library/reference_tensor_operation/cpu/reference_conv_backward_weight.hpp"

using F16  = ck::half_t;
using F32  = float;
using BF16 = ck::bhalf_t;

namespace ck {
namespace tensor_operation {
namespace device {
namespace instance {

using DeviceConvndBwdWeightNoOpPtr =
    DeviceConvBwdWeightPtr<ck::tensor_operation::element_wise::PassThrough,
                           ck::tensor_operation::element_wise::PassThrough,
                           ck::tensor_operation::element_wise::PassThrough>;

void add_device_conv1d_bwd_weight_xdl_nwc_kxc_nwk_f32_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);
void add_device_convnd_bwd_weight_xdl_nhwc_kyxc_nhwk_f32_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);
void add_device_conv3d_bwd_weight_xdl_ndhwc_kzyxc_ndhwk_f32_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);

void add_device_conv1d_bwd_weight_xdl_nwc_kxc_nwk_f16_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);
void add_device_convnd_bwd_weight_xdl_nhwc_kyxc_nhwk_f16_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);
void add_device_conv3d_bwd_weight_xdl_ndhwc_kzyxc_ndhwk_f16_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);

void add_device_conv1d_bwd_weight_xdl_nwc_kxc_nwk_bf16_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);
void add_device_conv2d_bwd_weight_xdl_nhwc_kyxc_nhwk_bf16_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);
void add_device_conv3d_bwd_weight_xdl_ndhwc_kzyxc_ndhwk_bf16_instances(
    std::vector<DeviceConvndBwdWeightNoOpPtr>&);

} // namespace instance
} // namespace device
} // namespace tensor_operation
} // namespace ck

namespace ck {
namespace profiler {

using DeviceConvndBwdWeightNoOpPtr =
    ck::tensor_operation::device::instance::DeviceConvndBwdWeightNoOpPtr;

template <typename InLayout>
HostTensorDescriptor get_input_host_tensor_descriptor(const std::vector<std::size_t>& dims,
                                                      int num_dim_spatial = 2)
{
    namespace tl = ck::tensor_layout::convolution;

    switch(num_dim_spatial)
    {
    case 3: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, InLayout{});
    }
    case 2: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, InLayout{});
    }
    case 1: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, InLayout{});
    }
    default: {
        throw std::runtime_error("Unsupported number of spatial dimensions provided!");
    }
    }
}

template <typename WeiLayout>
HostTensorDescriptor get_filters_host_tensor_descriptor(const std::vector<std::size_t>& dims,
                                                        int num_dim_spatial = 2)
{
    namespace tl = ck::tensor_layout::convolution;

    switch(num_dim_spatial)
    {
    case 3: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, WeiLayout{});
    }
    case 2: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, WeiLayout{});
    }
    case 1: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, WeiLayout{});
    }
    default: {
        throw std::runtime_error("Unsupported number of spatial dimensions provided!");
    }
    }
}

template <typename OutLayout>
HostTensorDescriptor get_output_host_ensor_descriptor(const std::vector<std::size_t>& dims,
                                                      int num_dim_spatial = 2)
{
    namespace tl = ck::tensor_layout::convolution;

    switch(num_dim_spatial)
    {
    case 3: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, OutLayout{});
    }
    case 2: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, OutLayout{});
    }
    case 1: {
        return ck::utils::conv::get_host_tensor_descriptor(dims, OutLayout{});
    }
    default: {
        throw std::runtime_error("Unsupported number of spatial dimensions provided!");
    }
    }
}

template <typename InDataType, typename WeiDataType, typename OutDataType>
void get_device_conv_bwd_weight_op_ptr(
    InDataType, WeiDataType, OutDataType, std::vector<DeviceConvndBwdWeightNoOpPtr>&, int)
{
    std::cout << "can not find device conv bwd weight" << std::endl;
    exit(1);
}

template <>
void get_device_conv_bwd_weight_op_ptr(
    F32, F32, F32, std::vector<DeviceConvndBwdWeightNoOpPtr>& conv_ptrs, int num_dim_spatial)
{
    switch(num_dim_spatial)
    {
    case 1:
        ck::tensor_operation::device::instance::
            add_device_conv1d_bwd_weight_xdl_nwc_kxc_nwk_f32_instances(conv_ptrs);
        break;
    case 2:
        ck::tensor_operation::device::instance::
            add_device_convnd_bwd_weight_xdl_nhwc_kyxc_nhwk_f32_instances(conv_ptrs);
        break;
    case 3:
        ck::tensor_operation::device::instance::
            add_device_conv3d_bwd_weight_xdl_ndhwc_kzyxc_ndhwk_f32_instances(conv_ptrs);
        break;
    default: break;
    }
}

template <>
void get_device_conv_bwd_weight_op_ptr(
    F16, F16, F16, std::vector<DeviceConvndBwdWeightNoOpPtr>& conv_ptrs, int num_dim_spatial)
{
    switch(num_dim_spatial)
    {
    case 1:
        ck::tensor_operation::device::instance::
            add_device_conv1d_bwd_weight_xdl_nwc_kxc_nwk_f16_instances(conv_ptrs);
        break;
    case 2:
        ck::tensor_operation::device::instance::
            add_device_convnd_bwd_weight_xdl_nhwc_kyxc_nhwk_f16_instances(conv_ptrs);
        break;
    case 3:
        ck::tensor_operation::device::instance::
            add_device_conv3d_bwd_weight_xdl_ndhwc_kzyxc_ndhwk_f16_instances(conv_ptrs);
        break;
    default: break;
    }
}

template <>
void get_device_conv_bwd_weight_op_ptr(
    BF16, BF16, BF16, std::vector<DeviceConvndBwdWeightNoOpPtr>& conv_ptrs, int num_dim_spatial)
{
    switch(num_dim_spatial)
    {
    case 1:
        ck::tensor_operation::device::instance::
            add_device_conv1d_bwd_weight_xdl_nwc_kxc_nwk_bf16_instances(conv_ptrs);
        break;
    case 2:
        ck::tensor_operation::device::instance::
            add_device_conv2d_bwd_weight_xdl_nhwc_kyxc_nhwk_bf16_instances(conv_ptrs);
        break;
    case 3:
        ck::tensor_operation::device::instance::
            add_device_conv3d_bwd_weight_xdl_ndhwc_kzyxc_ndhwk_bf16_instances(conv_ptrs);
        break;
    default: break;
    }
}

template <typename DataType>
void show_data_nhwc_layout(Tensor<DataType>& nhwc)
{
    std::cout << "[";
    for(int n = 0; n < ck::type_convert<int>(nhwc.mDesc.GetLengths()[0]); n++)
    {
        std::cout << "[";
        for(int hi = 0; hi < ck::type_convert<int>(nhwc.mDesc.GetLengths()[2]); hi++)
        {
            std::cout << "[";
            for(int wi = 0; wi < ck::type_convert<int>(nhwc.mDesc.GetLengths()[3]); wi++)
            {
                std::cout << "[";
                for(int c = 0; c < ck::type_convert<int>(nhwc.mDesc.GetLengths()[1]); c++)
                {
                    std::cout << static_cast<float>(nhwc(n, c, hi, wi)) << "  ";
                }
                std::cout << "]";
            }
            std::cout << "]";
        }
        std::cout << "]";
    }
    std::cout << "]";
}

template <int NDimSpatial,
          typename InDataType,
          typename WeiDataType,
          typename OutDataType,
          typename InLayout,
          typename WeiLayout,
          typename OutLayout>
bool profile_convnd_bwd_weight_impl(int do_verification,
                                    int init_method,
                                    bool do_log,
                                    bool time_kernel,
                                    ck::index_t N,
                                    ck::index_t K,
                                    ck::index_t C,
                                    std::vector<ck::index_t> input_spatial_lengths,
                                    std::vector<ck::index_t> filter_spatial_lengths,
                                    std::vector<ck::index_t> output_spatial_lengths,
                                    std::vector<ck::index_t> conv_filter_strides,
                                    std::vector<ck::index_t> conv_filter_dilations,
                                    std::vector<ck::index_t> input_left_pads,
                                    std::vector<ck::index_t> input_right_pads,
                                    ck::index_t split_k)
{
    using InElementOp  = ck::tensor_operation::element_wise::PassThrough;
    using WeiElementOp = ck::tensor_operation::element_wise::PassThrough;
    using OutElementOp = ck::tensor_operation::element_wise::PassThrough;

    const auto in_element_op  = InElementOp{};
    const auto wei_element_op = WeiElementOp{};
    const auto out_element_op = OutElementOp{};

    std::vector<std::size_t> input_dims{static_cast<std::size_t>(N), static_cast<std::size_t>(C)};
    input_dims.insert(
        std::end(input_dims), std::begin(input_spatial_lengths), std::end(input_spatial_lengths));

    std::vector<std::size_t> filter_dims{static_cast<std::size_t>(K), static_cast<std::size_t>(C)};
    filter_dims.insert(std::end(filter_dims),
                       std::begin(filter_spatial_lengths),
                       std::end(filter_spatial_lengths));

    std::vector<std::size_t> output_dims{static_cast<std::size_t>(N), static_cast<std::size_t>(K)};
    output_dims.insert(std::end(output_dims),
                       std::begin(output_spatial_lengths),
                       std::end(output_spatial_lengths));

    Tensor<InDataType> input(get_input_host_tensor_descriptor<InLayout>(input_dims, NDimSpatial));
    Tensor<WeiDataType> weights_host_result(
        get_filters_host_tensor_descriptor<WeiLayout>(filter_dims, NDimSpatial));
    Tensor<WeiDataType> weights_device_result(
        get_filters_host_tensor_descriptor<WeiLayout>(filter_dims, NDimSpatial));
    Tensor<OutDataType> output(
        get_output_host_ensor_descriptor<OutLayout>(output_dims, NDimSpatial));

    std::cout << "input: " << input.mDesc << std::endl;
    std::cout << "weights: " << weights_host_result.mDesc << std::endl;
    std::cout << "output: " << output.mDesc << std::endl;

    switch(init_method)
    {
    case 0: break;
    case 1:
        input.GenerateTensorValue(GeneratorTensor_2<OutDataType>{-2, 2});
        output.GenerateTensorValue(GeneratorTensor_2<WeiDataType>{-2, 2});
        break;
    default:
        input.GenerateTensorValue(GeneratorTensor_1<OutDataType>{1});
        output.GenerateTensorValue(GeneratorTensor_1<WeiDataType>{1});
    }

    DeviceMem in_device_buf(sizeof(InDataType) * input.mDesc.GetElementSpace());
    DeviceMem wei_device_buf(sizeof(WeiDataType) * weights_device_result.mDesc.GetElementSpace());
    DeviceMem out_device_buf(sizeof(OutDataType) * output.mDesc.GetElementSpace());

    in_device_buf.ToDevice(input.mData.data());
    out_device_buf.ToDevice(output.mData.data());

    // reset input to zero
    wei_device_buf.SetZero();

    if(do_verification)
    {
        auto RunReference = [&](auto& ref_conv) {
            auto ref_invoker = ref_conv.MakeInvoker();

            auto ref_argument = ref_conv.MakeArgument(input,
                                                      weights_host_result,
                                                      output,
                                                      conv_filter_strides,
                                                      conv_filter_dilations,
                                                      input_left_pads,
                                                      input_right_pads,
                                                      InElementOp{},
                                                      WeiElementOp{},
                                                      OutElementOp{});
            ref_invoker.Run(ref_argument);
        };

        auto ref_conv = ck::tensor_operation::host::ReferenceConvBwdWeight<InDataType,
                                                                           WeiDataType,
                                                                           OutDataType,
                                                                           InElementOp,
                                                                           WeiElementOp,
                                                                           OutElementOp,
                                                                           NDimSpatial>();
        RunReference(ref_conv);
    }

    // add device Conv instances
    std::vector<DeviceConvndBwdWeightNoOpPtr> conv_ptrs;
    get_device_conv_bwd_weight_op_ptr(
        InDataType{}, WeiDataType{}, OutDataType{}, conv_ptrs, NDimSpatial);

    if(conv_ptrs.size() <= 0)
    {
        throw std::runtime_error("wrong! no device Conv instance found");
    }

    std::string best_conv_name;
    float best_ave_time   = 0;
    float best_tflops     = 0;
    float best_gb_per_sec = 0;

    // profile device Conv instances
    bool success = true;
    for(auto& conv_ptr : conv_ptrs)
    {
        // using atomic, so need to reset input, setzero is done in invoker
        // if(split_k > 1)
        //{
        //    wei_device_buf.SetZero();
        //}

        auto argument_ptr = conv_ptr->MakeArgumentPointer(
            static_cast<InDataType*>(in_device_buf.GetDeviceBuffer()),
            static_cast<WeiDataType*>(wei_device_buf.GetDeviceBuffer()),
            static_cast<OutDataType*>(out_device_buf.GetDeviceBuffer()),
            N,
            K,
            C,
            input_spatial_lengths,
            filter_spatial_lengths,
            output_spatial_lengths,
            conv_filter_strides,
            conv_filter_dilations,
            input_left_pads,
            input_right_pads,
            in_element_op,
            wei_element_op,
            out_element_op,
            split_k);

        if(!conv_ptr->IsSupportedArgument(argument_ptr.get()))
        {
            std::cout << "wrong! device_conv with the specified compilation parameters does "
                         "not support this Conv problem"
                      << std::endl;
            continue;
        }

        auto invoker_ptr      = conv_ptr->MakeInvokerPointer();
        std::string conv_name = conv_ptr->GetTypeString();
        float ave_time        = 0;

        if(std::is_same<InDataType, ck::bhalf_t>::value && split_k > 1)
        {
            // alloc work space
            size_t bwd_weight_workspace_size = conv_ptr->GetWorkSpaceSize(argument_ptr.get());
            if(bwd_weight_workspace_size <= 0)
            {
                printf("wrong work space size\n");
                exit(1);
            }
            DeviceMem wei_work_space_device_buf(bwd_weight_workspace_size);
            wei_work_space_device_buf.SetZero();
            conv_ptr->SetWorkSpacePointer(argument_ptr.get(),
                                          wei_work_space_device_buf.GetDeviceBuffer());
            ave_time = invoker_ptr->Run(argument_ptr.get(), StreamConfig{nullptr, time_kernel});
        }
        else
        {
            ave_time = invoker_ptr->Run(argument_ptr.get(), StreamConfig{nullptr, time_kernel});
        }

        std::size_t flop =
            ck::utils::conv::get_flops(N, C, K, filter_spatial_lengths, output_spatial_lengths);
        std::size_t num_btype = ck::utils::conv::get_btype<InDataType, WeiDataType, OutDataType>(
            N, C, K, input_spatial_lengths, filter_spatial_lengths, output_spatial_lengths);

        float tflops     = static_cast<float>(flop) / 1.E9 / ave_time;
        float gb_per_sec = num_btype / 1.E6 / ave_time;

        std::cout << "Perf: " << ave_time << " ms, " << tflops << " TFlops, " << gb_per_sec
                  << " GB/s" << std::endl;

        if(tflops > best_tflops)
        {
            best_conv_name  = conv_name;
            best_tflops     = tflops;
            best_ave_time   = ave_time;
            best_gb_per_sec = gb_per_sec;
        }

        if(do_verification)
        {
            wei_device_buf.FromDevice(weights_device_result.mData.data());

            success = ck::utils::check_err(weights_host_result, weights_device_result);

            if(success == false)
            {
                std::cout << "Fail Info: " << conv_ptr->GetTypeString() << std::endl;
            }
            else
            {
                std::cout << "Pass Info: " << conv_ptr->GetTypeString() << std::endl;
            }

            if(do_log)
            {
                std::cout << "in : ";
                show_data_nhwc_layout(output);
                std::cout << std::endl;

                std::cout << "wei: ";
                show_data_nhwc_layout(weights_host_result);
                std::cout << std::endl;

                std::cout << "out  : ";
                show_data_nhwc_layout(input);
                std::cout << std::endl;

                std::cout << "wei_device: ";
                show_data_nhwc_layout(weights_device_result);
                std::cout << std::endl;
            }
        }
    }

    std::cout << "Best Perf: " << best_ave_time << " ms, " << best_tflops << " TFlops, "
              << best_gb_per_sec << " GB/s, " << best_conv_name << std::endl;
    return success;
}

} // namespace profiler
} // namespace ck
