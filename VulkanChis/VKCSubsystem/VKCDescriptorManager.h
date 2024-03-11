//
// Created by loochis on 3/5/24.
//

#ifndef VOXCHIS_VKCDESCRIPTORMANAGER_H
#define VOXCHIS_VKCDESCRIPTORMANAGER_H

#include <vulkan/vulkan.h>
#include "VKCDevice.h"
#include "../VKCDynamicComponents/VKCDescriptorSetLayout.h"
#include "../VKCDynamicComponents/VKCBuffer.h"

using namespace std;

namespace VKChis {

    class VKCDescriptorManager {
    public:
        VkDescriptorPool descriptorPool;
        vector<VkDescriptorSet> descriptorSets;

        unique_ptr<vector<VKCBuffer>> uniform_buffers;
        vector<void*> uniformBuffersMapped; // Persistent mapped buffer vec

        VKCDescriptorManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, shared_ptr<VKCDescriptorSetLayout> &in_descLayout, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result);
        ~VKCDescriptorManager();

        // Per Object utils
        vkc_Result AllocateObjectDescriptors();
        vkc_Result UpdateCameraUBOData(CameraMatrixUBO &ubo, int obj, int currentFrame);
        vkc_Result UpdateModelUBOData(ModelMatrixUBO &ubo, int obj, int currentFrame);
    private:
        uint32_t flags;

        VkDescriptorSetLayout cameraSetLayout;
        VkDescriptorSetLayout modelSetLayout;

        size_t dynamicAlignment;

        shared_ptr<VKCDevice> device;
        shared_ptr<VKCDescriptorSetLayout> descLayout;
        int MAX_FRAMES_IN_FLIGHT;
    };

} // VKChis

#endif //VOXCHIS_VKCDESCRIPTORMANAGER_H
