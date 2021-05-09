#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"

class Container
{
public:
    Container()
        :prog_(nullptr)
    {}

    ~Container()
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