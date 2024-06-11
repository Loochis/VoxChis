//
// Created by loochis on 2/5/24.
//

#include "VKCGraphicsPipeline.h"

#include "../Headers/ColorMessages.h"
#include "../Headers/VKCEnumerations.h"
#include "../Headers/VKCStructs.h"
#include <utility>


namespace VKChis {
    // The monster begins
    VKCGraphicsPipeline::VKCGraphicsPipeline(uint32_t in_flags, shared_ptr<vector<VKCShaderModule>> &in_shader_modules,
                                             VkExtent2D in_swapChainExtent, shared_ptr<VKCDevice> &in_device,
                                             shared_ptr<VKCDescriptorManager> &in_descSetManager, VkRenderPass in_renderPass, VkResult &result)
            : flags(in_flags),
              shader_modules(in_shader_modules),
              swapChainExtent(in_swapChainExtent),
              device(in_device),
              descSetManager(in_descSetManager),
              renderPass(in_renderPass) {


        VkPipelineShaderStageCreateInfo shaderStages[shader_modules->size()];
        for (int i = 0; i < shader_modules->size(); i++) {
            shaderStages[i] = {};
            shaderStages[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

            switch ((*shader_modules)[i].shader_type) {
                case 0:
                    shaderStages[i].stage = VK_SHADER_STAGE_VERTEX_BIT;
                    break;
                case 1:
                    shaderStages[i].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                    break;
            }

            shaderStages[i].module = (*shader_modules)[i].shaderModule;
            shaderStages[i].pName = "main";
        }

        // TESTING

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        // END TESTING

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapChainExtent.width;
        viewport.height = (float) swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;

        std::vector<VkDynamicState> dynamicStates = {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;

        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        //rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        //rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

        // TODO: Use standard way of flipping Y coordinate instead of matrix fiddling
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT;
        // THIS BLENDS BASED ON ALPHA newcol*alpha + oldcol*(1-alpha) [ala LERP]
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        //vector<VkDescriptorSetLayout> descSetLayouts = {descSetManager->transformationSetLayout};

        //setup push constants
        VkPushConstantRange push_constant;
        push_constant.offset = 0;
        push_constant.size = sizeof(glm::mat4) * 2;
        push_constant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = descSetManager->descSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = descSetManager->descSetLayouts.data();
        //pipelineLayoutInfo.setLayoutCount = 0;
        //pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1; // Optional
        pipelineLayoutInfo.pPushConstantRanges = &push_constant; // Optional

        result = vkCreatePipelineLayout(device->device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
        if (result) {
            print_colored("/// WARNING /// - Failed to create GFX Pipeline Layout", YELLOW);
            return;
        }

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = shader_modules->size();
        pipelineInfo.pStages = shaderStages;

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;

        pipelineInfo.layout = pipelineLayout;

        //pipelineInfo.renderPass = renderPass;
        pipelineInfo.renderPass = renderPass; // TEMP
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        result = vkCreateGraphicsPipelines(device->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
    }

    VKCGraphicsPipeline::~VKCGraphicsPipeline() {
        bool enableValidation = flags & VKC_ENABLE_VALIDATION_LAYER;

        vkDestroyPipeline(device->device, graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device->device, pipelineLayout, nullptr);

        if (enableValidation) print_colored("/// CLEAN /// - Destroyed GFX Pipeline", CYAN);
        if (enableValidation) print_colored("/// CLEAN /// - Destroyed GFX Pipeline Layout", CYAN);
    }
    // The monster ends

} // VKChis