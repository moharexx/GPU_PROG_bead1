#pragma once

#include <GLFW/glfw3.h>

class WindowModule {
    private:
        static int saved_width;
        static int saved_height;
        static int saved_x;
        static int saved_y;
    public:
        static int width;
        static int height;
        static GLFWwindow* window;
        static GLFWmonitor* monitor;
        static const GLFWvidmode* vidmode;
        static int is_focused;
        static int is_fullscreen;
        static int init();
        static void toggle_fullscreen();
};