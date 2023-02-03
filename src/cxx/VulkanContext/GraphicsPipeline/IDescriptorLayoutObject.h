//
// Created by Daniil on 02.02.2023.
//

#include <vulkan/vulkan.h>

#pragma once

class IDescriptorLayoutObject{
protected:
    VkDescriptorSetLayoutBinding  descriptorSetLayoutBind;
    unsigned int nativeType;
public:
    IDescriptorLayoutObject(unsigned int binding, unsigned int type, VkShaderStageFlags shaderStages){
        descriptorSetLayoutBind = {};
        descriptorSetLayoutBind.binding = binding;
        descriptorSetLayoutBind.descriptorType = VkDescriptorType(type);
        descriptorSetLayoutBind.descriptorCount = 1;
        descriptorSetLayoutBind.stageFlags = shaderStages;
        this->nativeType = type;
    }

    VkDescriptorSetLayoutBinding getDescriptorSetLayoutBind(){
        return descriptorSetLayoutBind;
    }
    VkDescriptorSetLayoutBinding* getPDescriptorSetLayoutBind(){
        return &descriptorSetLayoutBind;
    }

    unsigned int getNativeType() const {
        return nativeType;
    }

    virtual VkBuffer getBuffer(unsigned int number) = 0;
    virtual VkSampler getSampler() = 0;
    virtual VkImageView getSamplerImageView() = 0;
    virtual size_t getBufferSize() = 0;
};