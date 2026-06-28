#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
//#include <windowsx.h>
//#include <winuser.h>

#include "WindowModule.cpp"
#include "RenderModule.cpp"
#include "InputModule.cpp"

void terminate() {
    RenderModule::terminate();
    glfwTerminate();
}

int main() {

    if (WindowModule::init() == 0) {
        terminate();
        return -1;
    };
    RenderModule::init();

    //glViewport(0, 0, 1000, 1000);

    // ----------------------------------------------

    while (!glfwWindowShouldClose(WindowModule::window)) {

        WindowModule::is_focused = glfwGetWindowAttrib(WindowModule::window, GLFW_FOCUSED);
        if (WindowModule::is_focused) {
            InputModule::update(WindowModule::window);
            glfwGetWindowSize(WindowModule::window, &WindowModule::width, &WindowModule::height);
            glViewport(0, 0, WindowModule::width, WindowModule::height);
        } else Sleep(1);
        
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        //glUseProgram(shaderProgram);

        RenderModule::render();
    }

    terminate();
    return 0;
}