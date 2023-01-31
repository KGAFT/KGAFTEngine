#include <iostream>
#include <glm.hpp>
#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "VulkanContext/GraphicsPipeline/VulkanShader.h"
#include "VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"
#include "VulkanContext/VulkanImmediateShaderData/UniformBuffer/UniformBuffer.h"
#include "VulkanContext/VulkanDescriptorSet/VulkanDescriptorPool.h"
#include "VulkanContext/VulkanDescriptorSet/VulkanDescriptorSet.h"
#include "VulkanContext/VulkanImage/VulkanImage.h"
#include "Engine/RenderPipeline.h"


int main(){
	VulkanInstance instance;
    Window::initWindow("HelloVulkan", 800, 600);
	std::cout<<instance.createInstance("KGAFTTest", true)<<std::endl;

    VkPhysicalDevice toCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(toCreate, Window::getWindowInstance(), instance.getInstance(), true);
    RenderPipeline renderPipeline(&device, Window::getWindowInstance(), "E:/documents/projects/GitHub/KGAFTEngine/src/spir-v");
    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        renderPipeline.update();

        Window::getWindowInstance()->postRenderEvents();
    }

	return 0;
}
