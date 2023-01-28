//
// Created by daniil on 26.01.23.
//
#include <shaderc/shaderc.h>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <vulkan/vulkan.h>
#include <vector>
#include "../VulkanDevice/VulkanDevice.h"

#pragma once

class VulkanShader{
private:
    static inline shaderc_compiler_t compiler = shaderc_compiler_initialize();
public:
    static VulkanShader* loadShaderBlock(const char* directoryPath, VulkanDevice* device){
        std::vector<VkShaderModule> shaders;
        std::vector<shaderc_shader_kind> types;
        for (const auto & entry : std::filesystem::directory_iterator(directoryPath)){
            shaderc_shader_kind type;
            shaders.push_back(loadShaderFromFile(entry.path().c_str(), device, &type));
            types.push_back(type);
        }
        return new VulkanShader(device, shaders, types);
    }
    static VkShaderModule loadShaderFromFile(const char* path, VulkanDevice* device, shaderc_shader_kind* outType){
        std::string tPath = std::string(path);
        int offSetToName = tPath.find_last_of('/');
        std::string name = tPath.substr(offSetToName+1, tPath.size());
        shaderc_shader_kind type = getShaderType(name);
        *outType = type;
        size_t size = 0;
        const char* content = compileShader(path, type, name.c_str(), &size);
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.codeSize = size;
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pCode = reinterpret_cast<const uint32_t *>(content);
        VkShaderModule result;
        if (vkCreateShaderModule(device->getDevice(), &createInfo, nullptr, &result) != VK_SUCCESS) {
            delete content;
            throw std::runtime_error("failed to create shader module");
        }
        delete content;
        return result;
    }
    static shaderc_shader_kind getShaderType(std::string fileName){
        std::string extension = "";
        bool started = false;
        for (char ch: fileName){
            if(started){
                extension+=ch;
            }
            if(!started && ch=='.'){
                started = true;
            }
        }
        if(!std::strcmp(extension.c_str(), "frag")){
            return shaderc_glsl_fragment_shader;
        }
        if(!std::strcmp(extension.c_str(), "vert")){
            return shaderc_glsl_vertex_shader;
        }
        if(!std::strcmp(extension.c_str(), "geom")){
            return shaderc_glsl_geometry_shader;
        }
    }

    static const char* compileShader(const char* pathToShader, shaderc_shader_kind shaderType, const char* fileName, size_t* size){
        std::string shaderCode = readCode(pathToShader);
        shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, shaderCode.c_str(), shaderCode.size()*sizeof(char),
                                 shaderType, fileName, "main", nullptr);
        if(result == nullptr){
            throw std::runtime_error("Failed to compile shader: "+std::string(fileName));
        }
        if(shaderc_result_get_compilation_status(result) != shaderc_compilation_status_success){
            throw std::runtime_error("Failed to compile shader " + std::string(fileName) + "into SPIR-V:\n " + shaderc_result_get_error_message(result));
        }
        const char* code = shaderc_result_get_bytes(result);
        *size = shaderc_result_get_length(result);
        return code;
    }
    static std::string readCode(const char* filePath){
        std::ifstream fileReader(filePath, std::ios::binary);
        if(fileReader){
            std::string content;
            fileReader.seekg(0, std::ios::end);
            content.resize(fileReader.tellg());
            fileReader.seekg(0, std::ios::beg);
            fileReader.read(&content[0], content.size());
            fileReader.close();
            return content;
        }
    }
private:
    std::vector<VkShaderModule> shaders;
    std::vector<VkPipelineShaderStageCreateInfo> stages;
    VulkanDevice* device;
    VulkanShader(VulkanDevice* device, std::vector<VkShaderModule> shaders, std::vector<shaderc_shader_kind>& types){
        this->shaders = shaders;
        this->device=device;
        for(unsigned int i = 0; i<shaders.size(); i++){
            VkPipelineShaderStageCreateInfo stage = {};
            stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stage.stage = getShaderStage(types[i]);
            stage.module = shaders[i];
            stage.pName = "main";
            stage.flags = 0;
            stage.pNext = nullptr;
            stages.push_back(stage);
        }
    }
    VkShaderStageFlagBits getShaderStage(shaderc_shader_kind type){
        switch(type){
            case shaderc_glsl_fragment_shader:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            case shaderc_glsl_geometry_shader:
                return VK_SHADER_STAGE_GEOMETRY_BIT;
            case shaderc_glsl_vertex_shader:
                return VK_SHADER_STAGE_VERTEX_BIT;
            default:
                break;
        }
    }
public:
    VkPipelineShaderStageCreateInfo* getStages(){
        return stages.data();
    }
    void destroy(){
        for (const auto &item: shaders){
            vkDestroyShaderModule(device->getDevice(), item, nullptr);
        }
    }
    ~VulkanShader(){
        destroy();
    }
};
