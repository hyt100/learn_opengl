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

    int draw();

private:
    Program *prog_;
    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;
};