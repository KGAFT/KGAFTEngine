//
// Created by Daniil on 9/13/2022.
//


#include "Camera.h"
#include "../Window/MouseMovedCallBack.h"

#pragma once

class CameraRotationCallBack : public MouseMovedCallBack {
private:
    Camera *camera;
    float sensitivity;
public:
    CameraRotationCallBack(Camera *camera, float sensitivity) {
        this->camera = camera;
        this->sensitivity = sensitivity;
    }

    void mouseMoved(int xChange, int yChange) override {
        camera->rotateCam(sensitivity * xChange, sensitivity * yChange);
    }

    Camera *getCamera() const {
        return camera;
    }

    void setCamera(Camera *camera) {
        CameraRotationCallBack::camera = camera;
    }
};



