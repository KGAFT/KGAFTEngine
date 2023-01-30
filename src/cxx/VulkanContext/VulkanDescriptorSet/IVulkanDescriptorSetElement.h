#include <vulkan/vulkan.h>
#pragma once
class IVulkanDescriptorSetElement {
protected:
	VkDescriptorSetLayoutBinding description;
public:
	IVulkanDescriptorSetElement(unsigned int binding, VkShaderStageFlags targetShaders, VkDescriptorType elementType) {
		description = {};
		description.descriptorType = elementType;
		description.binding = binding;
		description.descriptorCount = 1;
		description.pImmutableSamplers = nullptr;
		description.stageFlags = targetShaders;
	}
	VkDescriptorSetLayoutBinding getDescription() {
		return this->description;
	}
};