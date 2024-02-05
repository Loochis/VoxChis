//
// Created by loochis on 2/5/24.
//

#ifndef VOXCHIS_VKCGRAPHICSPIPELINE_H
#define VOXCHIS_VKCGRAPHICSPIPELINE_H

#include <vulkan/vulkan.h>
#include <cstdint>

namespace VKChis {

    class VKCGraphicsPipeline {
    public:
        VkPipelineLayout pipelineLayout;

        explicit VKCGraphicsPipeline(uint32_t flags);
        ~VKCGraphicsPipeline();
    };

} // VKChis

#endif //VOXCHIS_VKCGRAPHICSPIPELINE_H
