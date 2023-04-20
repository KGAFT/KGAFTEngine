#include <Vulkan/VulkanInstance.h>
#include <Vulkan/VulkanDevice/VulkanDevice.h>
#include "Window/Window.h"



int main() {
    Window::initWindow("KGAFTEngine", 800, 600);
    VulkanInstance instance;
    const char** exts;
    uint32_t extsAmount;
    exts = glfwGetRequiredInstanceExtensions(&extsAmount);
    std::vector<const char*> extensions;
    for(uint32_t c = 0; c<extsAmount; c++){
        extensions.push_back(exts[c]);
    }

    instance.createInstance("KGAFTEngine", true, extensions);
    std::map<VkPhysicalDevice, VkPhysicalDeviceProperties> devices = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()->getWindowSurface(instance.getInstance()));
    std::vector<VkPhysicalDevice> devicesToCreate;
    int c = 0;
    std::cout<<"Engine found supported devices in your's system, please select device to render: "<<std::endl;
    std::cout<<"ID:\tDevice name:"<<std::endl;
    for(const auto& el : devices){
        devicesToCreate.push_back(el.first);
        std::cout<<c<<"\t"<<el.second.deviceName<<std::endl;
        c++;
    }
    std::cout<<"Please select device, by entering id: ";
    std::cin>>c;
    VulkanDevice device(devicesToCreate[c],Window::getWindowInstance()->getWindowSurface(instance.getInstance()), instance.getInstance(), true);


    while (!Window::getWindowInstance()->isWindowNeedToClose()) {
        Window::getWindowInstance()->preRenderEvents();

        Window::getWindowInstance()->postRenderEvents();
    }
    return 0;
}
