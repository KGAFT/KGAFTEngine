#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include "IVulkanLoggerCallback.h"

#pragma once

class VulkanLogger {
    friend class VulkanInstance;

public:
    static inline const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void *pUserData) {
        if(rawCallbacks.size()==0 && translatedCallbacks.size() == 0 && bothCallbacks.size()==0){
            std::cerr << "Vulkan: " << pCallbackData->pMessage << std::endl;
        }

        for (const auto &item: rawCallbacks) {
            item->messageRaw(messageSeverity, messageType, pCallbackData, pUserData);
        }
        for (const auto &item: bothCallbacks) {
            item->messageRaw(messageSeverity, messageType, pCallbackData, pUserData);
        }
        if (!translatedCallbacks.empty()) {
            const char *severity = translateSeverity(messageSeverity);
            const char *type = translateType(messageType);
            std::string message = std::string(pCallbackData->pMessage);
            for (const auto &item: translatedCallbacks) {
                item->translatedMessage(severity, type, message);
            }

            for (const auto &item: bothCallbacks) {
                item->translatedMessage(severity, type, message);
            }

        }

        return VK_FALSE;
    }

private:
    static const char *translateSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT severity) {
        const char *res;
        switch (severity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                res = "VERBOSE";
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                res = "ERROR";
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                res = "WARNING";
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                res = "INFO";
                break;
            default:
                res = "UNDEFINED";
                break;
        }
        return res;
    }

    static const char *translateType(VkDebugUtilsMessageTypeFlagsEXT type) {
        const char *res;
        switch (type) {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
                res = "BINDING";
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                res = "GENERAL";
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                res = "PERFORMANCE";
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                res = "VALIDATION";
                break;
            default:
                res = "UNDEFINED";
                break;
        }
        return res;
    }

private:
    static inline VkDebugUtilsMessengerEXT debugMessenger = nullptr;

    static inline std::vector<IVulkanLoggerCallback *> rawCallbacks = std::vector<IVulkanLoggerCallback *>();
    static inline std::vector<IVulkanLoggerCallback *> translatedCallbacks = std::vector<IVulkanLoggerCallback *>();
    static inline std::vector<IVulkanLoggerCallback *> bothCallbacks = std::vector<IVulkanLoggerCallback *>();

    static void describeLogger(VkDebugUtilsMessengerCreateInfoEXT &createInfo, VkInstanceCreateInfo *instanceInfo) {
        if (instanceInfo != nullptr) {
            instanceInfo->enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            instanceInfo->ppEnabledLayerNames = validationLayers.data();
        }

        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                                     | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;
    }

    static VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                instance,
                "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

public:
    static bool init(VkInstance instance) {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        describeLogger(createInfo, nullptr);
        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            return false;
        }
        return true;
    }

    static void registerCallback(IVulkanLoggerCallback *callback) {
        if (debugMessenger != nullptr) {
            switch (callback->getCallBackMode()) {
                case RAW_VULKAN_DEFS:
                    rawCallbacks.push_back(callback);
                    break;
                case TRANSLATED_DEFS:
                    translatedCallbacks.push_back(callback);
                    break;
                case BOTH_DEFS:
                    bothCallbacks.push_back(callback);
                    break;
            }
        }
    }

    static void removeCallback(IVulkanLoggerCallback *callback) {
        if (debugMessenger != nullptr) {
            std::vector<IVulkanLoggerCallback *> *toRemove = nullptr;
            switch (callback->getCallBackMode()) {
                case RAW_VULKAN_DEFS:
                    toRemove = &rawCallbacks;
                    break;
                case TRANSLATED_DEFS:
                    toRemove = &translatedCallbacks;
                    break;
                case BOTH_DEFS:
                    toRemove = &bothCallbacks;
                    break;
            }
            if (toRemove != nullptr) {
                std::vector<IVulkanLoggerCallback *> temp;
                bool found = false;
                for (IVulkanLoggerCallback *curCallback: *toRemove) {
                    if (curCallback != callback) {
                        temp.push_back(callback);
                    } else {
                        found = true;
                    }
                }
                if (found) {
                    toRemove->clear();
                    for (IVulkanLoggerCallback *curCallback: temp) {
                        toRemove->push_back(curCallback);
                    }
                }
            }
        }
    }
};