#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "Engine/PbrRenderPipeline.h"
#include "Engine/Util/ModelLoader.h"
#include "Engine/PbrEngine.h"
#include "VulkanContext/VulkanLogger/DefaultVulkanLoggerCallback.h"


int main() {
    Window::initWindow("KGAFTEngine", 1024, 768);
    VulkanInstance instance;
    instance.createInstance("KGAFTEngine", false);
    VulkanLogger::registerCallback(new DefaultVulkanLoggerCallback());
    VkPhysicalDevice deviceToCreate;
    std::vector<VkPhysicalDevice> devices;

    int i = 0;

    for (const auto &item: VulkanDevice::enumerateSupportedDevices(instance.getInstance(),
                                                                   Window::getWindowInstance())) {
        std::cout << i << "\t" << item.second.deviceName << std::endl;
        devices.push_back(item.first);
        i++;
    };
    std::cout << "Select device, by entering id: ";
    std::cin >> i;
    deviceToCreate = devices[i];

    VulkanDevice device(deviceToCreate, Window::getWindowInstance(), instance.getInstance(), false);
    PbrEngine engine(Window::getWindowInstance(), &device);

    ModelLoader loader(&device);


    std::vector<Mesh *> pokedexMeshes = loader.loadModel("models/pokedex/pokedex.gltf");
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
    pokedexMeshes[0]->scale(glm::vec3(0.1, 0.1, 0.1));
    pokedexMeshes[0]->rotate(180, glm::vec3(0, 1, 0));
    engine.addMesh(pokedexMeshes[0]);

    loader.clear();


    std::vector<Mesh *> stationMeshes = loader.loadModel("models/station/scene.gltf");

    VulkanImage stationAlb = VulkanImage::loadTextureFromFiles(&device,
                                                               "models/station/textures/station_1_baseColor.jpeg");
    VulkanImage stationEmissive = VulkanImage::loadTextureFromFiles(&device,
                                                                    "models/station/textures/station_1_emissive.jpeg");
    VulkanImage metallicRoughnessSta = VulkanImage::loadTextureFromFiles(&device,
                                                                         "models/station/textures/station_1_metallicRoughness.png");
    VulkanImage normalSta = VulkanImage::loadTextureFromFiles(&device,
                                                              "models/station/textures/station_1_clearcoat_normal.png");
    for (const auto &item: stationMeshes) {
        item->setPosition(glm::vec3(3, 0, 0));
        if (item->getAlbedoTexture() == nullptr) {
            item->setAlbedoTexture(new VulkanTexture(&stationAlb, &device));
        }
        if (item->getNormalTexture() == nullptr) {
            item->setNormalTexture(new VulkanTexture(&normalSta, &device));
        }
        if (item->getMetallicRoughnessTexture() == nullptr) {
            item->setMetallicRoughnessTexture(new VulkanTexture(&metallicRoughnessSta, &device));
        }
        if (item->getEmissiveTexture() == nullptr) {
            item->setEmissiveTexture(new VulkanTexture(&stationEmissive, &device));
        }
        item->scale(glm::vec3(0.3, 0.3, 0.3));
        engine.addMesh(item);
    }

    loader.clear();


    engine.getLightInfo().enabledPoints = 1;
    engine.getLightInfo().pointLights[0].color = glm::vec3(1, 1, 1);
    engine.getLightInfo().pointLights[0].intensity = 500;
    engine.getLightInfo().pointLights[0].position = glm::vec3(-2, 10, 5);
    while (!Window::getWindowInstance()->isWindowNeedToClose()) {
        for (const auto &item: engine.getMeshes()) {
            //item->rotate(0.1, glm::vec3(0,1,0));
        }
        engine.update();
    }
    return 0;
}
