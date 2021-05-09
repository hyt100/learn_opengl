#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"

class Rectangle
{
public:
    Rectangle()
        :prog_(nullptr)
    {}

    ~Rectangle()
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
};