//
// Created by daniil on 26.01.23.
//

#pragma once

#include "VulkanSwapChainControl.h"
#include "../GraphicsPipeline/GraphicsPipeline.h"

#include "../VulkanBuffers/VertexBuffer.h"
#include "../VulkanBuffers/IndexBuffer.h"


class VulkanRenderingPipeline : public WindowResizeCallback{
private:
    VulkanSwapChainControl *control;
    VulkanRenderPass *renderPass;
    GraphicsPipeline* pipeline;;

    VulkanDevice *device;
    bool pause = false;

    unsigned int currentImage = 0;

    std::vector<VkCommandBuffer> commandBuffers;

    float clearColorValues[4] = {0,0,0,1};
public:
    VulkanRenderingPipeline(VulkanSwapChainControl *control, VulkanRenderPass *renderPass, GraphicsPipeline *pipeline,
                            VulkanDevice *device) : control(control), renderPass(renderPass), pipeline(pipeline),
                                                    device(device) {
        createCommandBuffers();
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
    }
    void beginRender(){
        currentImage = control->acquireNextImage();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        if (vkBeginCommandBuffer(commandBuffers[currentImage], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
    }

    void setClearColorValues(float r, float g, float b, float a){
        clearColorValues[0] = r;
        clearColorValues[1] = g;
        clearColorValues[2] = b;
        clearColorValues[3] = a;
    }

    void beginDraw(){
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->renderPass;
        renderPassInfo.framebuffer = renderPass->frameBuffers[currentImage];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = control->swapChain->swapChainExtent;
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { clearColorValues[0], clearColorValues[1], clearColorValues[2], clearColorValues[3] };
        clearValues[1].depthStencil = { 1.0f, 0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();



        vkCmdBeginRenderPass(commandBuffers[currentImage], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[currentImage], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);
    }
    void endDraw(){
        vkCmdEndRenderPass(commandBuffers[currentImage]);
        if (vkEndCommandBuffer(commandBuffers[currentImage]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
        control->submitCommandBuffers(&commandBuffers[currentImage], &currentImage);
    }

        


    void update() {
        unsigned int imageIndex = control->acquireNextImage();
        control->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    }

    void prepareToDestroy() {
        vkDeviceWaitIdle(device->getDevice());
    }

    unsigned int getCurrentImage(){
        return currentImage;
    }
    VkCommandBuffer getCurrentCommandBuffer(){
        return commandBuffers[currentImage];
    }
    VkPipelineLayout getPipelineLayout(){
        return pipeline->pipelineLayout;
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
