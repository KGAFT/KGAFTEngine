#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "VulkanContext/VulkanLogger/DefaultVulkanLoggerCallback.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"
#include <iostream>
#include <glm/glm.hpp>
#include "Camera/CameraManager.h"
#include "VulkanContext/VulkanImmediateShaderData/UniformBuffers/UniformBuffer.h"
#include "VulkanContext/VulkanDescriptor/VulkanDescritptors.h"
#include "VulkanContext/VulkanImage/VulkanImage.h"
#include "VulkanContext/VulkanImage/VulkanTexture.h"
#include "VulkanContext/VulkanImage/VulkanSampler.h"
#include "VulkanContext/VulkanLogger/VulkanLogger.h"

class RenderImplCallback : public RenderPassCallback{
private:
    VertexBuffer* vbo;
    IndexBuffer* ibo;
    PushConstantDescriptionManager* pushConstant;
    VulkanDescriptors* descriptors;
    IDescriptorLayoutObject** objects;
public:

    RenderImplCallback(VertexBuffer *vbo, IndexBuffer *ibo, PushConstantDescriptionManager *pushConstant,
                       VulkanDescriptors *descriptors, IDescriptorLayoutObject** objects) : vbo(vbo), ibo(ibo),
                                                                             pushConstant(pushConstant),
                                                                             descriptors(descriptors), objects(objects) {}

    void invokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {
        pushConstant->loadConstantsToShader(commandBuffer, layout);
        descriptors->writeDescriptor(objects, 2, imageIndex);
        descriptors->bind(imageIndex, commandBuffer, layout);
        vbo->bind(commandBuffer);
        ibo->bind(commandBuffer);
        ibo->draw(commandBuffer);
    }
};

int main() {
    Window::initWindow("Hello world!", 1024, 768);
    VulkanInstance instance;
    instance.createInstance("TestApp", true);
    VulkanLogger::registerCallback(new DefaultVulkanLoggerCallback());
    VkPhysicalDevice deviceToCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(deviceToCreate, Window::getWindowInstance(), instance.getInstance(), true);
    VulkanSwapChain swapChain(&device, Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    VulkanRenderPass renderPass(&device, &swapChain);
    VulkanSwapChainControl control(&swapChain, &renderPass, &device);
    VertexBufferDescriptionManager vboDesc;

    PushConstant pushConstant(sizeof(PushConstantData), VERTEX_SHADER_PUSH_CONSTANT);
    vboDesc.createBinding(0, 8*sizeof(float));
    vboDesc.registerAttribute(0, 0, 3, 0);
    vboDesc.registerAttribute(0, 1, 3, sizeof(float)*3);
    vboDesc.registerAttribute(0, 2, 2, sizeof(float)*6);
    PushConstantDescriptionManager pcsDesc;
    pcsDesc.registerPushConstant(&pushConstant);

    PipelineConfiguration::PipelineConfigInfo config = PipelineConfiguration::defaultPipelineConfigInfo(Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    config.renderPass = renderPass.getRenderPass();

    UniformBuffer ubo(&device, sizeof(PushConstantData), VK_SHADER_STAGE_VERTEX_BIT, 0, 3) ;


    VulkanImage image = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/normal.tga");
    VulkanTexture texture(&image, &device);
    VulkanSampler sampler(&device, 1);
    sampler.setTexture(&texture);
    IDescriptorLayoutObject* objects[]{&ubo, &sampler};
    GraphicsPipeline pipeline(VulkanShader::loadShaderBlock("spir-v/TestRenderPipelineShaders", &device), &device, config ,
                              &vboDesc, &pcsDesc, objects, 2);

    VulkanRenderingPipeline renderingPipeline(&control, &renderPass, &pipeline,&device);
    Window::getWindowInstance()->registerResizeCallback(&renderingPipeline);
    VulkanDescriptors descriptors(&device, pipeline.getDescriptorSetLayout(), 3, objects, 2);
    float vertices[]{
        -0.5f, -0.5f, 0.5f,      -0.5f, -0.5f, 0.0f,    -0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,      -0.5f, 0.5f, 0.0f,    -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,      0.5f, -0.5f, 0.0f,    0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,         0.5f, 0.5f, 0.0f,      0.5f, 0.5f
    };

    unsigned int indices[]{
        0,1,2,3,2,1
    };

    VertexBuffer vertexBuffer(4, sizeof(float), 8, &device, vertices);
    IndexBuffer ibo(&device, indices, 6);

    RenderImplCallback rimp(&vertexBuffer, &ibo, &pcsDesc, &descriptors, objects);
    renderingPipeline.registerRenderPassCallback(&rimp);

    PushConstantData data{};
    CameraManager manager(&data);
    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        Window::getWindowInstance()->preRenderEvents();
        manager.update();
        ubo.write(&data);

        renderingPipeline.redrawCommandBuffers();

        Window::getWindowInstance()->postRenderEvents();
    }
    return 0;
}
