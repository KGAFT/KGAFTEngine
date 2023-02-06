//
// Created by Daniil on 02.02.2023.
//

#include <vulkan/vulkan.h>
#include <vector>
#include "../GraphicsPipeline/IDescriptorLayoutObject.h"
#include "../VulkanDevice/VulkanDevice.h"

#pragma once

class VulkanDescriptors{
private:
    VkDescriptorPool descriptorPool;
    VulkanDevice* device;
    std::vector<VkDescriptorSet> descriptorSets;
public:
    VulkanDescriptors(VulkanDevice* device, VkDescriptorSetLayout layout, unsigned int instanceCount, IDescriptorLayoutObject** objects, unsigned int objectCount){
        this->device = device;
        std::vector<VkDescriptorPoolSize> sizes;
        for (int i = 0; i < objectCount; ++i){
            VkDescriptorPoolSize poolSize{};
            poolSize.type = objects[i]->getPDescriptorSetLayoutBind()->descriptorType;
            poolSize.descriptorCount = instanceCount;
        }
        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = sizes.size();
        poolInfo.pPoolSizes = sizes.data();
        poolInfo.maxSets = instanceCount;

        if (vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
        createDescriptorSets(layout, instanceCount);
    }

    ~VulkanDescriptors(){
        destroy();
    }

    void destroy(){
        vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);
    }
    void writeAllDescriptors(IDescriptorLayoutObject* objects, unsigned int objectCount){
        for (int i = 0; i < descriptorSets.size(); ++i){
            std::vector<VkWriteDescriptorSet> writes;
            std::vector<std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo>*> infos;
            for (int oc = 0; oc < objectCount; ++oc){
                std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo>* info = new std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo>(getChildOfObject(objects[oc], i));
                VkWriteDescriptorSet write{};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = descriptorSets[i];
                write.dstBinding = 0;
                write.dstArrayElement = 0;
                write.descriptorType = objects[oc].getPDescriptorSetLayoutBind()->descriptorType;
                write.descriptorCount = 1;
                if(objects[oc].getPDescriptorSetLayoutBind()->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER){
                    write.pBufferInfo = &info->first;
                }
                else{
                    write.pImageInfo = &info->second;
                }
                writes.push_back(write);
                infos.push_back(info);
            }
            vkUpdateDescriptorSets(device->getDevice(), writes.size(), writes.data(), 0, nullptr);
            for (const auto &item: infos){
                delete item;
            }

        }
    }
    void writeDescriptor(IDescriptorLayoutObject** objects, unsigned int objectCount, unsigned int currentInstance){
        std::vector<VkWriteDescriptorSet> writes;
        std::vector<std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo>*> infos;
        for (int oc = 0; oc < objectCount; ++oc){
            std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo>* info = new std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo>(getChildOfObject(*objects[oc], currentInstance));
            VkWriteDescriptorSet write{};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = descriptorSets[currentInstance];
            write.dstBinding = objects[oc]->getPDescriptorSetLayoutBind()->binding;
            write.dstArrayElement = 0;
            write.descriptorType = objects[oc]->getPDescriptorSetLayoutBind()->descriptorType;
            write.descriptorCount = 1;
            if(objects[oc]->getPDescriptorSetLayoutBind()->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER){
                write.pBufferInfo = &info->first;
            }
            else{
                write.pImageInfo = &info->second;
            }
            writes.push_back(write);
            infos.push_back(info);
        }
        vkUpdateDescriptorSets(device->getDevice(), writes.size(), writes.data(), 0, nullptr);
        for (const auto &item: infos){
            delete item;
        }
    }
    void bind(unsigned int instanceNumber, VkCommandBuffer commandBuffer, VkPipelineLayout layout){
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSets[instanceNumber], 0, nullptr);
    }
private:
    std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo> getChildOfObject(IDescriptorLayoutObject& object, unsigned int currentInstance){
        std::pair<VkDescriptorBufferInfo, VkDescriptorImageInfo> result;
        if(object.getPDescriptorSetLayoutBind()->descriptorType==VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER){
            result.second = {};
            result.second.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            result.second.imageView = object.getSamplerImageView();
            result.second.sampler = object.getSampler();
        }
        else{
            result.first = {};
            result.first.buffer = object.getBuffer(currentInstance);
            result.first.offset = 0;
            result.first.range = object.getBufferSize();
        }
        return result;
    }
private:
    void createDescriptorSets(VkDescriptorSetLayout layout, unsigned int instanceCount){
        std::vector<VkDescriptorSetLayout> layouts(instanceCount, layout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = instanceCount;
        allocInfo.pSetLayouts = layouts.data();
        descriptorSets.resize(instanceCount);
        if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
    }
};