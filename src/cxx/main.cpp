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
#include "Engine/PbrRenderPipeline.h"
#include "Engine/Util/ModelLoader.h"


int main() {
    Window::initWindow("Hello world!", 1024, 768);
    VulkanInstance instance;
    instance.createInstance("TestApp", true);
    VulkanLogger::registerCallback(new DefaultVulkanLoggerCallback());
    VkPhysicalDevice deviceToCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(deviceToCreate, Window::getWindowInstance(), instance.getInstance(), true);
    PbrRenderPipeline renderPipeline(&device, Window::getWindowInstance());

    PushConstantData data{};
    CameraManager cameraManager(&data);
    ModelLoader loader(&device);
    std::vector<Mesh*> meshes = loader.loadModel("models/grind/scene.gltf");
    for (const auto &item: meshes){
        renderPipeline.addMesh(item);
    }
    VulkanImage albedoIm = VulkanImage::loadTextureFromFiles(&device, "models/grind/albedo.png");
    VulkanImage emissiveIm = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/emissive.tga");
    VulkanImage metallicRoughness = VulkanImage::loadTextureFromFiles(&device, "models/grind/mr.png");
    VulkanImage normalIm = VulkanImage::loadTextureFromFiles(&device, "models/grind/normal.png");
    VulkanImage aoIm = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/ao.tga");

    renderPipeline.setSamplerTexture(new VulkanTexture(&albedoIm, &device), 1);
    renderPipeline.setSamplerTexture(new VulkanTexture(&normalIm, &device), 2);
    renderPipeline.setSamplerTexture(new VulkanTexture(&metallicRoughness, &device), 3);
    renderPipeline.setSamplerTexture(new VulkanTexture(&metallicRoughness, &device), 4);
    renderPipeline.setSamplerTexture( new VulkanTexture(&aoIm, &device), 5);
    renderPipeline.setSamplerTexture(new VulkanTexture(&emissiveIm, &device), 6);
    renderPipeline.setSamplerTexture(new VulkanTexture(&metallicRoughness, &device), 7);


    renderPipeline.getPushConstant(0)->setData(&data);
    renderPipeline.getLightInfo().enabledDirects = 1;
    renderPipeline.getLightInfo().directLights[0].direction = glm::vec3(-1,-1,-1);
    renderPipeline.getLightInfo().directLights[0].color = glm::vec3(1,1,1);
    renderPipeline.getLightInfo().directLights[0].intensity = 20.0f;
    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        Window::getWindowInstance()->preRenderEvents();
        cameraManager.update();

        renderPipeline.update();

        Window::getWindowInstance()->postRenderEvents();
    }
    return 0;
}
