//
// Created by daniil on 28.01.23.
//
#include <vector>
#include <vulkan/vulkan.h>
#include <stdexcept>
#pragma once

class VertexBufferDescriptionManager{
private:
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    std::vector<VkVertexInputBindingDescription> bindingDescription;
public:
    void createBinding(int binding, size_t perStepSize){
        VkVertexInputBindingDescription bindDesc{};
        bindDesc.binding = binding;
        bindDesc.stride = perStepSize;
        bindDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescription.push_back(bindDesc);
    }
    void registerAttribute(int binding, int location, unsigned int vertexCoordsAmount, size_t offset){
        bool exist = false;
        for (const auto &item: bindingDescription){
            if(item.binding==binding){
                exist = true;
            }
        }
        if(!exist){
            throw std::runtime_error("Error: you cannot register attribute with unexisting binding");
        }

        for (const auto &item: attributeDescriptions){
            if(item.location == location){
                throw std::runtime_error("Error: you cannot register attribute with already registered location");
            }
        }
        VkVertexInputAttributeDescription attributeDescription{};
        attributeDescription.binding = binding;
        attributeDescription.offset = offset;
        attributeDescription.location = location;
        attributeDescription.format = prepareFormatForVertexCoordsAmount(vertexCoordsAmount);
        attributeDescriptions.push_back(attributeDescription);
    }

     std::vector<VkVertexInputAttributeDescription> &getAttributeDescriptions(){
        return attributeDescriptions;
    }

   std::vector<VkVertexInputBindingDescription> &getBindingDescription(){
        return bindingDescription;
    }

private:
    VkFormat prepareFormatForVertexCoordsAmount(unsigned int vertexCoordsAmount){
        switch (vertexCoordsAmount) {
            case 2:
                return VK_FORMAT_R32G32_SFLOAT;
            case 3:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case 4:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            default:
                return VK_FORMAT_R32_SFLOAT;
        }
    }

};
