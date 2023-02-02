//
// Created by Daniil on 02.02.2023.
//
#include <vulkan/vulkan.h>
#include "../VulkanContext/VulkanDevice/VulkanDevice.h"
#include "../VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "../VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "../VulkanContext/VulkanBuffers/VertexBufferDescriptionManager.h"
#include "../VulkanContext/VulkanImmediateShaderData/PushConstants/PushConstant.h"
#include "../VulkanContext/VulkanImmediateShaderData/PushConstants/PushConstantDescriptionManager.h"
#include "../VulkanContext/VulkanImmediateShaderData/UniformBuffers/UniformBuffer.h"
#include "../VulkanContext/VulkanImage/VulkanSampler.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"
#include "../VulkanContext/VulkanDescriptor/VulkanDescritptors.h"
#include "ShaderInputDataConfiguration.h"


#pragma once

class RenderPipeline  : public RenderPassCallback, PreRenderPassCallback{
protected:
    VulkanDevice* device;

    VulkanSwapChain* swapChain;
    VulkanRenderPass* renderPass;
    VulkanSwapChainControl* control;

    VertexBufferDescriptionManager* vertexBufferDescriptionManager;

    PushConstantDescriptionManager* pushConstantDescriptionManager;

    std::vector<UniformBuffer*> uniformBuffers;
    std::vector<PushConstant*> pushConstants;
    std::vector<VulkanSampler*> textureSamplers;

    VulkanRenderingPipeline* renderingPipeline;
    VulkanDescriptors* descriptors;
    GraphicsPipeline* graphicsPipeline;
    Window* window;
    VulkanShader* shader;
    RenderPipeline(VulkanDevice* device, Window* window) : device(device), window(window){

    }
    void load(ShaderInputDataConfiguration& configuration){
        swapChain = new VulkanSwapChain(device, Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
        renderPass = new VulkanRenderPass(device, swapChain);
        control = new VulkanSwapChainControl(swapChain, renderPass, device);
        vertexBufferDescriptionManager = new VertexBufferDescriptionManager();
        pushConstantDescriptionManager = new PushConstantDescriptionManager();
        PipelineConfiguration::PipelineConfigInfo config = PipelineConfiguration::defaultPipelineConfigInfo(Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
        config.renderPass = renderPass->getRenderPass();
        populatePipelineDescriptorObjects(configuration);
        this->shader = configuration.shader;
        loadGraphicsPipeline(shader,config );
        loadDescriptors();
        renderingPipeline = new VulkanRenderingPipeline(control, renderPass, graphicsPipeline, device);
        window->registerResizeCallback(renderingPipeline);
    }


    ~RenderPipeline(){
        destroy();
    }

    void destroy(){
        vkDeviceWaitIdle(device->getDevice());
        delete swapChain;
        delete renderPass;
        VkDescriptorSetLayout layout = graphicsPipeline->getDescriptorSetLayout();
        delete graphicsPipeline;
        for (const auto &item: uniformBuffers){
            delete item;
        }
        uniformBuffers.clear();
        for (const auto &item: pushConstants){
            delete item;
        }
        pushConstants.clear();
        delete descriptors;
        for (const auto &item: textureSamplers){
            delete item;
        }
        vkDestroyDescriptorSetLayout(device->getDevice(), layout, nullptr);
        delete control;
        delete renderingPipeline;
        delete vertexBufferDescriptionManager;
        delete pushConstantDescriptionManager;
        delete shader;
    }
private:
    void populatePipelineDescriptorObjects(ShaderInputDataConfiguration& configuration){
        if (configuration.vertexBufferDescriptionAmount == 0){
            throw new std::runtime_error("Error: you cannot create pipeline withount vertex inputs!");
        }

        size_t bindingSize = 0;
        for (int i = 0; i < configuration.vertexBufferDescriptionAmount; ++i){
            vertexBufferDescriptionManager->registerAttribute(0, configuration.pVertexBufferDescription[i].location, configuration.pVertexBufferDescription[i].vertexCoordinatesAmount, configuration.pVertexBufferDescription[i].offset);
            bindingSize+=configuration.pVertexBufferDescription[i].vertexCoordinatesAmount*sizeof(float);
        }
        vertexBufferDescriptionManager->createBinding(0, bindingSize);
        for (int i = 0; i < configuration.pushConstantDescriptionAmount; ++i){
            PushConstant* pushConstant = new PushConstant(configuration.pushConstantDescription[i].constantSize, configuration.pushConstantDescription[i].shaderStages);
            pushConstantDescriptionManager->registerPushConstant(pushConstant);
            pushConstants.push_back(pushConstant);
        }
        for (int i = 0; i < configuration.uniformBufferDescriptionAmount; ++i){
            UniformBuffer* buffer = new UniformBuffer(device, configuration.pUniformBufferDescription[i].bufferSize, configuration.pUniformBufferDescription[i].shaderLocation, configuration.pUniformBufferDescription[i].targetBinding, 3);
            uniformBuffers.push_back(buffer);
        }
        for (int i = 0; i < configuration.samplerDescAmount; ++i){
            VulkanSampler* sampler = new VulkanSampler(device, configuration.pSamplerDescriptions[i].binding);
            textureSamplers.push_back(sampler);
        }
    }

    void loadGraphicsPipeline(VulkanShader* shader, PipelineConfiguration::PipelineConfigInfo& configInfo){
        std::vector<IDescriptorLayoutObject*> descriptorLayoutObjects;
        for (int i = 0; i < uniformBuffers.size(); ++i){
            descriptorLayoutObjects.push_back(uniformBuffers[i]);
        }
        for (int i = 0; i < textureSamplers.size(); ++i){
            descriptorLayoutObjects.push_back(textureSamplers[i]);
        }
        graphicsPipeline = new GraphicsPipeline(shader, device, configInfo, vertexBufferDescriptionManager, pushConstantDescriptionManager, descriptorLayoutObjects.data(), descriptorLayoutObjects.size());
    }

    void loadDescriptors(){
        std::vector<IDescriptorLayoutObject*> descriptorLayoutObjects;
        for (int i = 0; i < uniformBuffers.size(); ++i){
            descriptorLayoutObjects.push_back(uniformBuffers[i]);
        }
        for (int i = 0; i < textureSamplers.size(); ++i){
            descriptorLayoutObjects.push_back(textureSamplers[i]);
        }
        descriptors = new VulkanDescriptors(device, graphicsPipeline->getDescriptorSetLayout(), 3, descriptorLayoutObjects.data(), descriptorLayoutObjects.size());
    }
    virtual void invokeCurrentRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) = 0;
    virtual void invokeCurrentPreRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) = 0;
public:
    void invokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {
        pushConstantDescriptionManager->loadConstantsToShader(commandBuffer, layout);

        std::vector<IDescriptorLayoutObject*> descriptorLayoutObjects;
        for (int i = 0; i < uniformBuffers.size(); ++i){
            descriptorLayoutObjects.push_back(uniformBuffers[i]);
        }
        for (int i = 0; i < textureSamplers.size(); ++i){
            descriptorLayoutObjects.push_back(textureSamplers[i]);
        }
        descriptors->writeDescriptor(descriptorLayoutObjects.data(), descriptorLayoutObjects.size(), imageIndex);
        invokeCurrentRender(commandBuffer, layout, imageIndex);
    }

    void invokePreRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {
        invokeCurrentPreRender(commandBuffer, layout, imageIndex);
    }

    virtual void update(){
        renderingPipeline->redrawCommandBuffers();;
    }


};


