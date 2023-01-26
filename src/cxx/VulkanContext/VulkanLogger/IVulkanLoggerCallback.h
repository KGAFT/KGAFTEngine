//
// Created by daniil on 24.01.23.
//
#define RAW_VULKAN_DEFS 1
#define TRANSLATED_DEFS 2
#define BOTH_DEFS 3

#include <vulkan/vulkan.h>
#include <string>

#pragma once

class IVulkanLoggerCallback {
public:
    virtual int getCallBackMode() = 0;

    virtual void messageRaw(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type,
                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) = 0;

    virtual void translatedMessage(const char *severity, const char *type, std::string message) = 0;
};
