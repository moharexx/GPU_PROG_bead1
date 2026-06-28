#pragma once

#include <GLFW/glfw3.h>

class RenderModule;
class InputModule {
    private:
        static void scroll_callback(GLFWwindow* window, double x, double y);
        static void key_callback(GLFWwindow* window, int key, int scancode, int actions, int mods);
    public:
        static double x;
        static double y;
        static double dx;
        static double dy;
        static int mouse_left;
        static double scroll_total;

        static double x_drag;
        static double y_drag;
        static double zoom;
        static int fullscreen;
        static int paused;

        static void update(GLFWwindow* window);
};