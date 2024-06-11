//
// Created by loochis on 2/4/24.
//

#ifndef VOXCHIS_VKCSWAPCHAIN_H
#define VOXCHIS_VKCSWAPCHAIN_H

#include "../Headers/VKCStructs.h"
#include "../Headers/ColorMessages.h"
#include "VKCDevice.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>

namespace VKChis {

    using namespace std;

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

        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;

        VKCSwapChain(uint32_t in_flags,
                     VkSurfaceKHR in_surface,
                     shared_ptr<VKCDevice> &in_device,
                     GLFWwindow *in_window,
                     VkResult &result);

        ~VKCSwapChain();

        // Framebuffers
        void createFrameBuffers(VkRenderPass renderPass);
    private:


        VkSurfaceKHR surface;
        shared_ptr<VKCDevice> device;
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
