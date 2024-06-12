//
// Created by loochis on 3/5/24.
//

#ifndef VOXCHIS_VKCDESCRIPTORMANAGER_H
#define VOXCHIS_VKCDESCRIPTORMANAGER_H

#include <vulkan/vulkan.h>
#include "VKCDevice.h"
#include "VKCBuffer.h"
#include "../VKCUtils/VKCAssetManager.h"

using namespace std;

namespace VKChis {

    class VKCDescriptorManager {
    public:
        VkDescriptorPool descriptorPool;

        VkDescriptorSetLayout descSetLayout;

        vector<VkDescriptorSet> descriptorSets;
        vector<VKCBuffer> uniform_buffers;
        vector<void*> uniformBuffersMapped; // Persistent mapped buffer vec

        VKCDescriptorManager(uint32_t in_flags, shared_ptr<VKCDevice> &in_device, shared_ptr<VKCAssetManager> &in_assetManager, int in_MAX_FRAMES_IN_FLIGHT, VkResult &result);
        ~VKCDescriptorManager();

        vkc_Result UpdateUBOs(int currentFrame);

        // Per Object utils
        vkc_Result AllocateObjectDescriptors();
        //vkc_Result UpdateCameraUBOData(CameraMatrixUBO &ubo, int currentFrame);
        //vkc_Result UpdateModelUBOData(ModelMatrixUBO &ubo, int currentFrame);


        // Modifiable Data
        CameraMatrixUBO cameraMatrix;

        // Read-only
        size_t dynamicAlignment;
    private:
        uint32_t flags;

        shared_ptr<VKCDevice> device;
        shared_ptr<VKCAssetManager> assetManager;
        int MAX_FRAMES_IN_FLIGHT;
    };

} // VKChis

#endif //VOXCHIS_VKCDESCRIPTORMANAGER_H
