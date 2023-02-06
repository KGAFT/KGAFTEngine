#pragma once
#include "VulkanImage.h"
#include "../VulkanDevice/VulkanDevice.h"

class VulkanTexture
{
    friend class VulkanSampler;
private:
	VulkanImage* image;
	VulkanDevice* device;
	VkImageView imageView;
    
public:
	VulkanTexture(VulkanImage* image, VulkanDevice* device) {
		this->image = image;
		this->device = device;
		imageView = device->createImageView(image->image, VK_FORMAT_R8G8B8A8_SRGB);
	}

	~VulkanTexture() {
		destroy();
	}

	void destroy() {
		vkDestroyImageView(device->getDevice(), imageView, nullptr);

	}
private:
    
};

