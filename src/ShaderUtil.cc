#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>
#include "ShaderUtil.h"

using namespace ShaderUtil;

namespace {
unsigned int LoadShader(GLenum type, const char *shaderSource)
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
}

Program::Program(const char *vertexShaderSource, const char *fragmentShaderSource)
    : isInitOk_(false), programObject_(0)
{
    if (!vertexShaderSource || !fragmentShaderSource) {
        std::cout << "null string pointer" << std::endl;
        return;
    }
    if (strlen(vertexShaderSource) == 0 || strlen(fragmentShaderSource) == 0) {
        std::cout << "string length is 0" << std::endl;
        return;
    }

    // std::cout << "vertex shader: " << vertexShaderSource << std::endl;
    // std::cout << "frag shader: " << fragmentShaderSource << std::endl;

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
    // std::cout << "init prog ok" << std::endl;
}

Program::~Program()
{
    if (programObject_) {
        glDeleteProgram(programObject_);
    }
}

void Program::use()
{
    glUseProgram(programObject_);
}

void Program::unuse()
{
    glUseProgram(0);
}

VAO::VAO()
{
    glGenVertexArrays(1, &vao_);
}

VAO::~VAO()
{
    for (auto x : vboList_) {
        glDeleteBuffers(1, &x);
    }

    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
    }
}

void VAO::addVertex3(int location, float *data, int vertexCnt)
{
    this->bind();

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(float)*vertexCnt, data, GL_STATIC_DRAW);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    // (注意：因为这个vbo绑定后，只有一个location在使用，所以这里偏移量只需设为0)
    glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(location);

    vboList_.push_back(vbo);
}

void VAO::bind()
{
    glBindVertexArray(vao_);
}

void VAO::unbind()
{
    glBindVertexArray(0);
}