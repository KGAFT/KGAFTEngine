//
// Created by Daniil on 02.02.2023.
//
#pragma once

#include "RenderPipeline.h"
#include "../Camera/CameraManager.h"

class TestRenderPipeline : RenderPipeline{
public:
    TestRenderPipeline(VulkanDevice* device, Window* window) : RenderPipeline(device, window) {

    }

};