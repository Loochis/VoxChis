//
// Created by loochis on 2/3/24.
//

#ifndef VOXCHIS_VKCDEVICE_H
#define VOXCHIS_VKCDEVICE_H

#include <memory>
#include <vulkan/vulkan.h>
#include "../Utils/VKCStructs.h"
#include "../Utils/VKCEnumerations.h"

namespace VKChis {

    class VKCDevice {
    public:
        VkPhysicalDevice physicalDevice;
        VkDevice device;

        QueueFamilyIndices indices;
        SwapChainSupportDetails swapChainSupport;

        VkQueue graphicsQueue;
        VkQueue presentQueue;

        explicit VKCDevice(uint32_t in_flags,
                           std::vector<const char*> in_validationLayers,
                           std::vector<const char*> in_deviceExtensions,
                           VkInstance in_instance,
                           VkSurfaceKHR in_surface,
                           VkResult &result);
        ~VKCDevice();
    private:
        uint32_t flags;

        VkInstance instance;
        VkSurfaceKHR surface;

        std::vector<const char*> validationLayers;
        std::vector<const char*> deviceExtensions;

        void PickPhysicalDevice(VkResult &result);
        bool isDeviceSuitable(VkPhysicalDevice ck_physDevice);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice ck_device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice ck_device);
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice ck_device);
    };

} // VKChis

#endif //VOXCHIS_VKCDEVICE_H
