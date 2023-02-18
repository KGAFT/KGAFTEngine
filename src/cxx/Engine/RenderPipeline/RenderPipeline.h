//
// Created by Daniil on 06.02.2023.
//
#pragma once

#include "../../VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "../../VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "../../VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "../../VulkanContext/VulkanBuffers/VertexBufferDescriptionManager.h"
#include "../../VulkanContext/VulkanImmediateShaderData/PushConstants/PushConstant.h"
#include "../../VulkanContext/VulkanImmediateShaderData/PushConstants/PushConstantDescriptionManager.h"
#include "../../VulkanContext/GraphicsPipeline/PipelineConfig.h"
#include "../../VulkanContext/VulkanImage/VulkanSampler.h"
#include "../../VulkanContext/VulkanImmediateShaderData/UniformBuffers/UniformBuffer.h"
#include "../../VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "../../VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"
#include "../../VulkanContext/VulkanDescriptor/VulkanDescritptors.h"
#include "ShaderInputDataConfiguration.h"

class RenderPipeline {
protected:
    VulkanDevice *device;
    VulkanSwapChain *swapChain;
    VulkanRenderPass *renderPass;
    VulkanSwapChainControl *swapChainControl;
    VertexBufferDescriptionManager *vertexBufferDescriptionManager;
    PushConstantDescriptionManager *pushConstantDescriptionManager;
    PipelineConfiguration::PipelineConfigInfo pipelineConfig;
    GraphicsPipeline *pipeline;
    VulkanRenderingPipeline *renderingPipeline;
    VulkanDescriptors *descriptors;

    std::vector<VulkanSampler *> samplers;
    std::vector<PushConstant *> pushConstants;
    std::vector<UniformBuffer *> uniformBuffers;


protected:
    RenderPipeline(VulkanDevice *device) : device(device) {


    }

    void load(Window *window, ShaderInputDataConfiguration &config) {
        swapChain = new VulkanSwapChain(device, window->getWidth(), window->getHeight());
        renderPass = new VulkanRenderPass(device, swapChain);
        swapChainControl = new VulkanSwapChainControl(swapChain, renderPass, device);
        vertexBufferDescriptionManager = new VertexBufferDescriptionManager();
        populateVertexBuffersDescription(config);
        pushConstantDescriptionManager = new PushConstantDescriptionManager();
        populatePushConstantsManager(config);

        pipelineConfig = PipelineConfiguration::defaultPipelineConfigInfo(window->getWidth(), window->getHeight());
        pipelineConfig.renderPass = renderPass->getRenderPass();
        populateUniforms(config);

        std::vector<IDescriptorLayoutObject *> objectsToWrite;
        for (VulkanSampler *sampler: samplers) {
            objectsToWrite.push_back(sampler);
        }
        for (const auto item: uniformBuffers) {
            objectsToWrite.push_back(item);
        }

        pipeline = new GraphicsPipeline(config.shader, device, pipelineConfig, vertexBufferDescriptionManager,
                                        pushConstantDescriptionManager, objectsToWrite.data(), objectsToWrite.size());

        renderingPipeline = new VulkanRenderingPipeline(swapChainControl, renderPass, pipeline, device);

        window->registerResizeCallback(renderingPipeline);

        descriptors = new VulkanDescriptors(device, pipeline->getDescriptorSetLayout(), 3, objectsToWrite.data(),
                                            objectsToWrite.size());

    }

    void beginRender() {
        renderingPipeline->beginRender();
    }

    void beginDraw() {
        renderingPipeline->beginDraw();

    }

    void updateShaderData() {
        std::vector<IDescriptorLayoutObject *> objectsToWrite;
        for (VulkanSampler *sampler: samplers) {
            objectsToWrite.push_back(sampler);
        }
        for (const auto item: uniformBuffers) {
            objectsToWrite.push_back(item);
        }
        pushConstantDescriptionManager->loadConstantsToShader(renderingPipeline->getCurrentCommandBuffer(),
                                                              renderingPipeline->getPipelineLayout());
        descriptors->writeDescriptor(objectsToWrite.data(), objectsToWrite.size(),
                                     renderingPipeline->getCurrentImage());
        descriptors->bind(renderingPipeline->getCurrentImage(), renderingPipeline->getCurrentCommandBuffer(),
                          renderingPipeline->getPipelineLayout());
    }

    void endRender() {
        renderingPipeline->endDraw();
    }

    void setClearColorValues(float r, float g, float b, float a) {
        renderingPipeline->setClearColorValues(r, g, b, a);
    }


    void destroy() {
        vkDeviceWaitIdle(device->getDevice());
        delete swapChain;
        delete renderPass;
        VkDescriptorSetLayout layout = pipeline->getDescriptorSetLayout();
        delete pipeline;
        for (const auto &item: uniformBuffers) {
            delete item;
        }
        uniformBuffers.clear();
        for (const auto &item: pushConstants) {
            delete item;
        }
        pushConstants.clear();
        delete descriptors;
        for (const auto &item: samplers) {
            delete item;
        }
        vkDestroyDescriptorSetLayout(device->getDevice(), layout, nullptr);
        delete swapChainControl;
        delete renderingPipeline;
        delete vertexBufferDescriptionManager;
        delete pushConstantDescriptionManager;
    }

private:
    void populateVertexBuffersDescription(ShaderInputDataConfiguration &config) {
        size_t stepSize = 0;
        for (int i = 0; i < config.vertexBufferDescriptionAmount; ++i) {
            vertexBufferDescriptionManager->registerAttribute(0, config.pVertexBufferDescription[i].location,
                                                              config.pVertexBufferDescription[i].vertexCoordinatesAmount,
                                                              config.pVertexBufferDescription[i].offset);
            stepSize += config.pVertexBufferDescription[i].vertexCoordinatesAmount * sizeof(float);
        }
        vertexBufferDescriptionManager->createBinding(0, stepSize);
    }

    void populatePushConstantsManager(ShaderInputDataConfiguration &config) {
        for (int i = 0; i < config.pushConstantDescriptionAmount; ++i) {
            pushConstants.push_back(new PushConstant(config.pushConstantDescription[i].constantSize,
                                                     config.pushConstantDescription[i].shaderStages));
            pushConstantDescriptionManager->registerPushConstant(pushConstants[i]);
        }
    }

    void populateUniforms(ShaderInputDataConfiguration &config) {
        for (int i = 0; i < config.uniformBufferDescriptionAmount; ++i) {
            uniformBuffers.push_back(new UniformBuffer(device, config.pUniformBufferDescription[i].bufferSize,
                                                       config.pUniformBufferDescription[i].shaderLocation,
                                                       config.pUniformBufferDescription[i].targetBinding, 3));
        }
        for (int i = 0; i < config.samplerDescAmount; ++i) {
            VulkanSampler *sampler = new VulkanSampler(device, config.pSamplerDescriptions[i].binding);
            samplers.push_back(sampler);
        }
    }
};