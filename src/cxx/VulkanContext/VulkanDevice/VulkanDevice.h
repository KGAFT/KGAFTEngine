//
// Created by daniil on 24.01.23.
//
#include <vulkan/vulkan.h>
#include <map>
#include "DeviceSuitability.h"
#include "../../Window/Window.h"

#pragma once


class VulkanDevice{
public:
    static std::map<VkPhysicalDevice, VkPhysicalDeviceProperties> enumerateSupportedDevices(VkInstance instance, Window* window){
        std::map<VkPhysicalDevice, VkPhysicalDeviceProperties> result;
        unsigned int deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            return {};
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (VkPhysicalDevice device : devices) {
            if (DeviceSuitability::isDeviceSuitable(device, window->getWindowSurface(instance))) {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(device, &properties);
                result.insert({device, properties});
            }
        }
        return result;
    }
};
