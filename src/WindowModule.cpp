#pragma once

#include "WindowModule.h"

#include <glad/glad.h>

#include <stdio.h>

GLFWwindow* WindowModule::window;
GLFWmonitor* WindowModule::monitor;
const GLFWvidmode* WindowModule::vidmode;
int WindowModule::is_focused;
int WindowModule::is_fullscreen;
int WindowModule::width;
int WindowModule::height;

int WindowModule::saved_width;
int WindowModule::saved_height;
int WindowModule::saved_x;
int WindowModule::saved_y;

int WindowModule::init() {
    is_focused = 1;
    is_fullscreen = 0;
    width = 1000;
    height = 1000;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    monitor = glfwGetPrimaryMonitor();
    vidmode = glfwGetVideoMode(monitor);

    if (!window) {
        printf("ERROR: Failed window creation\n");
        return 0;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("ERROR: Failed to initialize GLAD\n");
        return 0;
    }
    return 1;
}

void WindowModule::toggle_fullscreen() {
    if (is_fullscreen) {
        is_fullscreen = 0;
        glfwSetWindowMonitor(window, NULL, saved_x, saved_y, saved_width, saved_height, 0);
    } else {
        is_fullscreen = 1;
        saved_width = width;
        saved_height = height;
        glfwGetWindowPos(window, &saved_x, &saved_y);
        glfwSetWindowMonitor(window, monitor, 0, 0, vidmode->width, vidmode->height, vidmode->refreshRate);
    }
}