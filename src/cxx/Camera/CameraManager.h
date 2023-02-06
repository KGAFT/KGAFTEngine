//
// Created by Daniil on 26.12.2022.
//

#pragma once

#include "Camera.h"
#include "CameraMovementCallBack.h"
#include "CameraRotationCallBack.h"
#include "../Window/Window.h"
#include "CameraResizeCallBack.h"

struct PushConstantData{
    glm::mat4 cameraMatrix;
    glm::mat4 modelMatrix;
};

class CameraManager{
private:
    static inline CameraManager* cameraManagerInstance = nullptr;

private:
    float fov = 120.0f;
    Camera* currentCamera = new Camera(glm::vec3(0, 0, 2.0f), Window::getWindowInstance()->getWidth(), Window::getWindowInstance()->getHeight());
    CameraMovementCallBack* currentCameraMovementCallback = new CameraMovementCallBack(currentCamera, 0.002f);
    CameraRotationCallBack* currentCameraRotationCallBack = new CameraRotationCallBack(currentCamera, 0.5f);
    CameraResizeCallBack* cameraResizeCallBack = new CameraResizeCallBack(currentCamera);
    PushConstantData* data;

public:
    CameraManager(PushConstantData* data){
        this->data = data;
        Window::getWindowInstance()->registerResizeCallback(cameraResizeCallBack);
        Window::getWindowInstance()->registerKeyboardCallBack(currentCameraMovementCallback);
        Window::getWindowInstance()->registerMouseCallBack(currentCameraRotationCallBack);
    }
    void update(){
        glm::mat4 camMat = currentCamera->getCameraMatrix(fov, 0.001f, 100.0f);
        data->cameraMatrix = camMat;
        data->modelMatrix = glm::mat4(1.0f);
    }
    void setCurrentCamera(Camera* currentCamera){
        this->currentCamera = currentCamera;
        currentCameraRotationCallBack->setCamera(this->currentCamera);
        currentCameraMovementCallback->setCamera(this->currentCamera);
        cameraResizeCallBack->setCurrentCamera(this->currentCamera);
    }
};

