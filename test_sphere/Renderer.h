#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"
#include <vector>

class Renderer
{
public:
    Renderer()
        :prog_(nullptr)
    {}

    ~Renderer()
    {
        delete prog_;
    }

    int init();

    int draw();

private:
    void generateSphere();
    void generateSphere2();

    ShaderUtil::Program *prog_;
    ShaderUtil::VAO vao_;

    GLint attrLocation_aPos_;
    GLint uniformLocation_transform_;

    std::vector<float> vertices_;
    std::vector<float> texturePos_;
    std::vector<unsigned int> indices_;

    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;
};