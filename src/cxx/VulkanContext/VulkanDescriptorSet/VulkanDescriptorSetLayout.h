#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include "IVulkanDescriptorSetElement.h"
#include "../VulkanDevice/VulkanDevice.h"
#pragma once
class VulkanDescriptorSetLayout
{
private:
	VulkanDevice* device;
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector< VkDescriptorSetLayoutBinding> bindings;
	void loadBindings(std::vector<IVulkanDescriptorSetElement*>& descriptorElements) {
		for (IVulkanDescriptorSetElement* el : descriptorElements) {
			bindings.push_back(el->getDescription());
		}
	}
public:
	VulkanDescriptorSetLayout(VulkanDevice* device, std::vector<IVulkanDescriptorSetElement*>& descriptorElements) {
		this->device = device;
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		loadBindings(descriptorElements);
		layoutInfo.pBindings = bindings.data();
		layoutInfo.bindingCount = bindings.size();
		layoutInfo.pNext = nullptr;
		if (vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set descriptorSetLayout!");
		}
	}
	VkDescriptorSetLayout* getLayout() {
		return &descriptorSetLayout;
	}
	void destroy() {
		vkDestroyDescriptorSetLayout(device->getDevice(), descriptorSetLayout, nullptr);
	}
};

