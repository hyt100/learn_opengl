#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"

class Triangle
{
public:
    Triangle()
        :prog_(nullptr)
    {}

    ~Triangle()
    {
        delete prog_;
    }

    int init();

    int draw();

private:
    ShaderUtil::Program *prog_;
    unsigned int VAO_;
    unsigned int VBO_;
};