//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCSTRUCTS_H
#define VOXCHIS_VKCSTRUCTS_H

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

#endif //VOXCHIS_VKCSTRUCTS_H
