// Copyright 2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "core/buffer.h"
#include "sycl_engine.h"

OIDN_NAMESPACE_BEGIN

  class SYCLExternalBuffer : public USMBuffer
  {
  public:
    SYCLExternalBuffer(const Ref<SYCLEngine>& engine,
                       ExternalMemoryTypeFlag fdType,
                       int fd, size_t byteSize);

    SYCLExternalBuffer(const Ref<SYCLEngine>& engine,
                       ExternalMemoryTypeFlag handleType,
                       void* handle, const void* name, size_t byteSize);

    ~SYCLExternalBuffer();

  private:
    void init(const Ref<SYCLEngine>& engine, const void* importDesc, size_t byteSize);
  };

OIDN_NAMESPACE_END
