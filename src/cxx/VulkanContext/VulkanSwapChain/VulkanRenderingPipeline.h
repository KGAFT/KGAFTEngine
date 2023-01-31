//
// Created by daniil on 26.01.23.
//

#pragma once

#include "VulkanSwapChainControl.h"
#include "../GraphicsPipeline/GraphicsPipeline.h"
#include "../VulkanDescriptorSet/VulkanDescriptorSet.h"
#include "../VulkanImmediateShaderData/UniformBuffer/UniformBuffer.h"
#include "../VulkanBuffers/VertexBuffer.h"
#include "../VulkanBuffers/IndexBuffer.h"

class PreRenderPassCallback {
public:
    virtual void invokePreRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) = 0;
};

class RenderPassCallback {
public:
    virtual void invokeRender(VkCommandBuffer commandBuffer, VkPipelineLayout layout, unsigned int imageIndex) = 0;
};

class VulkanRenderingPipeline : public WindowResizeCallback{
private:
    VulkanSwapChainControl *control;
    VulkanRenderPass *renderPass;
    GraphicsPipeline* pipeline;;

    VulkanDevice *device;
    bool pause = false;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<PreRenderPassCallback*> preRenderPassCallbacks;
    std::vector<RenderPassCallback*> renderPassCallbacks;
public:
    VulkanRenderingPipeline(VulkanSwapChainControl *control, VulkanRenderPass *renderPass, GraphicsPipeline *pipeline,
                            VulkanDevice *device) : control(control), renderPass(renderPass), pipeline(pipeline),
                                                    device(device) {
        
    }
    void registerPreRenderPassCallback(PreRenderPassCallback* cb) {
        preRenderPassCallbacks.push_back(cb);
    }
    void registerRenderPassCallback(RenderPassCallback* cb) {
        renderPassCallbacks.push_back(cb);
    }
public:
    void createCommandBuffers() {
        commandBuffers.resize(control->swapChain->swapChainImages.size());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device->getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(device->getDevice(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
        unsigned int indices[]{
            0,1,2
        };
    }

    void redrawCommandBuffers() {
        if (!pause) {
            unsigned int i = control->acquireNextImage();

          

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            for (const auto& item : preRenderPassCallbacks) {
                item->invokePreRender(commandBuffers[i], pipeline->pipelineLayout, i);
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass->renderPass;
            renderPassInfo.framebuffer = renderPass->frameBuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = control->swapChain->swapChainExtent;
            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = { 0.1f, 0.5f, 0.3f, 1.0f };
            clearValues[1].depthStencil = { 1.0f, 0 };
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();
            
            

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);
            for (const auto& item : renderPassCallbacks) {
                item->invokeRender(commandBuffers[i], pipeline->pipelineLayout, i);
            }
            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
            control->submitCommandBuffers(&commandBuffers[i], &i);
        }
        
    }

    void update() {
        unsigned int imageIndex = control->acquireNextImage();
        control->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    }

    void prepareToDestroy() {
        vkDeviceWaitIdle(device->getDevice());
    }

    void resized(int width, int height) override {
        pause = true;
        prepareToDestroy();
        control->swapChain->recreate((unsigned int)width, (unsigned int)height);
        control->renderPass->recreate();
        pipeline->recreate(width, height, control->renderPass->getRenderPass());
        pause = false;
    }
};
