#include "../GraphicsPipeline/IDescriptorLayoutObject.h"
#include "VulkanTexture.h"

#pragma once

class VulkanSampler : public IDescriptorLayoutObject {
private:
    VulkanTexture *texture = nullptr;
    VulkanDevice *device;
    VkSampler sampler;
public:
    VulkanSampler(VulkanDevice *device, unsigned int binding) : IDescriptorLayoutObject(binding,
                                                                                        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                                        VK_SHADER_STAGE_FRAGMENT_BIT) {
        this->device = device;
        createTextureSampler();

    }

    void destroy() {
        vkDestroySampler(device->getDevice(), sampler, nullptr);
    }

    VkBuffer getBuffer(unsigned int number) override {
        return nullptr;
    }

    VkSampler getSampler() override {
        return sampler;
    }

    VkImageView getSamplerImageView() override {
        if (texture == nullptr) {
            return VK_NULL_HANDLE;
        }
        return texture->imageView;
    }

    size_t getBufferSize() override {
        return 0;
    }

    void setTexture(VulkanTexture *texture) {
        VulkanSampler::texture = texture;
    }

private:

    void createTextureSampler() {
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(device->getDeviceToCreate(), &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        if (vkCreateSampler(device->getDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
    }
};

