#pragma once

#include "RenderModule.h"
#include "InputModule.h"
#include "WindowModule.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string.h>
#include <iostream>
#include <filesystem>

clock_t RenderModule::frame_timer = clock();
int RenderModule::frame_counter = 0;
double RenderModule::time = 0;
double RenderModule::last_time = 0;



int RenderModule::current_shader = 0;
unsigned int RenderModule::VAO, RenderModule::VBO, RenderModule::EBO, RenderModule::vertexShader, RenderModule::fragmentShader;
int RenderModule::ShaderProgram;
const char *RenderModule::vertexShaderSource, *RenderModule::fragmentShaderSource;

// initialize the rendering; vertecies (2 triangles), and shaders
void RenderModule::init() {
    int success;
    char infoLog[1024];

    vertexShaderSource = read_shader((char*)"shader_vertex/base.glsl");
    
    // read first fragment shader from the directory
    for (const auto & entry : std::filesystem::directory_iterator("shader_fragment")) {
        fragmentShaderSource = read_shader((char*)entry.path().string().c_str());
        break;
    }

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, vertexShader);
    glAttachShader(ShaderProgram, fragmentShader);
    glLinkProgram(ShaderProgram);

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(ShaderProgram);
}

// duh
void RenderModule::terminate() {
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// reload the fragment shader, and reset time_u to 0
void RenderModule::refresh() {
    int success;
    char infoLog[1024];

    int i = 0;
    for (const auto & entry : std::filesystem::directory_iterator("shader_fragment")) {
        if (i < current_shader) i++;
        else {
            free((void*)fragmentShaderSource); // so no memory leak happens, read_shader uses malloc()
            fragmentShaderSource = read_shader((char*)entry.path().string().c_str());
            break;
        }
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    glAttachShader(ShaderProgram, fragmentShader);
    glLinkProgram(ShaderProgram);
    glUseProgram(ShaderProgram);

    time = 0;
    last_time = 0;
    glfwSetTime(0);
}

// called every frame, counts fps and uses render calls
void RenderModule::render() {
    double curr_time = glfwGetTime();
    double dt = curr_time - last_time;
    if (InputModule::paused == 0) time += dt;
    last_time = curr_time;

    glUniform1f(glGetUniformLocation(ShaderProgram, "u_time"), RenderModule::time);
    glUniform2f(glGetUniformLocation(ShaderProgram, "u_resolution"), WindowModule::width, WindowModule::height);
    glUniform1f(glGetUniformLocation(ShaderProgram, "u_x"), InputModule::x_drag);
    glUniform1f(glGetUniformLocation(ShaderProgram, "u_y"), InputModule::y_drag);
    glUniform1f(glGetUniformLocation(ShaderProgram, "u_mouse"), InputModule::mouse_left);
    glUniform1f(glGetUniformLocation(ShaderProgram, "u_zoom"), InputModule::zoom);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(WindowModule::window);
    glfwPollEvents();

    clock_t time_curr = clock();
    frame_counter++;
    if (time_curr - frame_timer >= 1000)
    {
        frame_timer = time_curr;
        printf("FPS: %d\n", frame_counter);
        frame_counter = 0;
    }
}

// reads file into char*
char* RenderModule::read_shader(char* f_name) {
    FILE* f;
    f = fopen(f_name, "rb");
    fseek(f, 0, SEEK_END);
    int f_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* out = (char*)malloc(f_size + 1);
    fread(out, f_size, 1, f);
    out[f_size] = 0;
    return out;
}