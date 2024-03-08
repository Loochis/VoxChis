//
// Created by loochis on 3/5/24.
//

#include "VKCDescriptorSetLayout.h"
#include "../Utils/ColorMessages.h"

namespace VKChis {

    VKCDescriptorSetLayout::VKCDescriptorSetLayout(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, VkResult &result)
    :   flags(in_flags),
        device(in_device)
    {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;

        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        result = vkCreateDescriptorSetLayout(device->device, &layoutInfo, nullptr, &descriptorSetLayout);
    }

    VKCDescriptorSetLayout::~VKCDescriptorSetLayout() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;
        
        vkDestroyDescriptorSetLayout(device->device, descriptorSetLayout, nullptr);

        if (enableValidation) print_colored("/// CLEAN /// - Destroyed Descriptor Set Layout", CYAN);
    }
} // VKChis