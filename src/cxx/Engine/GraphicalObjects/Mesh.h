//
// Created by Daniil on 05.02.2023.
//
#include <glm/glm.hpp>
#include <vector>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "../../VulkanContext/VulkanBuffers/VertexBuffer.h"
#include "../../VulkanContext/VulkanBuffers/IndexBuffer.h"
#include "../../VulkanContext/VulkanImage/VulkanTexture.h"

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
    VulkanTexture *albedoTexture = nullptr;
    VulkanTexture *normalTexture = nullptr;
    VulkanTexture *metallicTexture = nullptr;
    VulkanTexture *roughnessTexture = nullptr;
    VulkanTexture *aoTexture = nullptr;
    VulkanTexture *emissiveTexture = nullptr;
    VulkanTexture *metallicRoughnessTexture = nullptr;
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

    VulkanTexture *getAlbedoTexture() const {
        return albedoTexture;
    }

    VulkanTexture *getNormalTexture() const {
        return normalTexture;
    }

    VulkanTexture *getMetallicTexture() const {
        return metallicTexture;
    }

    VulkanTexture *getRoughnessTexture() const {
        return roughnessTexture;
    }

    VulkanTexture *getAoTexture() const {
        return aoTexture;
    }

    VulkanTexture *getEmissiveTexture() const {
        return emissiveTexture;
    }

    VulkanTexture *getMetallicRoughnessTexture() const {
        return metallicRoughnessTexture;
    }

    void setAlbedoTexture(VulkanTexture *albedoTexture) {
        Mesh::albedoTexture = albedoTexture;
    }

    void setNormalTexture(VulkanTexture *normalTexture) {
        Mesh::normalTexture = normalTexture;
    }

    void setMetallicTexture(VulkanTexture *metallicTexture) {
        Mesh::metallicTexture = metallicTexture;
    }

    void setRoughnessTexture(VulkanTexture *roughnessTexture) {
        Mesh::roughnessTexture = roughnessTexture;
    }

    void setAoTexture(VulkanTexture *aoTexture) {
        Mesh::aoTexture = aoTexture;
    }

    void setEmissiveTexture(VulkanTexture *emissiveTexture) {
        Mesh::emissiveTexture = emissiveTexture;
    }

    void setMetallicRoughnessTexture(VulkanTexture *metallicRoughnessTexture) {
        Mesh::metallicRoughnessTexture = metallicRoughnessTexture;
    }

    ~Mesh() {
        destroy();
    }

    void destroy() {

        delete data;
        delete indices;
    }
};