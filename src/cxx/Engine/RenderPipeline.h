//
// Created by Daniil on 06.02.2023.
//
#pragma once

#include "../VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "../VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "../VulkanContext/VulkanBuffers/VertexBufferDescriptionManager.h"
#include "../VulkanContext/VulkanImmediateShaderData/PushConstants/PushConstant.h"
#include "../VulkanContext/VulkanImmediateShaderData/PushConstants/PushConstantDescriptionManager.h"
#include "../VulkanContext/GraphicsPipeline/PipelineConfig.h"
#include "../VulkanContext/VulkanImage/VulkanSampler.h"
#include "../VulkanContext/VulkanImmediateShaderData/UniformBuffers/UniformBuffer.h"
#include "../VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"
#include "../VulkanContext/VulkanDescriptor/VulkanDescritptors.h"
#include "ShaderInputDataConfiguration.h"

class RenderPipeline : public RenderPassCallback{
protected:
    VulkanDevice* device;
    VulkanSwapChain* swapChain;
    VulkanRenderPass* renderPass;
    VulkanSwapChainControl* swapChainControl;
    VertexBufferDescriptionManager* vertexBufferDescriptionManager;
    PushConstantDescriptionManager* pushConstantDescriptionManager;
    PipelineConfiguration::PipelineConfigInfo pipelineConfig;
    GraphicsPipeline* pipeline;
    VulkanRenderingPipeline* renderingPipeline;
    VulkanDescriptors* descriptors;

    std::vector<VulkanSampler*> samplers;
    std::vector<PushConstant*> pushConstants;
    std::vector<UniformBuffer*> uniformBuffers;
protected:
    virtual void preInvokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) = 0;
    virtual void invokeRenderImm(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) = 0;
public:
    void invokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {
        preInvokeRender(commandBuffer, layout, imageIndex);
        std::vector<IDescriptorLayoutObject*> objectsToWrite;
        for(VulkanSampler* sampler : samplers){
            objectsToWrite.push_back(sampler);
        }
        for (const auto item: uniformBuffers){
            objectsToWrite.push_back(item);
        }
        pushConstantDescriptionManager->loadConstantsToShader(commandBuffer, layout);
        descriptors->writeDescriptor(objectsToWrite.data(), objectsToWrite.size(), imageIndex);
        descriptors->bind(imageIndex, commandBuffer, layout);
        invokeRenderImm(commandBuffer, layout, imageIndex);
    }


    RenderPipeline(VulkanDevice* device, Window* window, ShaderInputDataConfiguration& config) : device(device){
        swapChain = new VulkanSwapChain(device, window->getWidth(), window->getHeight());
        renderPass = new VulkanRenderPass(device, swapChain);
        swapChainControl = new VulkanSwapChainControl(swapChain, renderPass, device);
        vertexBufferDescriptionManager = new VertexBufferDescriptionManager();
        populateVertexBuffersDescription(config);
        pushConstantDescriptionManager = new PushConstantDescriptionManager();
        populatePushConstantsManager(config);

        pipelineConfig = PipelineConfiguration::defaultPipelineConfigInfo(window->getWidth(), window->getHeight());
        populateUniforms(config);

        std::vector<IDescriptorLayoutObject*> objectsToWrite;
        for(VulkanSampler* sampler : samplers){
            objectsToWrite.push_back(sampler);
        }
        for (const auto item: uniformBuffers){
            objectsToWrite.push_back(item);
        }

        pipeline = new GraphicsPipeline(config.shader, device, pipelineConfig, vertexBufferDescriptionManager, pushConstantDescriptionManager, objectsToWrite.data(), objectsToWrite.size());

        renderingPipeline = new VulkanRenderingPipeline(swapChainControl, renderPass, pipeline, device);

        window->registerResizeCallback(renderingPipeline);

        descriptors = new VulkanDescriptors(device, pipeline->getDescriptorSetLayout(), 3, objectsToWrite.data(), objectsToWrite.size());

        renderingPipeline->registerRenderPassCallback(this);

    }
    void update(){
        renderingPipeline->redrawCommandBuffers();
    }
private:
    void populateVertexBuffersDescription(ShaderInputDataConfiguration& config){
        size_t stepSize = 0;
        for (int i = 0; i < config.vertexBufferDescriptionAmount; ++i){
            vertexBufferDescriptionManager->registerAttribute(0, config.pVertexBufferDescription[i].location, config.pVertexBufferDescription[i].vertexCoordinatesAmount, config.pVertexBufferDescription[i].offset);
            stepSize+=config.pVertexBufferDescription[i].vertexCoordinatesAmount*sizeof(float);
        }
        vertexBufferDescriptionManager->createBinding(0, stepSize);
    }
    void populatePushConstantsManager(ShaderInputDataConfiguration& config){
        for (int i = 0; i < config.pushConstantDescriptionAmount; ++i){
            pushConstants.push_back(new PushConstant(config.pushConstantDescription[i].constantSize, config.pushConstantDescription[i].shaderStages));
            pushConstantDescriptionManager->registerPushConstant(pushConstants[i]);
        }
    }
    void populateUniforms(ShaderInputDataConfiguration& config){
        for (int i = 0; i < config.uniformBufferDescriptionAmount; ++i){
            uniformBuffers.push_back(new UniformBuffer(device, config.pUniformBufferDescription[i].bufferSize, config.pUniformBufferDescription[i].shaderLocation, config.pUniformBufferDescription[i].targetBinding, 3));
        }
    }
};