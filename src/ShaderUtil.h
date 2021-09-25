#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "FileReader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

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

    void setMat4(const std::string &name, const glm::mat4 &mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(programObject_, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }
    void setVec3(const std::string &name, const glm::vec3 &value)
    {
        glUniform3fv(glGetUniformLocation(programObject_, name.c_str()), 1, &value[0]); 
    }
    void setBool(const std::string &name, bool value)
    {         
        glUniform1i(glGetUniformLocation(programObject_, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value)
    { 
        glUniform1i(glGetUniformLocation(programObject_, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value)
    { 
        glUniform1f(glGetUniformLocation(programObject_, name.c_str()), value); 
    }

private:
    bool isInitOk_;
    unsigned int programObject_;  //handle to a progream object
};

GLenum CheckError(const char *file, int line);

typedef std::shared_ptr<Program> SharedProgram;

inline Program* CreateProgramFromFile(const char *vertexShaderFile, const char *fragmentShaderFile)
{
    FileReader fileVert(vertexShaderFile);
    FileReader fileFrag(fragmentShaderFile);
    return new ShaderUtil::Program(fileVert, fileFrag);
}

inline SharedProgram CreateSharedProgramFromFile(const char *vertexShaderFile, const char *fragmentShaderFile)
{
    return SharedProgram(CreateProgramFromFile(vertexShaderFile, fragmentShaderFile));
}

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

#define ShaderUtilCheckError() ShaderUtil::CheckError(__FILE__, __LINE__) 