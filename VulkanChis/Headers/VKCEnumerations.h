//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCENUMERATIONS_H
#define VOXCHIS_VKCENUMERATIONS_H

#include <cstdint>

enum vkc_InitFlags : uint32_t {
        VKC_ENABLE_VALIDATION_LAYER = 1 << 0,
        VKC_DISPLAY_GPU_INFO        = 1 << 1,
        VKC_ASSET_LOAD_DEBUG        = 1 << 2
};

enum vkc_Result : uint32_t {
    VKC_SUCCESS                     = 1 << 0,
    VKC_ALLOCATION_FAILURE          = 1 << 1
};

#endif //VOXCHIS_VKCENUMERATIONS_H
