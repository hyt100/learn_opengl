#pragma once
#include "Shader.h"
#include "FileReader.h"

class Triangle
{
public:
    Triangle()
        :shader_(nullptr)
    {}

    ~Triangle()
    {
        delete shader_;
    }

    int init();

    int draw();

private:
    Shader *shader_;
    unsigned int VAO_;
    unsigned int VBO_;
};