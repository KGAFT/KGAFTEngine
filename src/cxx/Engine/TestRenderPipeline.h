//
// Created by Daniil on 02.02.2023.
//
#pragma once

#include "RenderPipeline.h"
#include "../Camera/CameraManager.h"

class TestRenderPipeline : RenderPipeline{
public:
    TestRenderPipeline(VulkanDevice* device, Window* window) : RenderPipeline(device, window) {
        VulkanShader* shader = VulkanShader::loadShaderBlock("spir-v", device);
        ShaderInputDataConfiguration configuration{};
        configuration.shader = shader;
        SamplerDescription samplerDescription;
        samplerDescription.binding = 1;
        std::vector<VertexBufferDescription> vboDescriptions;
        vboDescriptions.push_back({0, 3, 0});
        vboDescriptions.push_back({1, 3, 3*sizeof(float)});
        vboDescriptions.push_back({2, 2, 6*sizeof(float)});
        UniformBufferDescription uboDesc{0, sizeof(PushConstantData), VK_SHADER_STAGE_VERTEX_BIT};
        PushConstantDescription pcDesc{VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstantData)};
        configuration.samplerDescAmount = 1;
        configuration.pushConstantDescriptionAmount = 1;
        configuration.vertexBufferDescriptionAmount = 3;
        configuration.uniformBufferDescriptionAmount = 1;
        configuration.pVertexBufferDescription = vboDescriptions.data();
        configuration.pushConstantDescription = &pcDesc;
        configuration.pSamplerDescriptions = &samplerDescription;
        configuration.pUniformBufferDescription = &uboDesc;

        load(configuration);
    }
    void update(){
        RenderPipeline::update();
    }
private:
    void invokeCurrentRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {

    }

    void
    invokeCurrentPreRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {

    }

};