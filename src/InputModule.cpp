#pragma once

#include "InputModule.h"
#include "RenderModule.h"

#include <iostream>
#include <filesystem>
#include <cmath>

#include "WindowModule.cpp"

double InputModule::x = 0;
double InputModule::y = 0;
double InputModule::dx = 0;
double InputModule::dy = 0;
int InputModule::mouse_left = 0;
double InputModule::scroll_total = 0;

double InputModule::x_drag = 0;
double InputModule::y_drag = 0;
double InputModule::zoom = 1;
int InputModule::fullscreen = 0;
int InputModule::paused = 0;

// processing mouse scroll inputs
void InputModule::scroll_callback(GLFWwindow* window, double x, double y) {
    scroll_total += y;
    double dzoom = pow(1.2, -y);
    zoom *= dzoom;
    x_drag /= dzoom;
    y_drag /= dzoom;
}

// processing keyboard inputs
void InputModule::key_callback(GLFWwindow* window, int key, int scancode, int actions, int mods) {
    if (key == GLFW_KEY_F11 && actions == GLFW_PRESS) {
        WindowModule::toggle_fullscreen();
    } else if (key == GLFW_KEY_F5 && actions == GLFW_PRESS) {
        scroll_total = 0;
        zoom = 1;
        x_drag = 0;
        y_drag = 0;
        RenderModule::refresh();
    } else if (key == GLFW_KEY_F4 && actions == GLFW_PRESS) {
        if (InputModule::paused == 0) InputModule::paused = 1;
        else InputModule::paused = 0;
    } else if (key == GLFW_KEY_LEFT && actions == GLFW_PRESS) {
        int count = std::distance(std::filesystem::directory_iterator("shader_fragment"), std::filesystem::directory_iterator());
        RenderModule::current_shader--;
        if (RenderModule::current_shader == -1) RenderModule::current_shader = count - 1;

        if (InputModule::paused == 1) InputModule::paused = 0;
        scroll_total = 0;
        zoom = 1;
        x_drag = 0;
        y_drag = 0;
        RenderModule::refresh();
    } else if (key == GLFW_KEY_RIGHT && actions == GLFW_PRESS) {
        int count = std::distance(std::filesystem::directory_iterator("shader_fragment"), std::filesystem::directory_iterator());
        RenderModule::current_shader = (RenderModule::current_shader + 1) % count;

        if (InputModule::paused == 1) InputModule::paused = 0;
        scroll_total = 0;
        zoom = 1;
        x_drag = 0;
        y_drag = 0;
        RenderModule::refresh();
    }
}

// getting inputs, processing mouse move and drag inputs, this is called every frame
void InputModule::update(GLFWwindow* window) {
    double cx;
    double cy;
    glfwGetCursorPos(window, &cx, &cy);
    dx = cx - x;
    dy = cy - y;
    x = cx;
    y = cy;

    glfwSetMouseButtonCallback(window, GLFW_MOUSE_BUTTON_LEFT);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwPollEvents();
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
        mouse_left = 1;
        x_drag += dx;
        y_drag += dy;
    } else {
        mouse_left = 0;
    }
}