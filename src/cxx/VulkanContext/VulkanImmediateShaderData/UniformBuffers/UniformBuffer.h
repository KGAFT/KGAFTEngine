//
// Created by Daniil on 02.02.2023.
//

#include <vector>
#include "../../GraphicsPipeline/IDescriptorLayoutObject.h"
#include "../../VulkanDevice/VulkanDevice.h"

#pragma once


class UniformBuffer : public IDescriptorLayoutObject {
private:
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void *> uniformBuffersMapped;
    VulkanDevice *device;
    size_t size;
public:
    UniformBuffer(VulkanDevice *device, size_t bufferSize, VkShaderStageFlags targetShaders, unsigned int binding,
                  unsigned int instanceCount) : IDescriptorLayoutObject(binding, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                                                        targetShaders) {
        this->device = device;
        this->size = bufferSize;
        uniformBuffers.resize(instanceCount);
        uniformBuffersMemory.resize(instanceCount);
        uniformBuffersMapped.resize(instanceCount);

        for (size_t i = 0; i < instanceCount; i++) {
            device->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                 uniformBuffers[i], uniformBuffersMemory[i]);
            vkMapMemory(device->getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
        }
    }

    ~UniformBuffer() {
        destroy();
    }

    void destroy() {
        for (int i = 0; i < uniformBuffers.size(); ++i) {
            vkDestroyBuffer(device->getDevice(), uniformBuffers[i], nullptr);
            vkFreeMemory(device->getDevice(), uniformBuffersMemory[i], nullptr);
        }
    }

    void write(void *data) {
        for (const auto &item: uniformBuffersMapped) {
            memcpy(item, data, size);
        }
    }

    VkBuffer getBuffer(unsigned int number) override {
        return uniformBuffers[number];
    }

    VkSampler getSampler() override {
        return nullptr;
    }

    VkImageView getSamplerImageView() override {
        return nullptr;
    }

    size_t getBufferSize() override {
        return size;
    }
};
