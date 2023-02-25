//
// Created by Daniil on 09.12.2022.
//


#define GLFW_INCLUDE_VULKAN

#include "GLFW/glfw3.h"

#include "GLFW/glfw3native.h"
#include <vector>
#include <cstdlib>
#include <vulkan/vulkan_core.h>
#include <stdexcept>

#include "KeyBoardCallBack.h"
#include "MouseMovedCallBack.h"
#include "WindowResizeCallback.h"
#include "GLFW/glfw3.h"

#pragma once


class Window {
private:
    static inline Window *windowInstance = nullptr;

    static void resized(GLFWwindow *window, int width, int height) {
        windowInstance->width = width;
        windowInstance->height = height;
        for (const auto &item: windowInstance->resizeCallBacks) {
            if (item != nullptr) {
                item->resized(width, height);
            }

        }
    }

public:
    friend class LogicalDevice;

    static bool initWindow(const char *title, int width, int height) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        GLFWwindow *windowHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (windowHandle != nullptr) {
            windowInstance = new Window(width, height, title, windowHandle);
            glfwSetWindowSizeCallback(windowHandle, resized);
            glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


            return true;
        }
        return false;
    }

    static Window *getWindowInstance() {
        return windowInstance;
    }

private:
    int width;
    int height;
    const char *title;
    long long counter = 0;
    double prevTime = 0;
    bool fpsCounter = true;
    GLFWwindow *handle;
    std::vector<WindowResizeCallback *> resizeCallBacks;
    std::vector<MouseMovedCallBack *> mouseMoveCallBacks;
    std::vector<KeyBoardCallBack *> keyBoardCallBacks;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    Window() {

    }

    Window(int width, int height, const char *title, GLFWwindow *handle) : width(width), height(height), title(title),
                                                                           handle(handle) {
        keyBoardCallBacks.clear();
        resizeCallBacks.clear();
        mouseMoveCallBacks.clear();
    }

    ~Window() {
        glfwDestroyWindow(handle);
        glfwTerminate();
        delete this;
    }

    void checkMouseCallBacks() {
        int width = 0;
        int height = 0;
        glfwGetWindowSize(handle, &width, &height);
        double cursorX = 0;
        double cursorY = 0;
        glfwGetCursorPos(handle, &cursorX, &cursorY);
        int xChange = 0;
        int yChange = 0;
        if (abs(cursorX - width / 2) > 0) {
            xChange = (cursorX - width / 2) / abs(cursorX - width / 2);
        }
        if (abs(cursorY - height / 2) > 0) {
            yChange = (cursorY - height / 2) / abs(cursorY - height / 2);
        }
        if (xChange != 0 || yChange != 0) {
            for (const auto &item: mouseMoveCallBacks) {
                if (item != nullptr) {
                    item->mouseMoved(-1 * xChange, -1 * yChange);
                }
            }
        }
        glfwSetCursorPos(handle, width / 2, height / 2);
    }

    void checkKeyBoardCallBacks() {
        for (const auto &item: keyBoardCallBacks) {
            if (item != nullptr) {
                for (unsigned int counter = 0; counter < item->getKeyCodeAmount(); counter++) {
                    if (glfwGetKey(handle, item->getKeyCodes()[counter])) {
                        item->action(item->getKeyCodes()[counter]);
                    }
                }
            }

        }
    }

public:
    void registerResizeCallback(WindowResizeCallback *callBack) {
        this->resizeCallBacks.push_back(callBack);
    }

    VkSurfaceKHR getWindowSurface(VkInstance instance) {
        if (surface == VK_NULL_HANDLE) {
            glfwCreateWindowSurface(instance, handle, nullptr, &surface);
        }
        return surface;
    }

    void preRenderEvents() {
        checkKeyBoardCallBacks();
        checkMouseCallBacks();
    }

    void registerMouseCallBack(MouseMovedCallBack *mouseCallback) {
        mouseMoveCallBacks.push_back(mouseCallback);
    }

    void registerKeyboardCallBack(KeyBoardCallBack *keyBoardCallBack) {
        keyBoardCallBacks.push_back(keyBoardCallBack);
    }

    void removeResizeCallBack(WindowResizeCallback *callBack) {
        std::vector<WindowResizeCallback *> newCallBacks;
        for (int i = 0; i < (unsigned int) resizeCallBacks.size(); i++) {
            if (resizeCallBacks[i] != callBack) {
                newCallBacks.push_back(resizeCallBacks[i]);
            }
        }
        resizeCallBacks.clear();
        this->resizeCallBacks = newCallBacks;
    }

    void setSize(int width, int height) {
        this->width = width;
        this->height = height;
        glfwSetWindowSize(handle, width, height);
    }

    void setTitle(const char *title) {
        this->title = title;
        if (!fpsCounter) {
            glfwSetWindowTitle(handle, title);
        }

    }

    bool isWindowNeedToClose() {
        return glfwWindowShouldClose(handle);
    }

    void postRenderEvents() {
        glfwPollEvents();
        if (fpsCounter) {
            double crntTime = glfwGetTime();
            double timeDiff = crntTime - prevTime;
            counter++;

            if (timeDiff >= 1.0 / 30.0) {
                std::string FPS = std::to_string((int) ((1.0 / timeDiff) * counter));
                std::string ms = std::to_string((timeDiff / counter) * 1000);
                glfwSetWindowTitle(handle, (std::string(title) + " FPS: " + FPS + " ms: " + ms).c_str());

                prevTime = crntTime;
                counter = 0;
            }
        }

    }

    bool isFpsCounter() const {
        return fpsCounter;
    }

    void setFpsCounter(bool fpsCounter) {
        Window::fpsCounter = fpsCounter;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }
};


