#pragma once

#include <glad/glad.h>
#include <time.h>

class RenderModule {
    private:
        static clock_t frame_timer;
        static int frame_counter;
        static double time;
        static double last_time;
    public:
        static int current_shader;
        static unsigned int VAO, VBO, EBO, vertexShader, fragmentShader;
        static int ShaderProgram;
        static const char *vertexShaderSource, *fragmentShaderSource;
        static void init();
        static void terminate();
        static void refresh();
        static void render();
        static char* read_shader(char* f_name);
};