#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"

class Rotate
{
public:
    Rotate()
        :prog_(nullptr)
    {}

    ~Rotate()
    {
        delete prog_;
    }

    int init();

    int setRotate(float pitch, float yaw);

    int draw();

private:
    ShaderUtil::Program *prog_;
    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;

    float pitch_;
    float yaw_;
};