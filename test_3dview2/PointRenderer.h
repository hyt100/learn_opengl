#pragma once
#include "ShaderUtil.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PointRenderer
{
public:
    PointRenderer()
        :prog_(nullptr)
    {}

    ~PointRenderer()
    {
        delete prog_;
    }

    int init();

    int draw(glm::mat4 &projection, glm::mat4 &view);

private:
    ShaderUtil::Program *prog_;
    unsigned int vao_;
    unsigned int vbo_;
    // unsigned int ebo_;
    int point_num_;
    GLint attrLocation_aPos_;
    GLint attrLocation_aColor_;
    GLint uniformLocation_mvp_;
};