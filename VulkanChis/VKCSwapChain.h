//
// Created by loochis on 2/4/24.
//

#ifndef VOXCHIS_VKCSWAPCHAIN_H
#define VOXCHIS_VKCSWAPCHAIN_H

#include "Utils/VKCStructs.h"
#include "Utils/ColorMessages.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>

namespace VKChis {

    class VKCSwapChain {
    public:
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        VKCSwapChain(VkSurfaceKHR in_surface,
                     VkDevice in_device,
                     GLFWwindow *in_window,
                     QueueFamilyIndices &in_indices,
                     SwapChainSupportDetails &in_swapChainSupport,
                     VkResult &result);

        ~VKCSwapChain();
    private:
        VkSurfaceKHR surface;
        VkDevice device;
        GLFWwindow *window;

        QueueFamilyIndices indices;
        SwapChainSupportDetails swapChainSupport;

        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
    };

} // VKChis

#endif //VOXCHIS_VKCSWAPCHAIN_H
