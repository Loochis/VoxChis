//
// Created by loochis on 2/4/24.
//

#ifndef VOXCHIS_VKCSWAPCHAIN_H
#define VOXCHIS_VKCSWAPCHAIN_H

#include "../Utils/VKCStructs.h"
#include "../Utils/ColorMessages.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <GLFW/glfw3.h>

namespace VKChis {

    // VKC SWAPCHAIN IS ALSO RESPONSIBLE FOR IT'S OWN IMAGE VIEWS

    class VKCSwapChain {
    public:
        // need to expose flags so recreation doesnt spam console
        uint32_t flags;

        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        VKCSwapChain(uint32_t in_flags,
                     VkSurfaceKHR in_surface,
                     VkDevice in_device,
                     GLFWwindow *in_window,
                     QueueFamilyIndices &in_indices,
                     SwapChainSupportDetails &in_swapChainSupport,
                     VkResult &result);

        ~VKCSwapChain();

        // Framebuffers
        void createFrameBuffers(VkRenderPass renderPass);
    private:


        VkSurfaceKHR surface;
        VkDevice device;
        GLFWwindow *window;

        QueueFamilyIndices indices;
        SwapChainSupportDetails swapChainSupport;

        // Swapchain selector functions
        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        // Image view creation/deletion
        void createImageViews();
    };

} // VKChis

#endif //VOXCHIS_VKCSWAPCHAIN_H
