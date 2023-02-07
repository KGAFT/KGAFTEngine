
// Created by daniil on 08.09.22.
//


#include "glm/glm.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#pragma once



class Camera
{
private:
    glm::vec3 position;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);

    int viewPortWidth;
    int viewPortHeight;

public:
    Camera(glm::vec3 position, int viewPortWidth, int viewPortHeight)
    {
        this->position = position;
        this->viewPortWidth = viewPortWidth;
        this->viewPortHeight = viewPortHeight;
    }

    glm::mat4 getCameraMatrix(float FOVdeg, float nearPlane, float farPlane)
    {
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::lookAt(position, position + orientation, up);
        projection = glm::perspective(glm::radians(FOVdeg), (float)viewPortWidth / (float)viewPortHeight, nearPlane,
                                      farPlane);
        return projection*view;

    }

    void setViewPortWidth(int viewPortWidth)
    {
        Camera::viewPortWidth = viewPortWidth;
    }

    void setViewPortHeight(int viewPortHeight)
    {
        Camera::viewPortHeight = viewPortHeight;
    }

    void moveCam(float forwardBackWardAmplifier, float leftRightAmplifier, float upDownAmplifier)
    {
        position += forwardBackWardAmplifier * orientation;
        position += -1 * leftRightAmplifier * glm::normalize(glm::cross(orientation, up));
        position += -1 * upDownAmplifier * up;
    }

    void rotateCam(float xSpeed, float ySpeed)
    {
        glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(ySpeed*-1),
                                               glm::normalize(glm::cross(orientation, up)));
        if (abs(glm::angle(newOrientation, up)) <= 90.0f)
        {
            orientation = newOrientation;
        }
        orientation = glm::rotate(orientation, glm::radians(xSpeed), up);
    }

    const glm::vec3 &getPosition() const
    {
        return position;
    }
};


