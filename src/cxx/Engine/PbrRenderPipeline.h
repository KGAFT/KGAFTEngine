//
// Created by Daniil on 07.02.2023.
//

#pragma once
#include <glm/glm.hpp>
#include "../Camera/CameraManager.h"
#include "RenderPipeline/RenderPipeline.h"
#include "GraphicalObjects/Mesh.h"

struct PointLight{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;
};

struct DirectLight{
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;
};

struct LightInfo{
    DirectLight directLights[100];
    PointLight pointLights[100];

    alignas(4) int enabledDirects;
    alignas(4) int enabledPoints;


    alignas(4) float emissiveIntensity = 2;
    alignas(4) float emissiveShininess = 1;
    alignas(4) float gammaCorrect  = 1.0f / 2.2f;
    alignas(4) float ambientIntensity = 0.03f ;

    alignas(4) int combinedMetallicRoughness = 1;
    alignas(4) int emissiveEnabled = 0;
    int aoEnabled = 0;


};

class PbrRenderPipeline : RenderPipeline{
private:
    LightInfo lightInfo{};

public:
    PbrRenderPipeline(VulkanDevice* device, Window* window) : RenderPipeline(device){
        VulkanShader* shader = VulkanShader::loadShaderBlock("spir-v/PbrRenderPipeline", device);
        ShaderInputDataConfiguration configuration{};
        configuration.shader = shader;
        SamplerDescription samplerDescription[7];
        for(unsigned int i = 0; i<7; i++){
            samplerDescription[i] = {i+1};
        }
        std::vector<VertexBufferDescription> vboDescriptions;
        vboDescriptions.push_back({0, 3, 0});
        vboDescriptions.push_back({1, 3, 3*sizeof(float)});
        vboDescriptions.push_back({2, 2, 6*sizeof(float)});
        UniformBufferDescription uboDesc{0, sizeof(lightInfo), VK_SHADER_STAGE_FRAGMENT_BIT};
        PushConstantDescription pcDesc{VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstantData)};

        configuration.samplerDescAmount = 7;
        configuration.pushConstantDescriptionAmount = 1;
        configuration.vertexBufferDescriptionAmount = 3;
        configuration.uniformBufferDescriptionAmount = 1;
        configuration.pVertexBufferDescription = vboDescriptions.data();
        configuration.pushConstantDescription = &pcDesc;
        configuration.pSamplerDescriptions = samplerDescription;
        configuration.pUniformBufferDescription = &uboDesc;

        RenderPipeline::load(window, configuration);
    }
    void beginRender(){
        uniformBuffers[0]->write(&lightInfo);
        RenderPipeline::beginRender();
    }
    void beginDraw(unsigned int* currentImage, VkCommandBuffer* commandBuffer, VkPipelineLayout* layout){
        RenderPipeline::beginDraw();
        (*currentImage) = RenderPipeline::renderingPipeline->getCurrentImage();
        (*commandBuffer) = RenderPipeline::renderingPipeline->getCurrentCommandBuffer();
        (*layout) = RenderPipeline::renderingPipeline->getPipelineLayout();
    }

    void endDraw(){
        RenderPipeline::endRender();
    }
    void updateShaderData(){
       // uniformBuffers[0]->write(&lightInfo);
        RenderPipeline::updateShaderData();
    }
    PushConstant* getPushConstant(unsigned int index){
        return pushConstants[index];
    }

    void setSamplerTexture(VulkanTexture* texture, unsigned int binding){
        for (const auto &item: samplers){
            if(item->getPDescriptorSetLayoutBind()->binding==binding){
                item->setTexture(texture);
                break;
            }
        }
    }

    LightInfo &getLightInfo() {
        return lightInfo;
    }
};
