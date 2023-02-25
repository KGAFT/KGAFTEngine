#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "Engine/PbrRenderPipeline.h"
#include "Engine/Util/ModelLoader.h"
#include "Engine/PbrEngine.h"
#include "VulkanContext/VulkanLogger/DefaultVulkanLoggerCallback.h"


int main() {
    Window::initWindow("KGAFTEngine", 2560, 1440);
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

    std::cout<<"Loading models..."<<std::endl;
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
    pokedexMeshes[0]->setOpacityTexture(nullptr);
    pokedexMeshes[0]->setPosition(glm::vec3(-3, 0, 0));
    pokedexMeshes[0]->scale(glm::vec3(0.1, 0.1, 0.1));
    pokedexMeshes[0]->rotate(180, glm::vec3(0, 1, 0));
    engine.addMesh(pokedexMeshes[0]);

    loader.clear();

/*
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
    */
/*
    loader.clear();
    std::vector<Mesh*> garageMeshes = loader.loadModel("models/garage/garage.obj");
    for (const auto &item : garageMeshes){
        item->scale(glm::vec3(0.15f, 0.15f, 0.15f));
        item->rotate(-90, glm::vec3(1, 0, 0));
        engine.addMesh(item);
    }
    loader.clear();

    std::vector<Mesh*> binocular = loader.loadModel("models/binocular/Binoculars from World War.obj");
    loader.clear();
    for (const auto &item : binocular){
        item->scale(glm::vec3(0.15, 0.15, 0.15));
        item->setPosition(glm::vec3(-8, 0, 0));
        item->rotate(-90, glm::vec3(1,0,0));
        engine.addMesh(item);
    }
    std::vector<Mesh*> grindMeshes = loader.loadModel("models/grind/scene.gltf");
    VulkanTexture* grindBaseColor = new VulkanTexture(new VulkanImage(VulkanImage::loadTextureFromFiles(&device, "models/grind/textures/Main_baseColor.png")), &device);
    VulkanTexture* grindNormal = new VulkanTexture(new VulkanImage(VulkanImage::loadTextureFromFiles(&device, "models/grind/textures/Main_normal.png")), &device);
    VulkanTexture* grindMetallicRoughness = new VulkanTexture(new VulkanImage(VulkanImage::loadTextureFromFiles(&device, "models/grind/textures/Main_metallicRoughness.png")), &device);

    for (const auto &item : grindMeshes){
        item->setAlbedoTexture(grindBaseColor);
        item->setNormalTexture(grindNormal);
        item->setMetallicRoughnessTexture(grindMetallicRoughness);
        item->setAoTexture(nullptr);
        item->setEmissiveTexture(nullptr);
        item->setMetallicTexture(nullptr);
        item->setRoughnessTexture(nullptr);
        item->setPosition(glm::vec3(-6, 0, 0));
        engine.addMesh(item);
    }
*/
    loader.clear();
    VulkanTexture* blackTexture = new VulkanTexture(new VulkanImage(VulkanImage::loadTextureFromFiles(&device, "models/blackTexture.png")), &device);
    VulkanTexture* secondBlackTexture = new VulkanTexture(new VulkanImage(VulkanImage::loadTextureFromFiles(&device, "models/blackTexture.png")), &device);
    VulkanTexture* thirdBlackTexture = new VulkanTexture(new VulkanImage(VulkanImage::loadTextureFromFiles(&device, "models/blackTexture.png")), &device);
    VulkanTexture* fourthTexture = new VulkanTexture(new VulkanImage(VulkanImage::loadTextureFromFiles(&device, "models/blackTexture.png")), &device);


    loader.clear();
    std::vector<Mesh*> underWater = loader.loadModel("models/UnderWater/scene.gltf");
    for (const auto &item: underWater){
        if(item->getAlbedoTexture()==nullptr){
            item->setAlbedoTexture(blackTexture);
        }
        if(item->getNormalTexture()==nullptr){
            item->setNormalTexture(secondBlackTexture);
        }
        if(item->getMetallicRoughnessTexture()==nullptr && item->getMetallicTexture()==nullptr && item->getRoughnessTexture() == nullptr){
            item->setMetallicRoughnessTexture(thirdBlackTexture);
        }
        if(item->getEmissiveTexture()==item->getNormalTexture()){
            item->setEmissiveTexture(nullptr);
        }
        item->setPosition(glm::vec3(5, 0, 0));
        if(item->getAoTexture()== nullptr){
            item->setAoTexture(fourthTexture);
        }
        item->setOpacityTexture(nullptr);
        item->rotate(-90, glm::vec3(1,0,0));
        engine.addMesh(item);
    }

    engine.getLightInfo().enabledDirects = 1;
    engine.getLightInfo().directLights[0].color = glm::vec3(1, 1, 1);
    engine.getLightInfo().directLights[0].intensity = 100;
    engine.getLightInfo().directLights[0].direction = glm::vec3(-1,-1,-1);

    std::cout<<"Loaded!"<<std::endl;

    while (!Window::getWindowInstance()->isWindowNeedToClose()) {
        for (const auto &item: engine.getMeshes()) {
            //item->rotate(0.1, glm::vec3(0,1,0));
        }
        engine.update();
    }
    return 0;
}
