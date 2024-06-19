//
// Created by loochis on 2/4/24.
//


#include <stdexcept>
#include <limits>
#include <algorithm>
#include "VKCSwapChain.h"
#include "../Headers/VKCEnumerations.h"

namespace VKChis {
    VKCSwapChain::VKCSwapChain(uint32_t in_flags,
                               shared_ptr<VKCDevice> &in_device,
                               shared_ptr<VKCCommandManager> &in_commandManager,
                               VkSurfaceKHR in_surface,
                               GLFWwindow *in_window,
                               VkResult &result)
    :   flags(in_flags),
        device(in_device),
        commandManager(in_commandManager),
        surface(in_surface),
        window(in_window),
        indices(device->indices),
        swapChainSupport(device->swapChainSupport)
    {
        surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        std::cout << width << ", " << height;

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;


        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        // create the swap chain
        result = vkCreateSwapchainKHR(device->device, &createInfo, nullptr, &swapChain);
        if (result) return; // Swap chain creation failure

        vkGetSwapchainImagesKHR(device->device, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(device->device, swapChain, &imageCount, swapChainImages.data());

        format_swapChainImage = surfaceFormat.format;
        format_depth = findDepthFormat();
        swapChainExtent = extent;

        // Create the depth image / views
        image_depth = make_unique<VKCImage>(flags, device, extent.width, extent.height, 1, format_depth, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

        // transition the depth image to depth attachment
        image_depth->transitionImageLayout(commandManager, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        // IMAGE VIEWS

        createImageViews();
    }

    VkSurfaceFormatKHR VKCSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) {
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VKCSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VKCSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            const char* desc;
            int error = glfwGetError(&desc);
            printf("GLFW error: %d, %s\n", error, desc);

            std::cout << width << " " << height << "\n";

            VkExtent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    VkFormat VKCSwapChain::findDepthFormat() {
        return findSupportedFormat(
                {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                VK_IMAGE_TILING_OPTIMAL,
                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    // TODO: Move all of the feature checks to a class or header or something idfk
    VkFormat VKCSwapChain::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(device->physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");
    }

    void VKCSwapChain::createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = format_swapChainImage;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device->device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("/// FATAL ERROR /// Failed to create Image Views!");
            }
        }

        if (flags & VKC_ENABLE_VALIDATION_LAYER) {
            std::string msg = "/// INFO /// - Created Image Views (";
            msg += std::to_string(swapChainImageViews.size());
            msg += ")";
            print_colored(msg, WHITE);
        }
    }

    void VKCSwapChain::createFrameBuffers(VkRenderPass renderPass) {
        swapChainFramebuffers.resize(swapChainImageViews.size());

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            vector<VkImageView> attachments = {
                    swapChainImageViews[i],
                    image_depth->imageView
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass;
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device->device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("/// FATAL ERROR /// Failed to create framebuffer!");
            }
        }

        if (flags & VKC_ENABLE_VALIDATION_LAYER) {
            std::string msg = "/// INFO /// - Created Framebuffers (";
            msg += std::to_string(swapChainImageViews.size());
            msg += ")";
            print_colored(msg, WHITE);
        }
    }

    VKCSwapChain::~VKCSwapChain() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        std::string msg = "/// CLEAN /// - Destroyed Framebuffers (";
        msg += std::to_string(swapChainFramebuffers.size());
        msg += ")";

        for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
            vkDestroyFramebuffer(device->device, swapChainFramebuffers[i], nullptr);
        }
        if (enableValidation) print_colored(msg, WHITE);

        msg = "/// CLEAN /// - Destroyed Image Views (";
        msg += std::to_string(swapChainImageViews.size());
        msg += ")";

        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            vkDestroyImageView(device->device, swapChainImageViews[i], nullptr);
        }
        if (enableValidation) print_colored(msg, WHITE);

        vkDestroySwapchainKHR(device->device, swapChain, nullptr);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed SwapChain", CYAN);
    }

} // VKChis