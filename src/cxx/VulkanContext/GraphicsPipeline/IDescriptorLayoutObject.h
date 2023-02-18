//
// Created by Daniil on 02.02.2023.
//

#include <vulkan/vulkan.h>

#pragma once

class IDescriptorLayoutObject {
protected:
    VkDescriptorSetLayoutBinding descriptorSetLayoutBind;
public:
    IDescriptorLayoutObject(unsigned int binding, VkDescriptorType type, VkShaderStageFlags shaderStages) {
        descriptorSetLayoutBind = {};
        descriptorSetLayoutBind.binding = binding;
        descriptorSetLayoutBind.descriptorType = type;
        descriptorSetLayoutBind.descriptorCount = 1;
        descriptorSetLayoutBind.stageFlags = shaderStages;
    }

    VkDescriptorSetLayoutBinding getDescriptorSetLayoutBind() {
        return descriptorSetLayoutBind;
    }

    VkDescriptorSetLayoutBinding *getPDescriptorSetLayoutBind() {
        return &descriptorSetLayoutBind;
    }

    virtual VkBuffer getBuffer(unsigned int number) = 0;

    virtual VkSampler getSampler() = 0;

    virtual VkImageView getSamplerImageView() = 0;

    virtual size_t getBufferSize() = 0;
};