//
// Created by Daniil on 2/9/2023.
//

#pragma once

#include "PbrRenderPipeline.h"

class PbrEngine {
private:
    PbrRenderPipeline *pbrRenderPipeline;
    Window *window;
    std::vector<Mesh *> meshes;
    PushConstantData pushConstantData{};
    CameraManager *cameraManager;

public:
    PbrEngine(Window *window, VulkanDevice *device) {
        pbrRenderPipeline = new PbrRenderPipeline(device, window);
        pbrRenderPipeline->getPushConstant(0)->setData(&pushConstantData);
        cameraManager = new CameraManager(&pushConstantData);
    }

    void drawMeshes() {
        cameraManager->update();
        pbrRenderPipeline->beginRender();
        unsigned int currentImage;
        VkCommandBuffer commandBuffer;
        VkPipelineLayout layout;
        pbrRenderPipeline->beginDraw(&currentImage, &commandBuffer, &layout);
        for (const auto &item: meshes) {
            pushConstantData.modelMatrix = item->getWorldMatrix();
            processMeshTextures(item);
            pbrRenderPipeline->updateShaderData();
            item->draw(commandBuffer);
        }
        pbrRenderPipeline->endDraw();
    }

    void update() {
        Window::getWindowInstance()->preRenderEvents();
        drawMeshes();
        Window::getWindowInstance()->postRenderEvents();
    }

    LightInfo &getLightInfo() {
        return pbrRenderPipeline->getLightInfo();
    }

    void addMesh(Mesh *mesh) {
        meshes.push_back(mesh);
    }

    const vector<Mesh *> &getMeshes() const {
        return meshes;
    }

private:
    void processMeshTextures(Mesh *mesh) {
        pbrRenderPipeline->getLightInfo().aoEnabled = mesh->getAoTexture() != nullptr;
        pbrRenderPipeline->getLightInfo().emissiveEnabled = mesh->getEmissiveTexture() != nullptr;
        pbrRenderPipeline->getLightInfo().combinedMetallicRoughness = mesh->getMetallicRoughnessTexture() != nullptr;
        pbrRenderPipeline->getLightInfo().opacityEnabled = mesh->getOpacityTexture()!=nullptr;
        if (mesh->getAlbedoTexture() == nullptr) {
            throw std::runtime_error("Error: you need to specify albedo for your mesh");
        }
        if (mesh->getNormalTexture() == nullptr) {
            throw std::runtime_error("Error: you need to specify normals map for your mesh");
        }
        if ((mesh->getMetallicTexture() == nullptr && mesh->getRoughnessTexture() == nullptr &&
             mesh->getMetallicRoughnessTexture() == nullptr)) {
            throw std::runtime_error("Error: you need to specify metallic and roughness textures!");
        }

        pbrRenderPipeline->setSamplerTexture(mesh->getAlbedoTexture(), 1);
        pbrRenderPipeline->setSamplerTexture(mesh->getNormalTexture(), 2);
        if (mesh->getMetallicRoughnessTexture() == nullptr) {
            pbrRenderPipeline->setSamplerTexture(mesh->getAlbedoTexture(), 7);
        } else {
            pbrRenderPipeline->setSamplerTexture(mesh->getMetallicRoughnessTexture(), 7);
        }
        if (mesh->getMetallicTexture() == nullptr || mesh->getRoughnessTexture() == nullptr) {
            pbrRenderPipeline->setSamplerTexture(mesh->getAlbedoTexture(), 3);
            pbrRenderPipeline->setSamplerTexture(mesh->getAlbedoTexture(), 4);

        } else {
            pbrRenderPipeline->setSamplerTexture(mesh->getMetallicTexture(), 3);
            pbrRenderPipeline->setSamplerTexture(mesh->getRoughnessTexture(), 4);

        }
        if (mesh->getAoTexture() == nullptr) {
            pbrRenderPipeline->setSamplerTexture(mesh->getAlbedoTexture(), 5);
        } else {
            pbrRenderPipeline->setSamplerTexture(mesh->getAoTexture(), 5);
        }
        if (mesh->getEmissiveTexture() == nullptr) {
            pbrRenderPipeline->setSamplerTexture(mesh->getAlbedoTexture(), 6);
        } else {
            pbrRenderPipeline->setSamplerTexture(mesh->getEmissiveTexture(), 6);
        }
        pbrRenderPipeline->setSamplerTexture(mesh->getOpacityTexture(), 8);
    }
};
