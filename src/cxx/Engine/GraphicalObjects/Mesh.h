//
// Created by Daniil on 05.02.2023.
//
#include <glm/glm.hpp>
#include <vector>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <Vulkan/VulkanBuffers/VertexBuffer.h>

#include <Vulkan/VulkanBuffers/IndexBuffer.h>
#include <Vulkan/VulkanImage/VulkanImage.h>

#pragma once

struct MeshData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Mesh {
private:
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    VertexBuffer *data;
    IndexBuffer *indices;
    VulkanImage *albedoTexture = nullptr;
    VulkanImage *normalTexture = nullptr;
    VulkanImage *metallicTexture = nullptr;
    VulkanImage *roughnessTexture = nullptr;
    VulkanImage *aoTexture = nullptr;
    VulkanImage *emissiveTexture = nullptr;
    VulkanImage *metallicRoughnessTexture = nullptr;
    VulkanImage* opacityTexture = nullptr;
public:
    Mesh(VertexBuffer *data, IndexBuffer *indices) : data(data),
                                                     indices(indices) {}

    void rotate(float degrees, glm::vec3 axesToRotate) {
        worldMatrix = glm::rotate(worldMatrix, glm::radians(degrees), axesToRotate);
    }

    void scale(glm::vec3 scaling) {
        worldMatrix = glm::scale(worldMatrix, scaling);
    }

    void setPosition(glm::vec3 position) {
        worldMatrix = glm::translate(worldMatrix, position);
    }

    void draw(VkCommandBuffer commandBuffer) {
        data->bind(commandBuffer);
        indices->bind(commandBuffer);
        indices->draw(commandBuffer);
    }

    const glm::mat4 &getWorldMatrix() const {
        return worldMatrix;
    }

    VulkanImage *getAlbedoTexture() const {
        return albedoTexture;
    }

    VulkanImage *getNormalTexture() const {
        return normalTexture;
    }

    VulkanImage *getMetallicTexture() const {
        return metallicTexture;
    }

    VulkanImage *getRoughnessTexture() const {
        return roughnessTexture;
    }

    VulkanImage *getAoTexture() const {
        return aoTexture;
    }

    VulkanImage *getEmissiveTexture() const {
        return emissiveTexture;
    }

    VulkanImage *getMetallicRoughnessTexture() const {
        return metallicRoughnessTexture;
    }

    void setAlbedoTexture(VulkanImage *albedoTexture) {
        Mesh::albedoTexture = albedoTexture;
    }

    void setNormalTexture(VulkanImage *normalTexture) {
        Mesh::normalTexture = normalTexture;
    }

    void setMetallicTexture(VulkanImage *metallicTexture) {
        Mesh::metallicTexture = metallicTexture;
    }

    void setRoughnessTexture(VulkanImage *roughnessTexture) {
        Mesh::roughnessTexture = roughnessTexture;
    }

    void setAoTexture(VulkanImage *aoTexture) {
        Mesh::aoTexture = aoTexture;
    }

    void setEmissiveTexture(VulkanImage *emissiveTexture) {
        Mesh::emissiveTexture = emissiveTexture;
    }

    void setMetallicRoughnessTexture(VulkanImage *metallicRoughnessTexture) {
        Mesh::metallicRoughnessTexture = metallicRoughnessTexture;
    }

    VulkanImage *getOpacityTexture() {
        return opacityTexture;
    }

    void setOpacityTexture(VulkanImage *opacityTexture) {
        Mesh::opacityTexture = opacityTexture;
    }

    ~Mesh() {
        destroy();
    }

    void destroy() {

        delete data;
        delete indices;
    }
};