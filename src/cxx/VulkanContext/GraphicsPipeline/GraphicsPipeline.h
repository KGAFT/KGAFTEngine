#include <vulkan/vulkan.h>
#include "PipelineConfig.h"
#include "../VulkanDevice/VulkanDevice.h"
#include "VulkanShader.h"
#include "../VulkanBuffers/VertexBufferDescriptionManager.h"
#include "../VulkanImmediateShaderData/PushConstants/PushConstantDescriptionManager.h"
#include "IDescriptorLayoutObject.h"


#pragma once

class GraphicsPipeline {
    friend class VulkanRenderingPipeline;

private:
    VulkanDevice *device;
    VkPipeline graphicsPipeline;
    PushConstantDescriptionManager *pcDescs;
    VkPipelineLayout pipelineLayout;

    VulkanShader *shader;
    VertexBufferDescriptionManager *manager;

    VkDescriptorSetLayout descriptorSetLayout;
public:
    GraphicsPipeline(VulkanShader *shader, VulkanDevice *device, PipelineConfiguration::PipelineConfigInfo &configInfo,
                     VertexBufferDescriptionManager *manager, PushConstantDescriptionManager *pcDescs,
                     IDescriptorLayoutObject **toLoad, unsigned int count) {
        this->device = device;
        this->pcDescs = pcDescs;

        this->shader = shader;
        this->manager = manager;

        VkDescriptorSetLayout layout = VK_NULL_HANDLE;
        if (count != 0 and toLoad != nullptr) {
            layout = loadDescriptors(toLoad, count);
        }
        createPipelineLayout(layout);
        create(shader, configInfo, manager);
    }

    void destroy() {

        vkDestroyPipeline(device->getDevice(), graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device->getDevice(), pipelineLayout, nullptr);
    }

    void recreate(unsigned int width, unsigned int height, VkRenderPass renderPass) {
        PipelineConfiguration::PipelineConfigInfo config = PipelineConfiguration::defaultPipelineConfigInfo(width,
                                                                                                            height);
        config.renderPass = renderPass;
        vkDestroyPipeline(device->getDevice(), graphicsPipeline, nullptr);
        create(shader, config, manager);
    }

    VkDescriptorSetLayout getDescriptorSetLayout() {
        return descriptorSetLayout;
    }

    ~GraphicsPipeline() {
        destroy();
    }

private:
    VkDescriptorSetLayout loadDescriptors(IDescriptorLayoutObject **toLoad, unsigned int count) {
        VkDescriptorSetLayout descriptorSetLayout;
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        for (unsigned int i = 0; i < count; i++) {
            bindings.push_back(toLoad[i]->getDescriptorSetLayoutBind());
        }
        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = bindings.size();
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
        return descriptorSetLayout;
    }

    void createPipelineLayout(VkDescriptorSetLayout layout) {
        this->descriptorSetLayout = layout;
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        if (layout != VK_NULL_HANDLE) {
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &layout;
        }
        pipelineLayoutInfo.pushConstantRangeCount = pcDescs->getPushConstantRanges().size();
        pipelineLayoutInfo.pPushConstantRanges = pcDescs->getPushConstantRanges().data();

        if (vkCreatePipelineLayout(device->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void create(VulkanShader *shader, PipelineConfiguration::PipelineConfigInfo &configInfo,
                VertexBufferDescriptionManager *manager) {


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