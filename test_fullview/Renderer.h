#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"
#include <vector>

class Renderer
{
public:
    Renderer()
        :prog_(nullptr), pitch_(0), yaw_(0)
    {}

    ~Renderer()
    {
        delete prog_;
    }

    int init();

    int setRotate(float pitch, float yaw);

    int draw();

private:
    void generateSphere();
    void generateSphere2();

    ShaderUtil::Program *prog_;
    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;

    float pitch_;
    float yaw_;

    GLint attrLocation_aPos_;
    GLint attrLocation_aTexCoord_;
    GLint uniformLocation_mvp_;
    GLint uniformLocation_ourTexture_;

    std::vector<float> vertices_;
    std::vector<unsigned int> indices_;
};