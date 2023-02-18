//
// Created by Daniil on 02.02.2023.
//
#include  <vulkan/vulkan.h>
#include "../../VulkanContext/GraphicsPipeline/VulkanShader.h"

#pragma once


struct UniformBufferDescription {
    unsigned int targetBinding;
    size_t bufferSize;
    VkShaderStageFlags shaderLocation;
};

struct PushConstantDescription {
    VkShaderStageFlags shaderStages;
    size_t constantSize;
};

struct VertexBufferDescription {
    unsigned int location;
    unsigned int vertexCoordinatesAmount;
    size_t offset;
};

struct SamplerDescription {
    unsigned int binding;
};

struct ShaderInputDataConfiguration {
    SamplerDescription *pSamplerDescriptions;
    UniformBufferDescription *pUniformBufferDescription;
    VertexBufferDescription *pVertexBufferDescription;
    PushConstantDescription *pushConstantDescription;

    VulkanShader *shader;

    unsigned int samplerDescAmount = 0;
    unsigned int uniformBufferDescriptionAmount = 0;
    unsigned int pushConstantDescriptionAmount = 0;
    unsigned vertexBufferDescriptionAmount = 0;

};