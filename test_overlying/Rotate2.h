#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"

class Rotate2
{
public:
    Rotate2()
        :prog_(nullptr)
    {}

    ~Rotate2()
    {
        delete prog_;
    }

    int init();

    int draw();

private:
    ShaderUtil::Program *prog_;
    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;
};