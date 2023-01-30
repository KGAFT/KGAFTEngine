#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include "../VulkanDevice/VulkanDevice.h"

#pragma once
class VulkanDescriptorPool
{
private:
	VkDescriptorPool descriptorPool;
	VulkanDevice* device;
public:
	VulkanDescriptorPool(VulkanDevice* device, std::vector<VkDescriptorPoolSize> sizes) {
		this->device = device;
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = sizes.size();
		poolInfo.pPoolSizes = sizes.data();
		poolInfo.maxSets = sizes.size();
		if (vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	~VulkanDescriptorPool() {
		destroy();
	}

	VkDescriptorPool getDescriptorPool() {
		return descriptorPool;
	}
	void destroy() {
		vkDestroyDescriptorPool(device->getDevice(), descriptorPool, nullptr);
	}
};

