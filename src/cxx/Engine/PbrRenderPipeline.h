//
// Created by Daniil on 04.02.2023.
//

#pragma once
#include <glm/glm.hpp>
#include "../Camera/CameraManager.h"
#include "RenderPipeline.h"

struct PointLight{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
};

struct DirectLight{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
};

struct LightInfo{
    PointLight pointLights[100];
    DirectLight directLights[100];
    int enabledSpotLights;
    int enabledDirectionalLights;
    int enabledPointLights;
    glm::vec3 cameraPosition;

    float emissiveIntensity;
    float emissiveShininess;
    float gammaCorrect;
    float ambientIntensity;
};


class PbrRenderPipeline : RenderPipeline{
private:
    LightInfo lightInfo{};
public:
    PbrRenderPipeline(VulkanDevice *device, Window *window) : RenderPipeline(device, window) {
        VulkanShader* shader = VulkanShader::loadShaderBlock("spir-v/PbrRenderPipeline", device);
        ShaderInputDataConfiguration configuration{};
        configuration.shader = shader;
        SamplerDescription samplerDescription[6];
        for(unsigned int i = 0; i<6; i++){
            samplerDescription[i] = {i+1};
        }
        std::vector<VertexBufferDescription> vboDescriptions;
        vboDescriptions.push_back({0, 3, 0});
        vboDescriptions.push_back({1, 3, 3*sizeof(float)});
        vboDescriptions.push_back({2, 2, 6*sizeof(float)});
        UniformBufferDescription uboDesc{0, sizeof(LightInfo), VK_SHADER_STAGE_FRAGMENT_BIT};
        PushConstantDescription pcDesc{VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstantData)};

        configuration.samplerDescAmount = 6;
        configuration.pushConstantDescriptionAmount = 1;
        configuration.vertexBufferDescriptionAmount = 3;
        configuration.uniformBufferDescriptionAmount = 1;
        configuration.pVertexBufferDescription = vboDescriptions.data();
        configuration.pushConstantDescription = &pcDesc;
        configuration.pSamplerDescriptions = samplerDescription;
        configuration.pUniformBufferDescription = &uboDesc;

        load(configuration);

    }
    void startRenderProcess(){
        RenderPipeline::beginRender();
        RenderPipeline::beginDraw();

    }
    void endRenderProcess(){
        RenderPipeline::endRender();
    }
    void setPushConstantData(void* data){
        for (const auto &item: pushConstants){
            item->setData(data);
        }
    }
    void writeDataToShader(){
        RenderPipeline::updatePushConstants();
        for (const auto &item: uniformBuffers){
            item->write(&lightInfo);
        }
        RenderPipeline::updateUniformsAndSamplers();
    }

    LightInfo *getLightInfo(){
        return &lightInfo;
    }

    void setSamplerTexture(unsigned int binding, VulkanTexture* texture){
        for(const auto& item : textureSamplers){
            if(item->getBinding()==binding){
                item->setTexture(texture);
            }
        }
    }

};