#include <assimp/Importer.hpp>

#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "VulkanContext/VulkanLogger/DefaultVulkanLoggerCallback.h"

#include "Engine/TestRenderPipeline.h"
#include "Engine/PbrRenderPipeline.h"
#include "Engine/Util/ModelLoader.h"


int main() {
    Window::initWindow("Hello world!", 1024, 768);
    VulkanInstance instance;
    instance.createInstance("TestApp", true);
    VulkanLogger::registerCallback(new DefaultVulkanLoggerCallback());
    std::vector<VkPhysicalDevice> physDevices;

    for (const auto &item: VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance())){
        std::cout<<"Device: "<<item.second.deviceName<<std::endl;
        physDevices.push_back(item.first);
    }

    int deviceIndex = 0;
    std::cout<<"Select device: ";
    std::cin>>deviceIndex;



    VulkanDevice device(physDevices[deviceIndex], Window::getWindowInstance(), instance.getInstance(), true);
    PbrRenderPipeline* renderPipeline = new PbrRenderPipeline(&device, Window::getWindowInstance());
    PushConstantData data{};
    CameraManager cameraManager(&data);


    VulkanImage baseColorImage = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/basecolor.tga");
    VulkanImage emissiveImage = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/emissive.tga");
    VulkanImage metallicImage = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/metallic.tga");
    VulkanImage normalImage = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/normal.tga");
    VulkanImage roughnessImage = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/roughness.tga");
    VulkanImage aoImage = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/ao.tga");

    VulkanTexture baseColor(&baseColorImage, &device);
    VulkanTexture emissive(&emissiveImage, &device);
    VulkanTexture metallic(&metallicImage, &device);
    VulkanTexture normal(&normalImage, &device);
    VulkanTexture roughness(&roughnessImage, &device);
    VulkanTexture ao(&aoImage, &device);
    ModelLoader loader(&device);
    std::vector<Mesh*> meshes = loader.loadModel("models/pokedex/pokedex.gltf");
    meshes[0]->setPosition(glm::vec3(1,1,1));
    loader.clear();

    renderPipeline->setSamplerTexture(1, &baseColor);
    renderPipeline->setSamplerTexture(2, &normal);
    renderPipeline->setSamplerTexture(3, &metallic);
    renderPipeline->setSamplerTexture(4, &roughness);
    renderPipeline->setSamplerTexture(5, &ao);
    renderPipeline->setSamplerTexture(6, &emissive);

    renderPipeline->getLightInfo()->enabledPointLights = 1;
    renderPipeline->getLightInfo()->pointLights[0].color=glm::vec3(1,1,1);
    renderPipeline->getLightInfo()->pointLights[0].position = glm::vec3(-10,13,1);
    renderPipeline->getLightInfo()->pointLights[0].intensity=200;
    while(!Window::getWindowInstance()->isWindowNeedToClose()) {
        Window::getWindowInstance()->preRenderEvents();
        cameraManager.update();
        renderPipeline->getLightInfo()->cameraPosition = cameraManager.getCurrentCamera()->getPosition();
        renderPipeline->startRenderProcess();
        renderPipeline->setPushConstantData(&data);
        renderPipeline->writeDataToShader();

        for (const auto &item: meshes){
            renderPipeline->drawMesh(item);
        }

        renderPipeline->endRenderProcess();
        Window::getWindowInstance()->postRenderEvents();
    }
    return 0;
}
