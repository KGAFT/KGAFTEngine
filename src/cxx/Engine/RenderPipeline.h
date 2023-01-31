#pragma once
#include "../VulkanContext/VulkanDevice/VulkanDevice.h"
#include "../VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "../VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "../VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "../VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"
#include <glm.hpp>
#include "../VulkanContext/VulkanImage/VulkanSampler.h"

struct PushConstantData {
	glm::mat4 cameraMatrix;
	glm::mat4 worldMatrix;
};

struct UniformBufferData {
	glm::vec2 firstAdder;
	glm::vec3 secondAdder;
	glm::vec4 amplifierFirst;
	float amplifierSecond;
};

class RenderPipeline : public PreRenderPassCallback, RenderPassCallback {
private:
	VulkanDevice* device;
	VulkanSwapChain* swapChain;
	VulkanShader* shader;
	VulkanRenderPass* renderPass;
	VulkanSwapChainControl* control;
	GraphicsPipeline* pipeline;
	PushConstantDescriptionManager* pushConstantsDescription;
	VulkanDescriptorPool* descriptorPool;
	VulkanDescriptorSetLayout* descriptorSetLayout;
	VertexBufferDescriptionManager* vertexManager;
	std::vector<VulkanDescriptorSet*> uniformDescriptorSets;
	std::vector<VulkanDescriptorSet*> textureDescriptorSets;
	PipelineConfiguration::PipelineConfigInfo config;
	VulkanRenderingPipeline* renderingPipeline;
	VulkanSampler* sampler;
	UniformBuffer* ubo;
	PushConstant* pushConstant;
public:
	RenderPipeline(VulkanDevice* device, Window* window, const char* targetDirectoryWithShaders) {
		this->device = device;
		swapChain = new VulkanSwapChain(device, window->getWidth(), window->getHeight());
		shader = VulkanShader::loadShaderBlock(targetDirectoryWithShaders, device);
		renderPass = new VulkanRenderPass(device, swapChain);
		control = new VulkanSwapChainControl(swapChain, renderPass, device);
		config = PipelineConfiguration::defaultPipelineConfigInfo(Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
		config.renderPass = renderPass->getRenderPass();
		vertexManager = new VertexBufferDescriptionManager();
		vertexManager->createBinding(0, 8 * sizeof(float));
		vertexManager->registerAttribute(0, 0, 3);
		vertexManager->registerAttribute(0, 1, 3);
		vertexManager->registerAttribute(0, 2, 2);

		pushConstantsDescription = new PushConstantDescriptionManager();
		pushConstant = new PushConstant(sizeof(PushConstantData), VERTEX_SHADER_PUSH_CONSTANT);
		pushConstantsDescription->registerPushConstant(pushConstant);

		sampler = new VulkanSampler(device, 1);
		ubo = new UniformBuffer(device, sizeof(UniformBufferData), 0, VK_SHADER_STAGE_FRAGMENT_BIT);
		std::vector<VkDescriptorPoolSize> sizes;
		VkDescriptorPoolSize uniformDesc{};
		uniformDesc.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uniformDesc.descriptorCount = 1;

		VkDescriptorPoolSize textureDesc{};
		textureDesc.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		textureDesc.descriptorCount = 1;
		for (int i = 0; i < 3; i++) {
			sizes.push_back(uniformDesc);
			sizes.push_back(textureDesc);
		}
		std::vector<IVulkanDescriptorSetElement*> descElements;
		descElements.push_back(ubo);
		descElements.push_back(sampler);
		descriptorPool = new VulkanDescriptorPool(device, sizes);
		descriptorSetLayout = new VulkanDescriptorSetLayout(device, descElements);

		for (int i = 0; i < 3; i++) {
			uniformDescriptorSets.push_back(new VulkanDescriptorSet(device, descriptorPool, descriptorSetLayout));
			textureDescriptorSets.push_back(new VulkanDescriptorSet(device, descriptorPool, descriptorSetLayout));
		}
		
		pipeline = new GraphicsPipeline(shader, device, config, vertexManager, pushConstantsDescription, descriptorSetLayout);
		renderingPipeline = new VulkanRenderingPipeline(control, renderPass, pipeline, device);
		window->registerResizeCallback(renderingPipeline);
		renderingPipeline->createCommandBuffers();
		renderingPipeline->registerPreRenderPassCallback(this);
		renderingPipeline->registerRenderPassCallback(this);
	}
	void invokePreRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout,  unsigned int imageIndex) override {
		textureDescriptorSets[imageIndex]->bind(commandBuffer, layout);
		uniformDescriptorSets[imageIndex]->bind(commandBuffer, layout);
	}
	void invokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {
		//pushConstantsDescription->loadConstantsToShader(commandBuffer, layout);
	}

	void update() {
		renderingPipeline->redrawCommandBuffers();
		
	}
};
