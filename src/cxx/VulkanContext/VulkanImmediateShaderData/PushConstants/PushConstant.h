//
// Created by daniil on 28.01.23.
//
#pragma once
#include <vulkan/vulkan.h>


#define VERTEX_SHADER_PUSH_CONSTANT VK_SHADER_STAGE_VERTEX_BIT
#define FRAGMENT_SHADER_PUSH_CONSTANT VK_SHADER_STAGE_FRAGMENT_BIT
#define GEOMETRY_SHADER_PUSH_CONSTANT VK_SHADER_STAGE_GEOMETRY_BIT

class PushConstant{
    friend class PushConstantDescriptionManager;
private:
    size_t size;
    VkShaderStageFlags shaderStages;
    void* data;
public:
    PushConstant(size_t size, VkShaderStageFlags shaderStages) : size(size), shaderStages(shaderStages) {
        
    }

    size_t getSize(){
        return size;
    }
    VkShaderStageFlags getShaderStages(){
        return shaderStages;
    }

    void *getData() {
        return data;
    }

    void setData(void *data) {
        PushConstant::data = data;
    }
};
