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

    //using my modules to initialize the window and rendering
    if (WindowModule::init() == 0) {
        terminate();
        return -1;
    };
    RenderModule::init();

    // ----------------------------------------------

    while (!glfwWindowShouldClose(WindowModule::window)) { // main loop
        WindowModule::is_focused = glfwGetWindowAttrib(WindowModule::window, GLFW_FOCUSED);
        if (WindowModule::is_focused) { // only update inputs if window is focused
            InputModule::update(WindowModule::window);
            glfwGetWindowSize(WindowModule::window, &WindowModule::width, &WindowModule::height);
            glViewport(0, 0, WindowModule::width, WindowModule::height);
        } else Sleep(1); // if the window isn't focused fps is made to drop
        
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // don't need to call
        //glClear(GL_COLOR_BUFFER_BIT); // don't need to call

        //glUseProgram(shaderProgram); // don't need to call every frame

        RenderModule::render();
    }

    terminate();
    return 0;
}