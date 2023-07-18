/***************************************************************************************************
 * Copyright (c) 2017 - 2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************************/
/*! \file
    \brief Tests for device-wide Implicit GEMM interface
*/

#include "../../common/cutlass_unit_test.h"
#include "cutlass/cutlass.h"


#include "cutlass/conv/kernel/default_conv2d_dgrad.h"
#include "cutlass/conv/device/implicit_gemm_convolution.h"

#include "conv2d_testbed.h"


#if defined(CUTLASS_ARCH_MMA_SM80_SUPPORTED)
    
////////////////////////////////////////////////////////////////////////////////

TEST(SM80_Device_Conv2d_Dgrad_Analytic_ImplicitGemm_f16nhwc_f16nhwc_f16nhwc_tensor_op_f16,
  128x128_64x3_64x64x64) {
 
  /// Conv operation element types for the Gemm equivalent (ImplicitGemm)
  using ElementA           = cutlass::half_t;
  using ElementB           = cutlass::half_t;
  using ElementC           = cutlass::half_t;
  using ElementAccumulator = cutlass::half_t;
  using ElementCompute     = cutlass::half_t;

  /// Device-level Conv2d instance
  using Conv2dDgradKernel = typename cutlass::conv::kernel::DefaultConv2dDgrad<
    ElementA, cutlass::layout::TensorNHWC,
    ElementB, cutlass::layout::TensorNHWC,
    ElementC, cutlass::layout::TensorNHWC,
    ElementAccumulator,
    cutlass::arch::OpClassTensorOp,
    cutlass::arch::Sm80,
    cutlass::gemm::GemmShape<128, 128, 64>,
    cutlass::gemm::GemmShape<64, 64, 64>,
    cutlass::gemm::GemmShape<16, 8, 16>,
    cutlass::epilogue::thread::LinearCombination<
      ElementC,
      128 / cutlass::sizeof_bits<ElementC>::value,
      ElementAccumulator,
      ElementCompute
    >,
    cutlass::gemm::threadblock::GemmIdentityThreadblockSwizzle<>,
    3,
    cutlass::arch::OpMultiplyAdd,
    cutlass::conv::IteratorAlgorithm::kAnalytic,
    cutlass::conv::StrideSupport::kUnity
  >::Kernel;

  using Conv2dDgrad = cutlass::conv::device::ImplicitGemmConvolution<Conv2dDgradKernel>;
  
  /// Run all unit test sizes with device-level Conv2d instance
  EXPECT_TRUE(test::conv::device::TestAllConv2d<Conv2dDgrad>());
}

////////////////////////////////////////////////////////////////////////////////

TEST(SM80_Device_Conv2d_Dgrad_Optimized_ImplicitGemm_f16nhwc_f16nhwc_f16nhwc_tensor_op_f16,
  128x128_64x3_64x64x64) {
 
  /// Conv operation element types for the Gemm equivalent (ImplicitGemm)
  using ElementA           = cutlass::half_t;
  using ElementB           = cutlass::half_t;
  using ElementC           = cutlass::half_t;
  using ElementAccumulator = cutlass::half_t;
  using ElementCompute     = cutlass::half_t;

  /// Device-level Conv2d instance
  using Conv2dDgradKernel = typename cutlass::conv::kernel::DefaultConv2dDgrad<
    ElementA, cutlass::layout::TensorNHWC,
    ElementB, cutlass::layout::TensorNHWC,
    ElementC, cutlass::layout::TensorNHWC,
    ElementAccumulator,
    cutlass::arch::OpClassTensorOp,
    cutlass::arch::Sm80,
    cutlass::gemm::GemmShape<128, 128, 64>,
    cutlass::gemm::GemmShape<64, 64, 64>,
    cutlass::gemm::GemmShape<16, 8, 16>,
    cutlass::epilogue::thread::LinearCombination<
      ElementC,
      128 / cutlass::sizeof_bits<ElementC>::value,
      ElementAccumulator,
      ElementCompute
    >,
    cutlass::gemm::threadblock::GemmIdentityThreadblockSwizzle<>,
    3,
    cutlass::arch::OpMultiplyAdd,
    cutlass::conv::IteratorAlgorithm::kOptimized,
    cutlass::conv::StrideSupport::kUnity
  >::Kernel;

  using Conv2dDgrad = cutlass::conv::device::ImplicitGemmConvolution<Conv2dDgradKernel>;
  
  /// Run all unit test sizes with device-level Conv2d instance
  EXPECT_TRUE(test::conv::device::TestAllConv2d<Conv2dDgrad>());
}

////////////////////////////////////////////////////////////////////////////////

TEST(SM80_Device_Conv2d_Dgrad_Analytic_ImplicitGemm_f16nhwc_f16nhwc_f16nhwc_tensor_op_f16_align4,
  128x128_64x3_64x64x64) {
 
  /// Conv operation element types for the Gemm equivalent (ImplicitGemm)
  using ElementA           = cutlass::half_t;
  using ElementB           = cutlass::half_t;
  using ElementC           = cutlass::half_t;
  using ElementAccumulator = cutlass::half_t;
  using ElementCompute     = cutlass::half_t;

  /// Device-level Conv2d instance
  using Conv2dDgradKernel = typename cutlass::conv::kernel::DefaultConv2dDgrad<
    ElementA, cutlass::layout::TensorNHWC,
    ElementB, cutlass::layout::TensorNHWC,
    ElementC, cutlass::layout::TensorNHWC,
    ElementAccumulator,
    cutlass::arch::OpClassTensorOp,
    cutlass::arch::Sm80,
    cutlass::gemm::GemmShape<128, 128, 64>,
    cutlass::gemm::GemmShape<64, 64, 64>,
    cutlass::gemm::GemmShape<16, 8, 16>,
    cutlass::epilogue::thread::LinearCombination<
      ElementC,
      4,
      ElementAccumulator,
      ElementCompute
    >,
    cutlass::gemm::threadblock::GemmIdentityThreadblockSwizzle<>,
    3,
    cutlass::arch::OpMultiplyAdd,
    cutlass::conv::IteratorAlgorithm::kAnalytic,
    cutlass::conv::StrideSupport::kUnity,
    4,
    4
  >::Kernel;

  using Conv2dDgrad = cutlass::conv::device::ImplicitGemmConvolution<Conv2dDgradKernel>;
 
  test::conv::device::Conv2dProblemVector problem_size_list;

  // run specific problem size in the unit test first
  problem_size_list.push_back(cutlass::conv::Conv2dProblemSize(
    {1, 4, 4, 12},     // input size (NHWC)
    {8, 3, 3, 12},     // filter size (KRSC)
    {0, 0, 0, 0},      // padding (pad_h, _, pad_w, _)
    {3, 3},            // stride (stride_h, stride_w)
    {1, 1}             // dilation (dilation_h, dilation_w)
  ));
 
  /// Run all unit test sizes with device-level Conv2d instance
  EXPECT_TRUE(test::conv::device::TestAllConv2d<Conv2dDgrad>(problem_size_list));
}

////////////////////////////////////////////////////////////////////////////////

TEST(SM80_Device_Conv2d_Dgrad_Optimized_ImplicitGemm_f16nhwc_f16nhwc_f16nhwc_tensor_op_f16_align4,
  128x128_64x3_64x64x64) {
 
  /// Conv operation element types for the Gemm equivalent (ImplicitGemm)
  using ElementA           = cutlass::half_t;
  using ElementB           = cutlass::half_t;
  using ElementC           = cutlass::half_t;
  using ElementAccumulator = cutlass::half_t;
  using ElementCompute     = cutlass::half_t;

  /// Device-level Conv2d instance
  using Conv2dDgradKernel = typename cutlass::conv::kernel::DefaultConv2dDgrad<
    ElementA, cutlass::layout::TensorNHWC,
    ElementB, cutlass::layout::TensorNHWC,
    ElementC, cutlass::layout::TensorNHWC,
    ElementAccumulator,
    cutlass::arch::OpClassTensorOp,
    cutlass::arch::Sm80,
    cutlass::gemm::GemmShape<128, 128, 64>,
    cutlass::gemm::GemmShape<64, 64, 64>,
    cutlass::gemm::GemmShape<16, 8, 16>,
    cutlass::epilogue::thread::LinearCombination<
      ElementC,
      4,
      ElementAccumulator,
      ElementCompute
    >,
    cutlass::gemm::threadblock::GemmIdentityThreadblockSwizzle<>,
    3,
    cutlass::arch::OpMultiplyAdd,
    cutlass::conv::IteratorAlgorithm::kOptimized,
    cutlass::conv::StrideSupport::kUnity,
    4,
    4
  >::Kernel;

  using Conv2dDgrad = cutlass::conv::device::ImplicitGemmConvolution<Conv2dDgradKernel>;
 
  test::conv::device::Conv2dProblemVector problem_size_list;

  // run specific problem size in the unit test first
  problem_size_list.push_back(cutlass::conv::Conv2dProblemSize(
    {1, 4, 4, 12},     // input size (NHWC)
    {8, 3, 3, 12},     // filter size (KRSC)
    {0, 0, 0, 0},      // padding (pad_h, _, pad_w, _)
    {3, 3},            // stride (stride_h, stride_w)
    {1, 1}             // dilation (dilation_h, dilation_w)
  ));
 
  /// Run all unit test sizes with device-level Conv2d instance
  EXPECT_TRUE(test::conv::device::TestAllConv2d<Conv2dDgrad>(problem_size_list));
}

////////////////////////////////////////////////////////////////////////////////

TEST(SM80_Device_Conv2d_Dgrad_Optimized_ImplicitGemm_f16nhwc_f16nhwc_f16nhwc_tensor_op_f16_align2,
  128x64_64x3_64x32x64) {
 
  /// Conv operation element types for the Gemm equivalent (ImplicitGemm)
  using ElementA           = cutlass::half_t;
  using ElementB           = cutlass::half_t;
  using ElementC           = cutlass::half_t;
  using ElementAccumulator = cutlass::half_t;
  using ElementCompute     = cutlass::half_t;

  /// Device-level Conv2d instance
  using Conv2dDgradKernel = typename cutlass::conv::kernel::DefaultConv2dDgrad<
    ElementA, cutlass::layout::TensorNHWC,
    ElementB, cutlass::layout::TensorNHWC,
    ElementC, cutlass::layout::TensorNHWC,
    ElementAccumulator,
    cutlass::arch::OpClassTensorOp,
    cutlass::arch::Sm80,
    cutlass::gemm::GemmShape<128, 64, 32>,
    cutlass::gemm::GemmShape<64, 32, 32>,
    cutlass::gemm::GemmShape<16, 8, 16>,
    cutlass::epilogue::thread::LinearCombination<
      ElementC,
      2,
      ElementAccumulator,
      ElementCompute
    >,
    cutlass::gemm::threadblock::GemmIdentityThreadblockSwizzle<>,
    3,
    cutlass::arch::OpMultiplyAdd,
    cutlass::conv::IteratorAlgorithm::kOptimized,
    cutlass::conv::StrideSupport::kUnity,
    2,
    2
  >::Kernel;

  using Conv2dDgrad = cutlass::conv::device::ImplicitGemmConvolution<Conv2dDgradKernel>;
 
  test::conv::device::Conv2dProblemVector problem_size_list;

  // run specific problem size in the unit test first
  problem_size_list.push_back(cutlass::conv::Conv2dProblemSize(
    {35, 100, 50, 64},     // input size (NHWC)
    {22, 1, 1, 64},     // filter size (KRSC)
    {0, 0, 0, 0},      // padding (pad_h, _, pad_w, _)
    {1, 1},            // stride (stride_h, stride_w)
    {1, 1}             // dilation (dilation_h, dilation_w)
  ));
 
  /// Run all unit test sizes with device-level Conv2d instance
  EXPECT_TRUE(test::conv::device::TestAllConv2d<Conv2dDgrad>(problem_size_list));
}

////////////////////////////////////////////////////////////////////////////////

#endif  // CUTLASS_ARCH_MMA_SM80_SUPPORTED
