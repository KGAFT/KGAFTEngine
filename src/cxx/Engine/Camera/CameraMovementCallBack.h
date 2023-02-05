//
// Created by Danil on 13.09.2022.
//

#pragma once


#include "Camera.h"
#include "GLFW/glfw3.h"
#include "../Window/KeyBoardCallBack.h"

class CameraMovementCallBack : public KeyBoardCallBack {
private:
    Camera *camera;
    float sensitivity;
    unsigned int keys[6]{
            GLFW_KEY_W,
            GLFW_KEY_A,
            GLFW_KEY_S,
            GLFW_KEY_D,
            GLFW_KEY_SPACE,
            GLFW_KEY_C
    };
public:
    CameraMovementCallBack(Camera *camera, float sensitivity) : camera(camera),
                                                                sensitivity(sensitivity) {}

    unsigned int getKeyCodeAmount() override {
        return sizeof(keys) / sizeof(unsigned int);
    }

    unsigned int *getKeyCodes() override{
        return keys;
    }

    void action(unsigned int keyCode) override {
        float fwbAmplifier = 0;
        float leftRightAmplifier = 0;
        float upDownAmplifier = 0;
        switch (keyCode) {
            case GLFW_KEY_W:
                fwbAmplifier += sensitivity;
                break;
            case GLFW_KEY_S:
                fwbAmplifier -= sensitivity;
                break;
            case GLFW_KEY_A:
                leftRightAmplifier += sensitivity;
                break;
            case GLFW_KEY_D:
                leftRightAmplifier -= sensitivity;
                break;
            case GLFW_KEY_SPACE:
                upDownAmplifier += sensitivity;
                break;
            case GLFW_KEY_C:
                upDownAmplifier -= sensitivity;
                break;
        }
        camera->moveCam(fwbAmplifier, leftRightAmplifier, upDownAmplifier);

    }

    Camera *getCamera() const {
        return camera;
    }

    void setCamera(Camera *camera) {
        CameraMovementCallBack::camera = camera;
    }

};



