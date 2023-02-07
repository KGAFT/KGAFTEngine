//
// Created by Daniil on 07.02.2023.
//

#pragma once
#include <glm/glm.hpp>
#include "../Camera/CameraManager.h"
#include "RenderPipeline/RenderPipeline.h"
#include "GraphicalObjects/Mesh.h"

struct PointLight{
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    float add;
};

struct DirectLight{
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float add;
};

struct LightInfo{
    DirectLight directLights[100];
    PointLight pointLights[100];

    glm::vec4 enabledDirects;
    glm::vec4 enabledPoints;


    float emissiveIntensity = 2;
    float emissiveShininess = 1;
    float gammaCorrect  = 1.0f / 2.2f;
    float ambientIntensity = 0.03f ;
};

class PbrRenderPipeline : RenderPipeline{
private:
    alignas(16) LightInfo lightInfo{};
    std::vector<Mesh*> meshes;
public:
    PbrRenderPipeline(VulkanDevice* device, Window* window) : RenderPipeline(device){
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
        UniformBufferDescription uboDesc{0, sizeof(lightInfo), VK_SHADER_STAGE_FRAGMENT_BIT};
        PushConstantDescription pcDesc{VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstantData)};

        configuration.samplerDescAmount = 6;
        configuration.pushConstantDescriptionAmount = 1;
        configuration.vertexBufferDescriptionAmount = 3;
        configuration.uniformBufferDescriptionAmount = 1;
        configuration.pVertexBufferDescription = vboDescriptions.data();
        configuration.pushConstantDescription = &pcDesc;
        configuration.pSamplerDescriptions = samplerDescription;
        configuration.pUniformBufferDescription = &uboDesc;

        RenderPipeline::load(window, configuration);
    }

    void update(){
        uniformBuffers[0]->write(&lightInfo);
        RenderPipeline::update();
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
    void addMesh(Mesh* mesh){
        meshes.push_back(mesh);
    }

    LightInfo &getLightInfo() {
        return lightInfo;
    }

private:
    void preInvokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {

    }

    void invokeRenderImm(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {
        for(Mesh* mesh : meshes){
            mesh->draw(commandBuffer);
        }
    }
};
