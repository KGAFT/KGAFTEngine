//
// Created by daniil on 26.01.23.
//

#include <vulkan/vulkan.h>
#include "PipelineConfig.h"
#include "../VulkanDevice/VulkanDevice.h"
#include "VulkanShader.h"
#include "../VulkanBuffers/VertexBufferDescriptionManager.h"
#include "../VulkanImmediateShaderData/PushConstants/PushConstantDescriptionManager.h"

#pragma once

class GraphicsPipeline{
    friend class VulkanRenderingPipeline;
private:
    VulkanDevice* device;
    VkPipeline graphicsPipeline;
    PushConstantDescriptionManager* pcDescs;
    VkPipelineLayout pipelineLayout;



public:
    GraphicsPipeline(VulkanShader* shader, VulkanDevice* device, PipelineConfiguration::PipelineConfigInfo& configInfo, VertexBufferDescriptionManager* manager, PushConstantDescriptionManager* pcDescs){
        this->device = device;
        this->pcDescs = pcDescs;
        createPipelineLayout();
        create(shader, configInfo, manager);
    }
private:

    void createPipelineLayout(){
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = pcDescs->getPushConstantRanges().size();
        pipelineLayoutInfo.pPushConstantRanges = pcDescs->getPushConstantRanges().data();
        if (vkCreatePipelineLayout(device->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void create(VulkanShader* shader, PipelineConfiguration::PipelineConfigInfo& configInfo, VertexBufferDescriptionManager* manager){


        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = manager->getAttributeDescriptions().size();
        vertexInputInfo.vertexBindingDescriptionCount = manager->getBindingDescription().size();
        vertexInputInfo.pVertexAttributeDescriptions = manager->getAttributeDescriptions().data();
        vertexInputInfo.pVertexBindingDescriptions = manager->getBindingDescription().data();

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &configInfo.viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &configInfo.scissor;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shader->getStages();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
        pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
        pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
        pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
        pipelineInfo.pDynamicState = nullptr;

        pipelineInfo.layout = pipelineLayout;
        pipelineInfo.renderPass = configInfo.renderPass;
        pipelineInfo.subpass = configInfo.subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        if (vkCreateGraphicsPipelines(
                device->getDevice(),
                VK_NULL_HANDLE,
                1,
                &pipelineInfo,
                nullptr,
                &graphicsPipeline) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline");
        }
    }
};