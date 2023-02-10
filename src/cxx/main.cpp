#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "Engine/PbrRenderPipeline.h"
#include "Engine/Util/ModelLoader.h"
#include "Engine/PbrEngine.h"
#include "VulkanContext/VulkanLogger/DefaultVulkanLoggerCallback.h"


int main() {
    Window::initWindow("Hello world!", 1024, 768);
    VulkanInstance instance;
    instance.createInstance("TestApp", true);
    VulkanLogger::registerCallback(new DefaultVulkanLoggerCallback());
    VkPhysicalDevice deviceToCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(deviceToCreate, Window::getWindowInstance(), instance.getInstance(), true);
    PbrEngine engine(Window::getWindowInstance(), &device);

    ModelLoader loader(&device);
    std::vector<Mesh*> grindMeshes = loader.loadModel("models/grind/scene.gltf");
    VulkanImage albedoImGrind = VulkanImage::loadTextureFromFiles(&device, "models/grind/albedo.png");
    VulkanImage metallicRoughnessGrind = VulkanImage::loadTextureFromFiles(&device, "models/grind/mr.png");
    VulkanImage normalImGrind = VulkanImage::loadTextureFromFiles(&device, "models/grind/normal.png");

    for (const auto &item: grindMeshes){
        item->setAlbedoTexture(new VulkanTexture(&albedoImGrind, &device));
        item->setNormalTexture(new VulkanTexture(&normalImGrind, &device));
        item->setMetallicRoughnessTexture(new VulkanTexture(&metallicRoughnessGrind, &device));
        engine.addMesh(item);

    }

    loader.clear();

    std::vector<Mesh*> pokedexMeshes = loader.loadModel("models/pokedex/pokedex.gltf");
    VulkanImage albedoIm = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/basecolor.tga");
    VulkanImage emissiveIm = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/emissive.tga");
    VulkanImage metallic = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/metallic.tga");
    VulkanImage normalIm = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/normal.tga");
    VulkanImage roughnessIm = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/roughness.tga");
    VulkanImage aoIm = VulkanImage::loadTextureFromFiles(&device, "models/pokedex/textures/ao.tga");

    pokedexMeshes[0]->setAlbedoTexture(new VulkanTexture(&albedoIm, &device));
    pokedexMeshes[0]->setNormalTexture(new VulkanTexture(&normalIm, &device));
    pokedexMeshes[0]->setMetallicTexture(new VulkanTexture(&metallic, &device));
    pokedexMeshes[0]->setRoughnessTexture(new VulkanTexture(&roughnessIm, &device));
    pokedexMeshes[0]->setAoTexture(new VulkanTexture(&aoIm, &device));
    pokedexMeshes[0]->setEmissiveTexture(new VulkanTexture(&emissiveIm, &device));
    pokedexMeshes[0]->setPosition(glm::vec3(-3, 0, 0));
    pokedexMeshes[0]->scale(glm::vec3(0.1,0.1,0.1));
    pokedexMeshes[0]->rotate(180, glm::vec3(0, 1, 0));
    engine.addMesh(pokedexMeshes[0]);

    engine.getLightInfo().enabledPoints = 1;
    engine.getLightInfo().pointLights[0].color = glm::vec3(1,1,1);
    engine.getLightInfo().pointLights[0].intensity = 3;
    engine.getLightInfo().pointLights[0].position = glm::vec3(-2,0,0);
    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        for (const auto &item: engine.getMeshes()){
            item->rotate(0.1, glm::vec3(0,1,0));
        }
        engine.update();
    }
    return 0;
}
