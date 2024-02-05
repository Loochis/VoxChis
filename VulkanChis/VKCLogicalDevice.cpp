//
// Created by loochis on 2/3/24.
//

#include <utility>
#include <vector>
#include <set>
#include <stdexcept>
#include "VKCLogicalDevice.h"
#include "Utils/ColorMessages.h"

namespace VKChis {
    VKCLogicalDevice::VKCLogicalDevice(uint32_t in_flags,
                                       std::vector<const char*> in_validationLayers,
                                       std::vector<const char*> in_deviceExtensions,
                                       VkInstance in_instance, VkSurfaceKHR in_surface,
                                       VkResult &result)
    :   flags(in_flags),
        validationLayers(std::move(in_validationLayers)),
        deviceExtensions(std::move(in_deviceExtensions)),
        instance(in_instance),
        surface(in_surface)
    {

        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        // Find a suitable physical GPU
        PickPhysicalDevice(result);

        if (result) {
            if (enableValidation)
                print_colored("/// WARN /// - Failed to find suitable physical device", YELLOW);
            return;
        }

        if (enableValidation)
            print_colored("/// GOOD /// - Found Suitable GPU", GREEN);

        if (flags & VKC_DISPLAY_GPU_INFO) {
            VkPhysicalDeviceProperties pProps;
            vkGetPhysicalDeviceProperties(physicalDevice, &pProps);
            std::string msg1 = "/// INFO /// - GPU Name:   ";
            msg1 += pProps.deviceName;
            print_colored(msg1, WHITE);
            std::string msg2 = "/// INFO /// - GPU ID:     ";
            msg2 += std::to_string(pProps.deviceID);
            print_colored(msg2, WHITE);
            std::string msg3 = "/// INFO /// - API Ver.    ";
            msg3 += std::to_string(pProps.apiVersion);
            print_colored(msg3, WHITE);
            std::string msg4 = "/// INFO /// - Driver Ver. ";
            msg4 += std::to_string(pProps.driverVersion);
            print_colored(msg4, WHITE);
        }

        // Fill in global variables for reference later
        indices = findQueueFamilies(physicalDevice);
        swapChainSupport = querySwapChainSupport(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // DEPRECATED FEATURE, set to 0 for back compat
        //createInfo.enabledExtensionCount = 0;

        if (enableValidation) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }

    void VKCLogicalDevice::PickPhysicalDevice(VkResult &result) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto& ck_physDevice : devices) {
            if (isDeviceSuitable(ck_physDevice)) {
                physicalDevice = ck_physDevice;
                break;
            }
        }

        result = VK_SUCCESS;

        if (physicalDevice == VK_NULL_HANDLE)
            result = VK_ERROR_DEVICE_LOST;
    }

    bool VKCLogicalDevice::isDeviceSuitable(VkPhysicalDevice ck_physDevice) {
        QueueFamilyIndices ck_indices = findQueueFamilies(ck_physDevice);

        bool extensionsSupported = checkDeviceExtensionSupport(ck_physDevice);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails ck_swapChainSupport = querySwapChainSupport(ck_physDevice);
            swapChainAdequate = !ck_swapChainSupport.formats.empty() && !ck_swapChainSupport.presentModes.empty();
        }

        return ck_indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    QueueFamilyIndices VKCLogicalDevice::findQueueFamilies(VkPhysicalDevice ck_device) {
        QueueFamilyIndices ck_indices;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(ck_device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(ck_device, &queueFamilyCount, queueFamilies.data());

        // FIND BLIT QUEUE
        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                ck_indices.graphicsFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(ck_device, i, surface, &presentSupport);

            if (presentSupport) {
                ck_indices.presentFamily = i;
            }

            if (ck_indices.isComplete()) {
                break;
            }

            i++;
        }

        return ck_indices;
    }

    bool VKCLogicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice ck_device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(ck_device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(ck_device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    SwapChainSupportDetails VKCLogicalDevice::querySwapChainSupport(VkPhysicalDevice ck_device) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ck_device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(ck_device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(ck_device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(ck_device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(ck_device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    VKCLogicalDevice::~VKCLogicalDevice() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyDevice(device, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Device", CYAN);
    }
} // VKChis