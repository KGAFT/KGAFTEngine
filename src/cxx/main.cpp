#include <Vulkan/VulkanInstance.h>
#include <Vulkan/VulkanDevice/VulkanDevice.h>
#include "Window/Window.h"
#include "Engine/PbrEngine.h"
#include "Camera/CameraManager.h"
#include "Engine/Util/ModelLoader.h"

int main()
{
    Window::initWindow("KGAFTEngine", 2560, 1440);
    VulkanInstance instance;
    const char **exts;
    uint32_t extsAmount;
    exts = glfwGetRequiredInstanceExtensions(&extsAmount);
    std::vector<const char *> extensions;
    for (uint32_t c = 0; c < extsAmount; c++)
    {
        extensions.push_back(exts[c]);
    }

    instance.createInstance("KGAFTEngine", false, extensions);
    std::map<VkPhysicalDevice, VkPhysicalDeviceProperties> devices = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()->getWindowSurface(instance.getInstance()));
    std::vector<VkPhysicalDevice> devicesToCreate;
    int c = 0;
    std::cout << "Engine found supported devices in your's system, please select device to render: " << std::endl;
    std::cout << "ID:\tDevice name:" << std::endl;
    for (const auto &el : devices)
    {
        devicesToCreate.push_back(el.first);
        std::cout << c << "\t" << el.second.deviceName << std::endl;
        c++;
    }
    std::cout << "Please select device, by entering id: ";
    std::cin >> c;
    VulkanDevice device(devicesToCreate[c], Window::getWindowInstance()->getWindowSurface(instance.getInstance()), instance.getInstance(), false);
    VulkanSwapChain swapChain(&device, Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    PbrEngine engine(Window::getWindowInstance(), &device, &swapChain);
    ModelLoader loader(&device);

    std::cout << "Loading models..." << std::endl;
    std::vector<Mesh *> pokedexMeshes = loader.loadModel("models/pokedex/pokedex.gltf");
    VulkanImage *albedoIm = VulkanImage::loadTexture("models/pokedex/textures/basecolor.tga", &device);
    VulkanImage *emissiveIm = VulkanImage::loadTexture("models/pokedex/textures/emissive.tga", &device);
    VulkanImage *metallic = VulkanImage::loadTexture("models/pokedex/textures/metallic.tga", &device);
    VulkanImage *normalIm = VulkanImage::loadTexture("models/pokedex/textures/normal.tga", &device);
    VulkanImage *roughnessIm = VulkanImage::loadTexture("models/pokedex/textures/roughness.tga", &device);
    VulkanImage *aoIm = VulkanImage::loadTexture("models/pokedex/textures/ao.tga", &device);

    pokedexMeshes[0]->setAlbedoTexture(albedoIm);
    pokedexMeshes[0]->setNormalTexture(normalIm);
    pokedexMeshes[0]->setMetallicTexture(metallic);
    pokedexMeshes[0]->setRoughnessTexture(roughnessIm);
    pokedexMeshes[0]->setAoTexture(nullptr);
    pokedexMeshes[0]->setEmissiveTexture(emissiveIm);
    pokedexMeshes[0]->setOpacityTexture(nullptr);
    pokedexMeshes[0]->setPosition(glm::vec3(-3, 0, 0));
    pokedexMeshes[0]->scale(glm::vec3(0.1, 0.1, 0.1));
    pokedexMeshes[0]->rotate(180, glm::vec3(0, 1, 0));
    engine.addMesh(pokedexMeshes[0]);

    std::vector<Mesh*> helmetMeshes = loader.loadModel("models/Helmet/DamagedHelmet.gltf");
    helmetMeshes[1]->setMetallicRoughnessTexture(VulkanImage::loadTexture("models/Helmet/Default_metalRoughness.jpg", &device));
    helmetMeshes[1]->setMetallicTexture(nullptr);
    helmetMeshes[1]->setRoughnessTexture(nullptr);
    helmetMeshes[1]->setAoTexture(VulkanImage::loadTexture("models/Helmet/Default_AO.jpg", &device));

    helmetMeshes[0]->setMetallicRoughnessTexture(helmetMeshes[1]->getMetallicRoughnessTexture());
    helmetMeshes[0]->setMetallicTexture(nullptr);
    helmetMeshes[0]->setRoughnessTexture(nullptr);
    helmetMeshes[0]->setAoTexture(helmetMeshes[1]->getAoTexture());
    for (const auto &item: helmetMeshes){
        item->setPosition(glm::vec3(5, 0, 0));

        item->setOpacityTexture(nullptr);
        engine.addMesh(item);
    }

    engine.getLightInfo().enabledPoints = 1;
    engine.getLightInfo().pointLights[0].color = glm::vec3(1, 1, 1);
    engine.getLightInfo().pointLights[0].intensity = 100;
    engine.getLightInfo().pointLights[0].position = glm::vec3(-3, -1, -5);
    engine.getLightInfo().ambientIntensity = 0.01f;
    engine.getLightInfo().gammaCorrect = 1.0/2.5f;
    while (!Window::getWindowInstance()->isWindowNeedToClose())
    {
        engine.update();
    }
    return 0;
}
