#include <assimp/Importer.hpp>

#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "VulkanContext/VulkanLogger/DefaultVulkanLoggerCallback.h"

#include "Engine/TestRenderPipeline.h"
#include "Engine/PbrRenderPipeline.h"


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


    while(!Window::getWindowInstance()->isWindowNeedToClose()) {
        Window::getWindowInstance()->preRenderEvents();
        cameraManager.update();
        renderPipeline->startRenderProcess();
        renderPipeline->setPushConstantData(&data);
        renderPipeline->endRenderProcess();
        Window::getWindowInstance()->postRenderEvents();
    }
    return 0;
}
