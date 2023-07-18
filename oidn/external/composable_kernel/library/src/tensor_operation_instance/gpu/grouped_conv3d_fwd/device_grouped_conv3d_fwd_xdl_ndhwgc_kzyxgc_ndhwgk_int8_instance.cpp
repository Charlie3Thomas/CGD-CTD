// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2022, Advanced Micro Devices, Inc. All rights reserved.

#include <cstdlib>

#include "ck/ck.hpp"
#include "ck/tensor_operation/gpu/device/tensor_layout.hpp"
#include "ck/tensor_operation/gpu/device/impl/device_grouped_conv_fwd_multiple_d_xdl_cshuffle.hpp"
#include "ck/tensor_operation/gpu/device/convolution_forward_specialization.hpp"
#include "ck/tensor_operation/gpu/device/gemm_specialization.hpp"
#include "ck/tensor_operation/gpu/element/element_wise_operation.hpp"

#include "ck/library/tensor_operation_instance/add_device_operation_instance.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace instance {

using Empty_Tuple = ck::Tuple<>;

template <ck::index_t... Is>
using S = ck::Sequence<Is...>;

using NDHWGC = ck::tensor_layout::convolution::NDHWGC;
using KZYXGC = ck::tensor_layout::convolution::KZYXGC;
using NDHWGK = ck::tensor_layout::convolution::NDHWGK;

using PassThrough = ck::tensor_operation::element_wise::PassThrough;

static constexpr auto ConvFwdDefault =
    ck::tensor_operation::device::ConvolutionForwardSpecialization::Default;

static constexpr auto ConvFwd1x1P0 =
    ck::tensor_operation::device::ConvolutionForwardSpecialization::Filter1x1Pad0;

static constexpr auto ConvFwd1x1S1P0 =
    ck::tensor_operation::device::ConvolutionForwardSpecialization::Filter1x1Stride1Pad0;

static constexpr auto GemmMNKPadding = ck::tensor_operation::device::GemmSpecialization::MNKPadding;

// in[g, n, di, hi, wi, c] * wei[g, k, z, y, x, c] = out[g, n, do, ho, wo, k]
using device_grouped_conv3d_fwd_xdl_ndhwgc_kzyxgc_ndhwgk_int8_instances = std::tuple<
    // clang-format off
        // Default
        //########################################|  NumDim|      A|      B|          Ds|      E|  AData|  BData| AccData| CShuffle|          Ds|  EData|           A|           B|         CDE|    ConvForward|           GEMM| NumGemmK| Block|  MPer|  NPer|  KPer| AK1| BK1| MPer| NPer| MXdl| NXdl|  ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockLds|  BBlockTransfer| BBlockTransfer| BBlockTransfer| BlockTransfer| BBlockTransfer| BBlockTransfer| BBlockLds|    CShuffle|    CShuffle| CBlockTransferClusterLengths|  CBlockTransfer|
        //########################################| Spatial| Layout| Layout|      Layout| Layout|   Type|   Type|    Type| DataType|    DataType|   Type| Elementwise| Elementwise| Elementwise| Specialization| Specialization| Prefetch|  Size| Block| Block| Block|    |    |  XDL|  XDL|  Per|  Per|   ThreadCluster|  ThreadCluster| SrcAccessOrder|   SrcVectorDim|      SrcScalar|      DstScalar| AddExtraM|   ThreadCluster|  ThreadCluster| SrcAccessOrder|  SrcVectorDim|      SrcScalar|      DstScalar| AddExtraN| MXdlPerWave| NXdlPerWave|         _MBlock_MWaveMPerXdl| ScalarPerVector|
        //########################################|        |       |       |            |       |       |       |        |         |            |       |   Operation|   Operation|   Operation|               |               |    Stage|      |      |      |      |    |    |     |     | Wave| Wave| Lengths_K0_M_K1|   ArrangeOrder|               |               |      PerVector|   PerVector_K1|          | Lengths_K0_N_K1|   ArrangeOrder|               |              |      PerVector|   PerVector_K1|          |  PerShuffle|  PerShuffle|         _NBlock_NWaveNPerXdl|   _NWaveNPerXdl|
        //########################################|        |       |       |            |       |       |       |        |         |            |       |            |            |            |               |               |         |      |      |      |      |    |    |     |     |     |     |                |               |               |               |               |               |          |                |               |               |              |               |               |          |            |            |                             |                |
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   256,   256,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   256,   128,   256,    32,   8,   8,   32,   32,    2,    4,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   128,   128,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   256,   128,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   128,   128,    64,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   128,    64,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,    64,    64,    64,    32,   8,   8,   32,   32,    2,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   256,   128,    64,    32,   8,   8,   32,   32,    2,    1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   256,    64,   128,    32,   8,   8,   32,   32,    1,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   128,   128,    32,    32,   8,   8,   32,   32,    2,    1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,   128,    32,   128,    32,   8,   8,   32,   32,    1,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,    64,    64,    32,    32,   8,   8,   32,   32,    2,    1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwdDefault, GemmMNKPadding,        1,    64,    32,    64,    32,   8,   8,   32,   32,    1,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,

        // Filter1x1Pad0
        //########################################|  NumDim|      A|      B|          Ds|      E|  AData|  BData| AccData| CShuffle|          Ds|  EData|           A|           B|         CDE|    ConvForward|           GEMM| NumGemmK| Block|  MPer|  NPer|  KPer| AK1| BK1| MPer| NPer| MXdl| NXdl|  ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockLds|  BBlockTransfer| BBlockTransfer| BBlockTransfer| BlockTransfer| BBlockTransfer| BBlockTransfer| BBlockLds|    CShuffle|    CShuffle| CBlockTransferClusterLengths|  CBlockTransfer|
        //########################################| Spatial| Layout| Layout|      Layout| Layout|   Type|   Type|    Type| DataType|    DataType|   Type| Elementwise| Elementwise| Elementwise| Specialization| Specialization| Prefetch|  Size| Block| Block| Block|    |    |  XDL|  XDL|  Per|  Per|   ThreadCluster|  ThreadCluster| SrcAccessOrder|   SrcVectorDim|      SrcScalar|      DstScalar| AddExtraM|   ThreadCluster|  ThreadCluster| SrcAccessOrder|  SrcVectorDim|      SrcScalar|      DstScalar| AddExtraN| MXdlPerWave| NXdlPerWave|         _MBlock_MWaveMPerXdl| ScalarPerVector|
        //########################################|        |       |       |            |       |       |       |        |         |            |       |   Operation|   Operation|   Operation|               |               |    Stage|      |      |      |      |    |    |     |     | Wave| Wave| Lengths_K0_M_K1|   ArrangeOrder|               |               |      PerVector|   PerVector_K1|          | Lengths_K0_N_K1|   ArrangeOrder|               |              |      PerVector|   PerVector_K1|          |  PerShuffle|  PerShuffle|         _NBlock_NWaveNPerXdl|   _NWaveNPerXdl|
        //########################################|        |       |       |            |       |       |       |        |         |            |       |            |            |            |               |               |         |      |      |      |      |    |    |     |     |     |     |                |               |               |               |               |               |          |                |               |               |              |               |               |          |            |            |                             |                |
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   256,   256,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   256,   128,   256,    32,   8,   8,   32,   32,    2,    4,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   128,   128,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   256,   128,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   128,   128,    64,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   128,    64,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,    64,    64,    64,    32,   8,   8,   32,   32,    2,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   256,   128,    64,    32,   8,   8,   32,   32,    2,    1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   256,    64,   128,    32,   8,   8,   32,   32,    1,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   128,   128,    32,    32,   8,   8,   32,   32,    2,    1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,   128,    32,   128,    32,   8,   8,   32,   32,    1,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,    64,    64,    32,    32,   8,   8,   32,   32,    2,    1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough,   ConvFwd1x1P0, GemmMNKPadding,        1,    64,    32,    64,    32,   8,   8,   32,   32,    1,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,
                                                                                                                                                                                                                
        // Filter1x1Stride1Pad0                                                                                                                                                                                 
        //########################################|  NumDim|      A|      B|          Ds|      E|  AData|  BData| AccData| CShuffle|          Ds|  EData|           A|           B|         CDE|    ConvForward|           GEMM| NumGemmK| Block|  MPer|  NPer|  KPer| AK1| BK1| MPer| NPer| MXdl| NXdl|  ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockLds|  BBlockTransfer| BBlockTransfer| BBlockTransfer| BlockTransfer| BBlockTransfer| BBlockTransfer| BBlockLds|    CShuffle|    CShuffle| CBlockTransferClusterLengths|  CBlockTransfer|
        //########################################| Spatial| Layout| Layout|      Layout| Layout|   Type|   Type|    Type| DataType|    DataType|   Type| Elementwise| Elementwise| Elementwise| Specialization| Specialization| Prefetch|  Size| Block| Block| Block|    |    |  XDL|  XDL|  Per|  Per|   ThreadCluster|  ThreadCluster| SrcAccessOrder|   SrcVectorDim|      SrcScalar|      DstScalar| AddExtraM|   ThreadCluster|  ThreadCluster| SrcAccessOrder|  SrcVectorDim|      SrcScalar|      DstScalar| AddExtraN| MXdlPerWave| NXdlPerWave|         _MBlock_MWaveMPerXdl| ScalarPerVector|
        //########################################|        |       |       |            |       |       |       |        |         |            |       |   Operation|   Operation|   Operation|               |               |    Stage|      |      |      |      |    |    |     |     | Wave| Wave| Lengths_K0_M_K1|   ArrangeOrder|               |               |      PerVector|   PerVector_K1|          | Lengths_K0_N_K1|   ArrangeOrder|               |              |      PerVector|   PerVector_K1|          |  PerShuffle|  PerShuffle|         _NBlock_NWaveNPerXdl|   _NWaveNPerXdl|
        //########################################|        |       |       |            |       |       |       |        |         |            |       |            |            |            |               |               |         |      |      |      |      |    |    |     |     |     |     |                |               |               |               |               |               |          |                |               |               |              |               |               |          |            |            |                             |                |
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   256,   256,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   256,   128,   256,    32,   8,   8,   32,   32,    2,    4,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   128,   128,   128,    32,   8,   8,   32,   32,    4,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   256,   128,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   128,   128,    64,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   128,    64,   128,    32,   8,   8,   32,   32,    2,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,    64,    64,    64,    32,   8,   8,   32,   32,    2,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   256,   128,    64,    32,   8,   8,   32,   32,    2,    1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   256,    64,   128,    32,   8,   8,   32,   32,    1,    2,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 64, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   128,   128,    32,    32,   8,   8,   32,   32,    2,    1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 32, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,   128,    32,   128,    32,   8,   8,   32,   32,    1,    2,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 32, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 8>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,    64,    64,    32,    32,   8,   8,   32,   32,    2,    1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>,
        DeviceGroupedConvFwdMultipleD_Xdl_CShuffle<       3, NDHWGC, KZYXGC, Empty_Tuple, NDHWGK, int8_t, int8_t, int32_t,   int8_t, Empty_Tuple, int8_t, PassThrough, PassThrough, PassThrough, ConvFwd1x1S1P0, GemmMNKPadding,        1,    64,    32,    64,    32,   8,   8,   32,   32,    1,    2,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,              2,              8,              8,         1,     S<4, 16, 1>,     S<1, 0, 2>,     S<1, 0, 2>,             2,              8,              8,         1,           1,           1,               S<1, 16, 1, 4>,               8>
    // clang-format on
    >;

void add_device_grouped_conv3d_fwd_xdl_ndhwgc_kzyxgc_ndhwgk_int8_instances(
    std::vector<std::unique_ptr<DeviceGroupedConvFwdMultipleD<3,
                                                              NDHWGC,
                                                              KZYXGC,
                                                              Empty_Tuple,
                                                              NDHWGK,
                                                              int8_t,
                                                              int8_t,
                                                              Empty_Tuple,
                                                              int8_t,
                                                              PassThrough,
                                                              PassThrough,
                                                              PassThrough>>>& instances)
{
    add_device_operation_instances(
        instances, device_grouped_conv3d_fwd_xdl_ndhwgc_kzyxgc_ndhwgk_int8_instances{});
}

} // namespace instance
} // namespace device
} // namespace tensor_operation
} // namespace ck
