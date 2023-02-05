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

class Mesh{
private:
    glm::mat4 worldMatrix = glm::mat4(1.0f);
    VertexBuffer* data;
    IndexBuffer* indices;
    std::vector<VulkanTexture*> textures;
public:
    Mesh(VertexBuffer *data,  IndexBuffer *indices) : data(data),
                                                         indices(indices) {}
    void addTexture(VulkanTexture* texture){
        textures.push_back(texture);
    }
    void rotate(float degrees, glm::vec3 axesToRotate){
        worldMatrix = glm::rotate(worldMatrix, glm::radians(degrees), axesToRotate);
    }
    void scale(glm::vec3 scaling){
        worldMatrix = glm::scale(worldMatrix, scaling);
    }
    void setPosition(glm::vec3 position){
        worldMatrix = glm::translate(worldMatrix, position);
    }
    void draw(VkCommandBuffer commandBuffer){
        data->bind(commandBuffer);
        indices->bind(commandBuffer);
        indices->draw(commandBuffer);
    }
    ~Mesh(){
        destroy();
    }

    void destroy(){

        delete data;
        delete indices;
    }
};