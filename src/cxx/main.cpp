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

struct PushConstantData {
    glm::vec2 offset;
    alignas(16) glm::vec3 secondOffset;
};

struct UniformBufferData {
    glm::vec2 offset;
};

int main(){
	VulkanInstance instance;
    Window::initWindow("HelloVulkan", 800, 600);
	std::cout<<instance.createInstance("KGAFTTest", true)<<std::endl;

    VkPhysicalDevice toCreate = VulkanDevice::enumerateSupportedDevices(instance.getInstance(), Window::getWindowInstance()).crbegin()->first;
    VulkanDevice device(toCreate, Window::getWindowInstance(), instance.getInstance(), true);
    VulkanShader* shader = VulkanShader::loadShaderBlock("C:/Users/Daniil/Documents/KGAFTEngine/src/spir-v", &device);
    VulkanSwapChain swapChain(&device, Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    VulkanRenderPass renderPass(&device, &swapChain);
    VulkanSwapChainControl control(&swapChain, &renderPass, &device);
    VulkanImage image = VulkanImage::loadTextureFromFiles(&device, "C:/Users/Daniil/Documents/KGAFTEngine/x64/Debug/texture.jpg");
    PipelineConfiguration::PipelineConfigInfo config = PipelineConfiguration::defaultPipelineConfigInfo(Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    config.renderPass = renderPass.getRenderPass();

    VertexBufferDescriptionManager manager;
    manager.createBinding(0, 3*sizeof(float));
    manager.registerAttribute(0, 0, 3);

    float vertices[]{0.0f, -0.5f, 0.0f,
                     0.5f, 0.5f, 0.0f,
                     -0.5f, 0.5f, 0.0f};

    VertexBuffer buffer(3, sizeof(float), 3, &device, vertices);
    
    VulkanDescriptorSetLayout layout();
    
    
    PushConstantDescriptionManager pcsDesc;
    PushConstantData data{ glm::vec2(0.1,0.1), glm::vec3(-0.1,-0.1,-0.1) };
    PushConstant pc(sizeof(PushConstantData), VERTEX_SHADER_PUSH_CONSTANT);
    pc.setData(&data);
    pcsDesc.registerPushConstant(&pc);

    UniformBuffer ubo(&device, sizeof(UniformBufferData), 0, VK_SHADER_STAGE_VERTEX_BIT);
    std::vector<IVulkanDescriptorSetElement*> descriptorElements;
    std::vector<VkDescriptorPoolSize> sizes;
    VkDescriptorPoolSize sizeDesc{};
    sizeDesc.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    sizeDesc.descriptorCount = 1;
    sizes.push_back(sizeDesc);
    sizes.push_back(sizeDesc);
    sizes.push_back(sizeDesc);
    descriptorElements.push_back(&ubo);
    VulkanDescriptorSetLayout dscLayout(&device, descriptorElements);

    VulkanDescriptorPool descriptorPool(&device, sizes);
    VulkanDescriptorSet descriptorSet(&device, &descriptorPool, &dscLayout);
    VulkanDescriptorSet secondSet(&device, & descriptorPool, & dscLayout);
    VulkanDescriptorSet thirdSet(&device, &descriptorPool, &dscLayout);

    std::vector<VulkanDescriptorSet*> descriptorSets;
    descriptorSets.push_back(&descriptorSet);
    descriptorSets.push_back(&secondSet);
    descriptorSets.push_back(&thirdSet);

    UniformBufferData uboData{ glm::vec2(0, -0.3f) };
    
    GraphicsPipeline pipeline(shader, &device, config, &manager, &pcsDesc, &dscLayout);
    VulkanRenderingPipeline renderPipeline(&control, &renderPass, &pipeline, &device,&buffer, descriptorSets, &ubo);
    Window::getWindowInstance()->registerResizeCallback(&renderPipeline);
    renderPipeline.createCommandBuffers();

    while(!Window::getWindowInstance()->isWindowNeedToClose()){
        ubo.write(&uboData);
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
