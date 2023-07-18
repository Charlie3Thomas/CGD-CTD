// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2022, Advanced Micro Devices, Inc. All rights reserved.

#include "ck/ck.hpp"
#include "ck/utility/tuple.hpp"

#include "ck/tensor_operation/gpu/element/element_wise_operation.hpp"
#include "ck/tensor_operation/gpu/device/impl/device_elementwise_impl.hpp"
#include "ck/library/tensor_operation_instance/add_device_operation_instance.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace instance {

using F32 = float;

using Normalize = ck::tensor_operation::element_wise::NormalizeInInfer;

// clang-format off
template <index_t Rank>
using device_batchnorm_infer_f32_instances =
     std::tuple <
        // Tuple<XDataType, MeanDataType, VarDataType, ScaleDataType, BiasDataType>, Tuple<YDataType>, NormalizeOp, Rank, MPerThread, Sequence<XVectorSize, MeanDataType, VarDataType, ScaleVectorSize, BiasVectorSize>, Sequence<YVectorSize> 
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 1, Sequence<1, 1, 1, 1, 1>, Sequence<1> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 2, Sequence<1, 1, 1, 1, 1>, Sequence<1> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 2, Sequence<2, 1, 1, 1, 1>, Sequence<2> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 2, Sequence<1, 2, 2, 2, 2>, Sequence<1> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 2, Sequence<2, 2, 2, 2, 2>, Sequence<2> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<1, 1, 1, 1, 1>, Sequence<1> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<2, 1, 1, 1, 1>, Sequence<2> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<1, 2, 2, 2, 2>, Sequence<1> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<2, 2, 2, 2, 2>, Sequence<2> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<4, 1, 1, 1, 1>, Sequence<4> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<1, 4, 4, 4, 4>, Sequence<1> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<4, 2, 2, 2, 2>, Sequence<4> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<2, 4, 4, 4, 4>, Sequence<2> >,
        DeviceElementwiseImpl<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, Rank, 4, Sequence<4, 4, 4, 4, 4>, Sequence<4> >
     >;
// clang-format on

void add_device_batchnorm_infer_rank_4_f32_instances(
    std::vector<std::unique_ptr<
        DeviceElementwise<Tuple<F32, F32, F32, F32, F32>, Tuple<F32>, Normalize, 4>>>& instances)
{
    add_device_operation_instances(instances, device_batchnorm_infer_f32_instances<4>{});
}

} // namespace instance
} // namespace device
} // namespace tensor_operation
} // namespace ck
