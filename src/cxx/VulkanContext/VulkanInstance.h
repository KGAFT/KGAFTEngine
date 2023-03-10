//
// Created by KGAFT on 3/10/2023.
//
#pragma once

#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"
#include "VulkanLogger/VulkanLogger.h"
#include <vector>
#include <unordered_set>


class VulkanInstance {
private:
    VkInstance instance;

public:
    bool createInstance(const char *appName, bool enableLogging) {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = appName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "KGAFTEngine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions(enableLogging);
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if (enableLogging) {
            VulkanLogger::describeLogger(debugCreateInfo, &createInfo);
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            return false;
        }
        if(enableLogging && checkGlfwExtensions(true)){
            return VulkanLogger::init(instance);
        }
        else if(enableLogging){
            return false;
        }
        return true;
    }

private:
    bool checkGlfwExtensions(bool logging) {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::unordered_set<std::string> available;
        for (const auto &extension: extensions) {
            available.insert(extension.extensionName);
        }

        auto requiredExtensions = getRequiredExtensions(logging);
        for (const auto &required: requiredExtensions) {
            if (available.find(required) == available.end()) {
                return false;
            }
        }
        return true;
    }

    std::vector<const char *> getRequiredExtensions(bool enableValidationLayers) {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

public:
    VkInstance getInstance() {
        return instance;
    }
};
