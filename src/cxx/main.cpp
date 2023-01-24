#include <iostream>
#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
int main(){
	VulkanInstance instance;
    Window::initWindow("HelloVulkan", 800, 600);
	std::cout<<instance.createInstance("KGAFTTest", true)<<std::endl;
    for (const auto &item: VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance())){
        std::cout<<item.second.deviceName<<std::endl;
    }
    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        Window::getWindowInstance()->postRenderEvents();
    }
	return 0;
}
