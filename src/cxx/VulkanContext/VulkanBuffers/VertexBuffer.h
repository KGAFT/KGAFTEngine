//
// Created by daniil on 27.01.23.
//
#pragma once

#include <cstring>
#include "../VulkanDevice/VulkanDevice.h"

class VertexBuffer {
private:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VulkanDevice *device;
    unsigned int verticesAmount;

public:
    VertexBuffer(size_t stepSize, unsigned int verticesAmount, VulkanDevice *device, void *data) {
        this->device = device;
        createVertexBuffers(data, stepSize, verticesAmount);
    }

    ~VertexBuffer() {
        destroy();
    }

    void destroy() {
        vkDestroyBuffer(device->getDevice(), vertexBuffer, nullptr);
        //vkFreeMemory(device->getDevice(), vertexBufferMemory, nullptr);
    }


    void draw(VkCommandBuffer commandBuffer) {
        vkCmdDraw(commandBuffer, verticesAmount, 1, 0, 0);
    }

    void bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

private:
    void createVertexBuffers(void *vertices, size_t stepSize, unsigned int verticesAmount) {
        VkDeviceSize bufferSize = stepSize * verticesAmount;
        this->verticesAmount = verticesAmount;
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        device->createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                stagingBuffer,
                stagingBufferMemory);

        void *data;
        vkMapMemory(device->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices, static_cast<size_t>(bufferSize));
        vkUnmapMemory(device->getDevice(), stagingBufferMemory);

        device->createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                vertexBuffer,
                vertexBufferMemory);

        device->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(device->getDevice(), stagingBuffer, nullptr);
        vkFreeMemory(device->getDevice(), stagingBufferMemory, nullptr);
    }
};

