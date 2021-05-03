#pragma once
#include "Shader.h"
#include "FileReader.h"

class Rectangle
{
public:
    Rectangle()
        :shader_(nullptr)
    {}

    ~Rectangle()
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
};