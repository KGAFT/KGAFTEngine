#include <vulkan/vulkan.h>
#include "../../VulkanDescriptorSet/IVulkanDescriptorSetElement.h"
#include "../../VulkanDevice/VulkanDevice.h"
#pragma once
class UniformBuffer : public IVulkanDescriptorSetElement
{
private:
	VkBuffer buffer;
	VulkanDevice* device;
	VkDeviceMemory memory;
	size_t bufferSize;
	void* memoryMap;
public:
	UniformBuffer(VulkanDevice* device, size_t bufferSize, unsigned int binding, VkShaderStageFlags targetShaders) : device(device), bufferSize(bufferSize), IVulkanDescriptorSetElement(binding, targetShaders, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) {
		device->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, buffer, memory);
		vkMapMemory(device->getDevice(), memory, 0, bufferSize, 0, &memoryMap);
	}

	~UniformBuffer() {
		destroy();
	}

	void write(void* data) {
		memcpy(memoryMap, data, bufferSize);
	}


	void destroy() {
		vkDestroyBuffer(device->getDevice(), buffer, nullptr);
		vkFreeMemory(device->getDevice(), memory, nullptr);
	}

	 VkBuffer getBufferToWrite() override {
		 return buffer;
	 }
	 size_t getBufferSize() override {
		 return bufferSize;
	 }
	 VkSampler getSampler() override {
		 return VK_NULL_HANDLE;
	 }
	 virtual VkImageView getSamplerImageView() {
		 return VK_NULL_HANDLE;
	 }
};

