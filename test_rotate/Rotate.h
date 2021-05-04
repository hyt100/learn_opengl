#pragma once
#include "Shader.h"
#include "FileReader.h"

class Rotate
{
public:
    Rotate()
        :shader_(nullptr)
    {}

    ~Rotate()
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