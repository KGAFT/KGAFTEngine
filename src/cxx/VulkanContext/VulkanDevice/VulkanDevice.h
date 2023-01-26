//
// Created by daniil on 24.01.23.
//
#include <vulkan/vulkan.h>
#include <map>
#include "DeviceSuitability.h"
#include "../../Window/Window.h"

#pragma once


class VulkanDevice {
public:
    static std::map<VkPhysicalDevice, VkPhysicalDeviceProperties>
    enumerateSupportedDevices(VkInstance instance, Window *window) {
        std::map<VkPhysicalDevice, VkPhysicalDeviceProperties> result;
        unsigned int deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            return {};
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (VkPhysicalDevice device: devices) {
            if (DeviceSuitability::isDeviceSuitable(device, window->getWindowSurface(instance))) {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(device, &properties);
                result.insert({device, properties});
            }
        }
        return result;
    }


private:
    VkPhysicalDevice deviceToCreate;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    Window *windowInstance;
    VkInstance vkInstance;
    VkCommandPool commandPool;
public:
    VulkanDevice(VkPhysicalDevice deviceToCreate, Window *windowInstance, VkInstance vkInstance, bool logDevice)
            : deviceToCreate(deviceToCreate), windowInstance(windowInstance), vkInstance(vkInstance) {
        if (!DeviceSuitability::isDeviceSuitable(deviceToCreate, windowInstance->getWindowSurface(vkInstance))) {
            throw std::runtime_error("Error, you cannot create unsupported vulkan device");
        }
        createLogicalDevice(logDevice);
        createCommandPool();
    }

    void destroy() {
        vkDestroyCommandPool(device, commandPool, nullptr);
        vkDestroyDevice(device, nullptr);

    }

    ~VulkanDevice() {
        destroy();
    }

    void createImageWithInfo(
            const VkImageCreateInfo &imageInfo,
            VkMemoryPropertyFlags properties,
            VkImage &image,
            VkDeviceMemory &imageMemory) {
        if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
        }

        if (vkBindImageMemory(device, image, imageMemory, 0) != VK_SUCCESS) {
            throw std::runtime_error("failed to bind image memory!");
        }
    }

    VkFormat findSupportedFormat(
            const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format: candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(deviceToCreate, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            } else if (
                    tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }
        throw std::runtime_error("failed to find supported format!");
    }


    unsigned int findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(deviceToCreate, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }


    VkPhysicalDevice getDeviceToCreate() {
        return deviceToCreate;
    }

    VkDevice getDevice() {
        return device;
    }

    VkQueue getGraphicsQueue() {
        return graphicsQueue;
    }

    VkQueue getPresentQueue() {
        return presentQueue;
    }

    Window *getWindowInstance() {
        return windowInstance;
    }

    VkInstance getVkInstance() {
        return vkInstance;
    }

    VkCommandPool getCommandPool() {
        return commandPool;
    }

private:
    void createLogicalDevice(bool logDevice) {
        DeviceSuitability::QueueFamilyIndices indices = DeviceSuitability::findQueueFamilies(deviceToCreate,
                                                                                             windowInstance->getWindowSurface(
                                                                                                     vkInstance));

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily: uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceSuitability::deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = DeviceSuitability::deviceExtensions.data();

        if (logDevice) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanLogger::validationLayers.size());
            createInfo.ppEnabledLayerNames = VulkanLogger::validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(deviceToCreate, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
    }

    void createCommandPool() {
        DeviceSuitability::QueueFamilyIndices queueFamilyIndices = DeviceSuitability::findQueueFamilies(deviceToCreate,
                                                                                                        windowInstance->getWindowSurface(
                                                                                                                vkInstance));

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        poolInfo.flags =
                VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

};
