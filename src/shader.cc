#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>
#include "Shader.h"

static unsigned int LoadShader(GLenum type, const char *shaderSource)
{
    // create the shader object
    unsigned int shader = glCreateShader(type);
    if (0 == shader) {
        std::cout << "create shader failed" << std::endl;
        return 0;
    }
    // load the shader source
    glShaderSource(shader, 1, &shaderSource, NULL);
    // compile the shader
    glCompileShader(shader);

    // check the compile status
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = new char[infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cout << "Error compiling shader: \n" << infoLog << "\n" << std::endl;
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource)
    :isInitOk_(false), programObject_(0)
{
    if (!vertexShaderSource || !fragmentShaderSource) {
        return;
    }
    if (strlen(vertexShaderSource) == 0 || strlen(fragmentShaderSource) == 0) {
        return;
    }

    // load shaders
    unsigned int vertexShader = LoadShader(GL_VERTEX_SHADER, vertexShaderSource);
    if (0 == vertexShader) {
        return;
    }

    unsigned int fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (0 == fragmentShader) {
        glDeleteShader(vertexShader);
        return;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    if (0 == shaderProgram) {
        std::cout << "create program failed" << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
    }
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLint infoLen = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = new char[infoLen];
            glGetProgramInfoLog(shaderProgram, infoLen, NULL, infoLog);
            std::cout << "Error linking program: \n" << infoLog << "\n" << std::endl;
            delete[] infoLog;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        return;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    programObject_ = shaderProgram;
    isInitOk_ = true;
}

Shader::Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
    :isInitOk_(false), programObject_(0)
{
    Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
}

Shader::~Shader()
{
    if (programObject_) {
        glDeleteProgram(programObject_);
    }
}

void Shader::use()
{
    glUseProgram(programObject_);
}