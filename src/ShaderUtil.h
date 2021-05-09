#pragma once
#include <string>
#include <vector>
#include "FileReader.h"

namespace ShaderUtil {

class Program
{
public:
    Program(const char *vertexShaderSource, const char *fragmentShaderSource);
    Program(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
        : Program(vertexShaderSource.c_str(), fragmentShaderSource.c_str())
    {
        // std::cout << "init 1" << std::endl;
    }
    Program(const FileReader &vertexShaderSource, const FileReader &fragmentShaderSource)
       : Program((const char *)vertexShaderSource.data(), (const char *)fragmentShaderSource.data())
    {
        // std::cout << "init 2" << std::endl;
    }
    ~Program();

    bool isInitOk() { return isInitOk_; }
    void use();
    void unuse();
    unsigned int getProgram() { return programObject_; }

private:
    bool isInitOk_;
    unsigned int programObject_;  //handle to a progream object
};

class VAO
{
public:
    VAO();
    ~VAO();

    // 设置EBO顶点索引（如果设置了的话，渲染用glDrawElements；未设置的话，渲染用glDrawArrays）
    void setVertexIndex(unsigned int *index, int indexSize);

    // 属性有3个数据（例如，x/y/z）
    void addVertex3(int location, float *data, int vertexCnt);

    void bind();
    void unbind();

    void draw(int vertexNum); // vertexNum： 绘制顶点个数

private:
    unsigned int vao_;
    unsigned int ebo_;
    std::vector<unsigned int> vboList_;
};

class Texture
{
public:
    Texture();
    ~Texture();

    void bind();
    void unbind();

    void addRGB(uint8_t *data, int width, int height);

private:
    unsigned int texture_;
};

}