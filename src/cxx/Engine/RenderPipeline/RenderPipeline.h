//
// Created by Daniil on 06.02.2023.
//
#pragma once

#include <Vulkan/VulkanEndRenderPipeline.h>
#include <Vulkan/VulkanImage/VulkanImage.h>
#include <stdexcept>
#include <Util/ShaderLoader.h>

struct OutputConfig
{
    unsigned int width;
    unsigned int height;
    unsigned int amount;
    unsigned int imagePerStepAmount;
};

class RenderPipeline
{
private:
    VulkanEndRenderPipeline *endRenderPipeline;
    VulkanSyncManager *syncManager;
    VulkanShader *shader;
    VulkanDevice *device;
    VulkanSwapChain *swapChain;
    std::vector<VulkanImage *> outputImages;

public:
    RenderPipeline(VulkanDevice *device, VulkanSwapChain *swapChain) : device(device), swapChain(swapChain)
    {
        syncManager = new VulkanSyncManager(device, swapChain);
    }

    void initialize(const char *pathToShadersDir, PipelineEndConfig &config, OutputConfig &outputConfig)
    {
        shader = ShaderLoader::loadShaders(pathToShadersDir, device);
        std::vector<VkImageView> renderTargets;
        for (unsigned int i = 0; i < outputConfig.amount; i++)
        {
            outputImages.push_back(VulkanImage::createImage(device, outputConfig.width, outputConfig.height));
            renderTargets.push_back(outputImages[outputImages.size() - 1]->getView());
        }
        if (renderTargets.size() == 0 and swapChain == nullptr)
        {
            throw std::runtime_error("Error: you need to specify output images if you haven't swapChain");
        }
        if (swapChain != nullptr)
        {
            endRenderPipeline = new VulkanEndRenderPipeline(device, syncManager, shader, &config, outputConfig.width, outputConfig.height, swapChain->getSwapChainImageViews(), 1, swapChain->getSwapChainImageFormat());
        }
        else
        {
            endRenderPipeline = new VulkanEndRenderPipeline(device, syncManager, shader, &config, outputConfig.width, outputConfig.height, renderTargets, outputConfig.imagePerStepAmount, outputImages[0]->getFormat());
        }
    }

    std::vector<VulkanSampler*>& getSamplers(){
        return endRenderPipeline->getSamplers();
    }
    std::vector<VulkanUniformBuffer*>& getUniforms(){
        return endRenderPipeline->getUniformBuffers();
    }
    std::vector<VulkanPushConstant*>& getPushConstants(){
        return endRenderPipeline->getPushConstants();
    }

    VkCommandBuffer beginRender(bool isSamplersUpdate, bool isNewUniformBuffers)
    {
        if (isSamplersUpdate)
        {
            endRenderPipeline->updateSamplers();
        }
        if (isNewUniformBuffers)
        {
            endRenderPipeline->updateUniforms();
        }
        VkCommandBuffer cmd = endRenderPipeline->beginRender();
        if (endRenderPipeline->getPushConstants().size() > 0)
        {
            endRenderPipeline->updatePcs();
        }
        if (endRenderPipeline->getUniformBuffers().size() > 0 or endRenderPipeline->getSamplers().size() > 0)
        {
            endRenderPipeline->bindImmediate();
        }
        return cmd;
    }

    void updateSamplers()
    {
        endRenderPipeline->updateSamplers();
    }

    void endRender()
    {
        endRenderPipeline->endRender();
    }
};