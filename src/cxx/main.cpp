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

struct PushConstantData {
    glm::vec2 offset;
    alignas(16) glm::vec3 secondOffset;
};

int main(){
	VulkanInstance instance;
    Window::initWindow("HelloVulkan", 1920, 1080);
	std::cout<<instance.createInstance("KGAFTTest", true)<<std::endl;

    VkPhysicalDevice toCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(toCreate, Window::getWindowInstance(), instance.getInstance(), true);
    VulkanShader* shader = VulkanShader::loadShaderBlock("E:/documents/projects/GitHub/KGAFTEngine/src/spir-v", &device);
    VulkanSwapChain swapChain(&device, Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    VulkanRenderPass renderPass(&device, &swapChain);
    VulkanSwapChainControl control(&swapChain, &renderPass, &device);
    PipelineConfiguration::PipelineConfigInfo config = PipelineConfiguration::defaultPipelineConfigInfo(Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    config.renderPass = renderPass.getRenderPass();

    VertexBufferDescriptionManager manager;
    manager.createBinding(0, 3*sizeof(float));
    manager.registerAttribute(0, 0, 3);

    float vertices[]{0.0f, -0.5f, 0.0f,
                     0.5f, 0.5f, 0.0f,
                     -0.5f, 0.5f, 0.0f};

    VertexBuffer buffer(3, sizeof(float), 3, &device, vertices);
    PushConstantDescriptionManager pcsDesc;
    PushConstantData data{ glm::vec2(0.1,0.1), glm::vec3(-0.1,-0.1,-0.1) };
    PushConstant pc(sizeof(PushConstantData), VERTEX_SHADER_PUSH_CONSTANT);
    pc.setData(&data);
    pcsDesc.registerPushConstant(&pc);
    GraphicsPipeline pipeline(shader, &device, config, &manager, &pcsDesc);
    VulkanRenderingPipeline renderPipeline(&control, &renderPass, &pipeline, &device,&buffer);
    Window::getWindowInstance()->registerResizeCallback(&renderPipeline);
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
