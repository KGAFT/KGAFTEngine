#include <vulkan/vulkan.h>
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"

#pragma once

class VulkanDescriptorSet {
private:
    static VkWriteDescriptorSet prepareWrite(VulkanDescriptorSet* set, IVulkanDescriptorSetElement* descElement, unsigned int binding){
        VkDescriptorBufferInfo bufferInfo{};
        VkDescriptorImageInfo imageInfo{};
        if (descElement->getPDescription()->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
            bufferInfo.buffer = descElement->getBufferToWrite();
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(descElement->getBufferSize());
        }
        else {
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = descElement->getSamplerImageView();
            imageInfo.sampler = descElement->getSampler();
        }


        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = set->descriptorSet;
        descriptorWrite.dstBinding = binding;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.descriptorType = descElement->getPDescription()->descriptorType;
        if (descElement->getPDescription()->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
            descriptorWrite.pBufferInfo = &bufferInfo;
        }
        else {
            descriptorWrite.pImageInfo = &imageInfo;
        }
    }
public:
    static void writeDescriptors(VulkanDescriptorSet* set, std::map<IVulkanDescriptorSetElement*, unsigned int> descriptors){
        std::vector<VkWriteDescriptorSet> writes;
        for (const auto &item: descriptors){
            writes.push_back(prepareWrite(set, item.first, item.second));
        }
        vkUpdateDescriptorSets(set->device->getDevice(), writes.size(), writes.data(), 0, nullptr);
    }
private:
	VkDescriptorSet descriptorSet;
	VkDescriptorSetLayout layout;
	VulkanDevice* device;
public:
	VulkanDescriptorSet(VulkanDevice* device, VulkanDescriptorPool* descriptorPool, VulkanDescriptorSetLayout* layout) {
		this->device = device;
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool->getDescriptorPool();
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = layout->getLayout();
		VkResult result = vkAllocateDescriptorSets(device->getDevice(), &allocInfo, &descriptorSet);
		if (result != VK_SUCCESS) {
			if (result == VK_ERROR_OUT_OF_HOST_MEMORY) {
				std::cout << "HM" << std::endl;
			}
			if (result == VK_ERROR_OUT_OF_DEVICE_MEMORY) {
				std::cout << "DM" << std::endl;
			}
			if (result == VK_ERROR_FRAGMENTED_POOL) {
				std::cout << "FP" << std::endl;
			}
			if (result == VK_ERROR_OUT_OF_POOL_MEMORY) {
				std::cout << "OFP" << std::endl;
			}
			throw std::runtime_error("failed to allocate descriptor set!");
		}
	}
	void write(IVulkanDescriptorSetElement* descElement, unsigned int binding, unsigned int arrayElement = 0) {
		VkDescriptorBufferInfo bufferInfo{};
		VkDescriptorImageInfo imageInfo{};
		if (descElement->getPDescription()->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
			bufferInfo.buffer = descElement->getBufferToWrite();
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(descElement->getBufferSize());
		}
		else {
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = descElement->getSamplerImageView();
			imageInfo.sampler = descElement->getSampler();
		}


		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = arrayElement;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = descElement->getPDescription()->descriptorType;
		if (descElement->getPDescription()->descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
			descriptorWrite.pBufferInfo = &bufferInfo;
		}
		else {
			descriptorWrite.pImageInfo = &imageInfo;
		}
        VkWriteDescriptorSet writes[]{descriptorWrite};
		vkUpdateDescriptorSets(device->getDevice(), 1, writes, 0, nullptr);
	}
	void bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) {
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	}
};