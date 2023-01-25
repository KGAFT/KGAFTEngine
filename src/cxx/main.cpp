#include <iostream>
#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChain.h"

int main(){
	VulkanInstance instance;
    Window::initWindow("HelloVulkan", 800, 600);
	std::cout<<instance.createInstance("KGAFTTest", true)<<std::endl;
    VkPhysicalDevice toCreate = VK_NULL_HANDLE;
    for (const auto &item: VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance())){
        std::cout<<item.second.deviceName<<std::endl;
        if(toCreate==VK_NULL_HANDLE){
            toCreate = item.first;
        }
    }
    VulkanDevice device(toCreate, Window::getWindowInstance(), instance.getInstance(), true);
    VulkanSwapChain swapChain(&device);
    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        Window::getWindowInstance()->postRenderEvents();
    }
	return 0;
}
