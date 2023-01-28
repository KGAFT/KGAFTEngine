#include <iostream>
#include "VulkanContext/VulkanInstance.h"
#include "VulkanContext/VulkanDevice/VulkanDevice.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "VulkanContext/GraphicsPipeline/VulkanShader.h"
#include "VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"

int main(){
	VulkanInstance instance;
    Window::initWindow("HelloVulkan", 800, 600);
	std::cout<<instance.createInstance("KGAFTTest", true)<<std::endl;

    VkPhysicalDevice toCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(toCreate, Window::getWindowInstance(), instance.getInstance(), true);
    VulkanShader* shader = VulkanShader::loadShaderBlock("spir-v", &device);
    VulkanSwapChain swapChain(&device);
    VulkanRenderPass renderPass(&device, &swapChain);
    VulkanSwapChainControl control(&swapChain, &renderPass, &device);
    PipelineConfiguration::PipelineConfigInfo config = PipelineConfiguration::defaultPipelineConfigInfo(Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    config.renderPass = renderPass.getRenderPass();

    VertexBufferManager manager;
    manager.createBinding(0, 3*sizeof(float));
    manager.registerAttribute(0, 0, 3);

    float vertices[]{0.0f, -0.5f, 0.0f,
                     0.5f, 0.5f, 0.0f,
                     -0.5f, 0.5f, 0.0f};

    VertexBuffer buffer(3, sizeof(float), 3, &device, vertices);

    GraphicsPipeline pipeline(shader, &device, config, &manager);
    VulkanRenderingPipeline renderPipeline(&control, &renderPass, &pipeline, &device,&buffer);
    renderPipeline.createCommandBuffers();

    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        renderPipeline.redrawCommandBuffers();

        Window::getWindowInstance()->postRenderEvents();
    }


    renderPipeline.prepareToDestroy();
    buffer.destroy();
    renderPass.destroy();
    swapChain.destroy();
    device.destroy();
	return 0;
}
