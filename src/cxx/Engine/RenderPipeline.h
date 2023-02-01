#pragma once
#include "../VulkanContext/VulkanDevice/VulkanDevice.h"
#include "../VulkanContext/VulkanSwapChain/VulkanSwapChain.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderPass.h"
#include "../VulkanContext/VulkanSwapChain/VulkanSwapChainControl.h"
#include "../VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "../VulkanContext/GraphicsPipeline/GraphicsPipeline.h"
#include "../VulkanContext/VulkanSwapChain/VulkanRenderingPipeline.h"
#include <glm/glm.hpp>
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

    VertexBuffer* vertexBuffer;
    VertexBuffer* normalsBuffer;
    VertexBuffer* uvsBuffer;
    IndexBuffer* indexBuffer;
    VulkanImage* textureImage;
    VulkanTexture* texture;

    UniformBufferData uboDat;
    PushConstantData pushConstantDat;
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
        float vertices[]{-0.5f, -0.5f, 1.0f,
                        0.5f, -0.5f, 1.0f,
                        0.5f, 0.5f, 1.0f,
                        -0.5f, 0.5f, 1.0f};
        float normals[]{
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f
        };
        float uvs[]{
                1.0f, 0.0f,
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f
        };

        unsigned int indices[]{
                0, 1, 2, 2, 3, 0
        };
        vertexBuffer = new VertexBuffer(4, sizeof(float), 3, device, vertices);
        normalsBuffer = new VertexBuffer(4, sizeof(float), 3, device, normals);
        uvsBuffer = new VertexBuffer(4, sizeof(float), 2, device, uvs);
        indexBuffer = new IndexBuffer(device, indices, 6);
        textureImage = new VulkanImage(VulkanImage::loadTextureFromFiles(device, "texture.tga"));
        texture = new VulkanTexture(textureImage, device);
        sampler->setTexture(texture);
        pushConstantDat.cameraMatrix = glm::mat4(1.0f);
        pushConstantDat.worldMatrix = glm::mat4(1.0f);
        pushConstant->setData(&pushConstantDat);
        for(int i = 0; i<3; i++){
            textureDescriptorSets[i]->write(sampler, 1);
            uniformDescriptorSets[i]->write(ubo, 0);
        }
	}
	void invokePreRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout,  unsigned int imageIndex) override {
        ubo->write(&uboDat);
        uniformDescriptorSets[imageIndex]->write(ubo, 0, 0);
		textureDescriptorSets[imageIndex]->bind(commandBuffer, layout);
		uniformDescriptorSets[imageIndex]->bind(commandBuffer, layout);
	}
	void invokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) override {
		pushConstant->setData(&pushConstantDat);
        pushConstantsDescription->loadConstantsToShader(commandBuffer, layout);


        vertexBuffer->bind(commandBuffer);
        normalsBuffer->bind(commandBuffer);
        uvsBuffer->bind(commandBuffer);
        indexBuffer->bind(commandBuffer);
        indexBuffer->draw(commandBuffer);
	}

	void update() {
		renderingPipeline->redrawCommandBuffers();
		
	}
};
