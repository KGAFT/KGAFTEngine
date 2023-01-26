#include <iostream>
#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "VulkanContext/GraphicsPipeline/VulkanShader.h"
int main(){
	VulkanInstance instance;
    Window::initWindow("HelloVulkan", 800, 600);
	std::cout<<instance.createInstance("KGAFTTest", true)<<std::endl;

    VkPhysicalDevice toCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(toCreate, Window::getWindowInstance(), instance.getInstance(), true);
    VulkanShader::loadShaderBlock("spir-v", &device);
    VulkanSwapChain swapChain(&device);
    VulkanRenderPass renderPass(&device, &swapChain);
    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        Window::getWindowInstance()->postRenderEvents();
    }
    renderPass.destroy();
    swapChain.destroy();
    //device.destroy();
	return 0;
}
