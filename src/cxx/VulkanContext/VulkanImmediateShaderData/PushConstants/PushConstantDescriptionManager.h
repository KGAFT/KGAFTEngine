//
// Created by daniil on 28.01.23.
//

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include "PushConstant.h"

#pragma once

class PushConstantDescriptionManager {
private:
    std::vector<VkPushConstantRange> pushConstantRanges;
    std::vector<PushConstant *> pushConstants;
public:
    void registerPushConstant(PushConstant *pushConstant) {
        for (const auto &item: pushConstantRanges) {
            if (item.stageFlags == pushConstant->getShaderStages()) {
                throw std::runtime_error("Error: you cannot add push constant with already existing flags");
            }
        }
        VkPushConstantRange range{};
        range.stageFlags = pushConstant->getShaderStages();
        range.size = pushConstant->getSize();
        range.offset = 0;
        pushConstantRanges.push_back(range);
        pushConstants.push_back(pushConstant);
    }

    std::vector<VkPushConstantRange> &getPushConstantRanges() {
        return pushConstantRanges;
    }

    void loadConstantsToShader(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout) {
        for (const auto &item: pushConstants) {
            vkCmdPushConstants(
                    commandBuffer,
                    pipelineLayout,
                    item->getShaderStages(),
                    0,
                    item->getSize(),
                    item->getData());
        }

    }
};