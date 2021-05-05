#pragma once
#include "Shader.h"
#include "FileReader.h"

class Container
{
public:
    Container()
        :shader_(nullptr)
    {}

    ~Container()
    {
        delete shader_;
    }

    int init();

    int draw();

private:
    Shader *shader_;
    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;
};