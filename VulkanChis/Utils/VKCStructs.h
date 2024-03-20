//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCSTRUCTS_H
#define VOXCHIS_VKCSTRUCTS_H

#include <vulkan/vulkan.h>
#include <glm/glm.hpp> // Linalg (Yippee!)
#include <array>
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

struct Vertex {
    glm::vec3 pos;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};

        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        return attributeDescriptions;
    }

};

struct CameraMatrixUBO {
    glm::mat4 view;
    glm::mat4 proj;
};

struct ModelMatrixUBO {
    glm::mat4 model;
};

#endif //VOXCHIS_VKCSTRUCTS_H
