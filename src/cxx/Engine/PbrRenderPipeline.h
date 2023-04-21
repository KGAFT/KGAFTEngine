#pragma once

#include "RenderPipeline/RenderPipeline.h"
#include <glm/glm.hpp>
#include "../Camera/CameraManager.h"

struct PointLight
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;
};

struct DirectLight
{
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;
};

struct LightInfo
{
    DirectLight directLights[100];
    PointLight pointLights[100];

    alignas(4) int enabledDirects;
    alignas(4) int enabledPoints;

    alignas(4) float emissiveIntensity = 2;
    alignas(4) float emissiveShininess = 1;
    alignas(4) float gammaCorrect = 1.0f / 2.2f;
    alignas(4) float ambientIntensity = 0.03f;

    alignas(4) int combinedMetallicRoughness = 1;
    alignas(4) int emissiveEnabled = 0;
    alignas(4) int aoEnabled = 0;
    int opacityEnabled = 0;
};

class PbrRenderPipeline : RenderPipeline
{
private:
    VulkanDevice *device;
    VulkanSwapChain *swapChain;
    LightInfo lightConfig{};
    bool firstFrame = true;
public:
    PbrRenderPipeline(VulkanDevice *device, VulkanSwapChain *swapChain) : RenderPipeline(device, swapChain), device(device), swapChain(swapChain)
    {
        PipelineEndConfig endConfig{};
        endConfig.vertexInputs.push_back({0, 3, sizeof(float), VK_FORMAT_R32G32B32_SFLOAT});
        endConfig.vertexInputs.push_back({1, 3, sizeof(float), VK_FORMAT_R32G32B32_SFLOAT});
        endConfig.vertexInputs.push_back({2, 2, sizeof(float), VK_FORMAT_R32G32_SFLOAT});

        endConfig.pushConstantInfos.push_back({VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstantData)});
        endConfig.uniformBuffers.push_back({0, sizeof(LightInfo), VK_SHADER_STAGE_FRAGMENT_BIT});
        for (int i = 1; i <= 8; i++)
        {
            endConfig.samplers.push_back({i, VK_SHADER_STAGE_FRAGMENT_BIT});
        }
        OutputConfig config{};
        config.amount = 0;
        config.width = Window::getWindowInstance()->getWidth();
        config.height = Window::getWindowInstance()->getHeight();
        RenderPipeline::initialize("spir-v/PbrRenderPipeline", endConfig, config);
        RenderPipeline::getUniforms()[0]->write(&lightConfig);
    }
    VkCommandBuffer beginRender(bool samplersUpdate){
        VkCommandBuffer cmd = RenderPipeline::beginRender(samplersUpdate, firstFrame);
         RenderPipeline::getUniforms()[0]->write(&lightConfig);
        firstFrame = false;
        return cmd;
    }
    void updatePushConstants(){
        RenderPipeline::updatePushConstants();
    }
    void bindImmediate(){
        RenderPipeline::bindImmediate();
    }
    void endRender(){
        RenderPipeline::endRender();
    }

    void updateSamplers(){
        RenderPipeline::updateSamplers();
    }

    VulkanPushConstant *getPushConstant(unsigned int index) {
        return RenderPipeline::getPushConstants()[index];
    }
    LightInfo& getLightInfo(){
        return lightConfig;
    }

    void setSamplerTexture(VulkanImage *texture, unsigned int binding) {
        for (const auto &item: RenderPipeline::getSamplers()) {
            if (item->getBinding()==binding) {
                item->setSamplerImageView(texture->getView());
                break;
            }
        }
    }
};