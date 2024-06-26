//
// Created by loochis on 1/28/24.
//

#include <vulkan/vulkan.h>
#include "VKCValidator.h"

namespace VKChis {
    bool VKCValidator::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
        uint32_t layerCount;
        // call once to get count
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        // call again to fill vector
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // loop through until all layers are matched
        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }
} // VKChis