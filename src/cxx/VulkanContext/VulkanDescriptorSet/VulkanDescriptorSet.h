#include <vulkan/vulkan.h>
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSetLayout.h"

#pragma once

class VulkanDescriptorSet {
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
		if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor set!");
		}
	}
	void write(IVulkanDescriptorSetElement* descElement, unsigned int binding, unsigned int arrayElement = 0) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = descElement->getBufferToWrite();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(descElement->getBufferSize());

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = arrayElement;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = descElement->getPDescription()->descriptorType;
		descriptorWrite.pBufferInfo = &bufferInfo;

		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device->getDevice(), 1, &descriptorWrite, 0, nullptr);
	}
	void bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) {
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
	}
	
};