//
// Created by loochis on 2/4/24.
//

#ifndef VOXCHIS_VKCSWAPCHAIN_H
#define VOXCHIS_VKCSWAPCHAIN_H

#include "../Headers/VKCStructs.h"
#include "../Headers/ColorMessages.h"
#include "VKCDevice.h"
#include "VKCImage.h"
#include "VKCCommandManager.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <GLFW/glfw3.h>

namespace VKChis {

    using namespace std;

    // VKC SWAPCHAIN IS ALSO RESPONSIBLE FOR IT'S OWN IMAGE VIEWS

    class VKCSwapChain {
    public:
        // need to expose flags so recreation doesn't spam console
        uint32_t flags;

        VkSwapchainKHR swapChain;

        // Can't abstract this to VKCImage (Memory is handled by swapchain)
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        unique_ptr<VKCImage> image_depth;

        VkFormat format_swapChainImage;
        VkFormat format_depth;

        VkExtent2D swapChainExtent;

        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;

        VKCSwapChain(uint32_t in_flags,
                     shared_ptr<VKCDevice> &in_device,
                     shared_ptr<VKCCommandManager> &in_commandManager,
                     VkSurfaceKHR in_surface,
                     GLFWwindow *in_window,
                     VkResult &result);

        ~VKCSwapChain();

        // Framebuffers
        void createFrameBuffers(VkRenderPass renderPass);
    private:
        VkSurfaceKHR surface;
        shared_ptr<VKCDevice> device;
        shared_ptr<VKCCommandManager> commandManager;
        GLFWwindow *window;

        QueueFamilyIndices indices;
        SwapChainSupportDetails swapChainSupport;

        // Swapchain selector functions
        static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

        // Format selectors
        VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();

        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        // Image view creation/deletion
        void createImageViews();
    };

} // VKChis

#endif //VOXCHIS_VKCSWAPCHAIN_H
