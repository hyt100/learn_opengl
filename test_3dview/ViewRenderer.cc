#include <iostream>
#include <cmath>
#include "ViewRenderer.h"

static const char *mesh_vertStr = R"(
#version 330 core
in vec3 aPos;
uniform mat4 aMvp;

void main()
{
  gl_Position = aMvp * vec4(aPos, 1.0);
}
)";

static const char *mesh_fragStr = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.44, 0.5, 0.56, 1.0); 
}
)";

static const char *axis_vertStr = R"(
#version 330 core
in vec3 aPos;
in vec3 aColor;
uniform mat4 aMvp;

out vec3 Color;

void main()
{
  gl_Position = aMvp * vec4(aPos, 1.0);
  Color = aColor;
}
)";

static const char *axis_fragStr = R"(
#version 330 core
in vec3 Color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(Color, 1.0); 
}
)";

int ViewRenderer::axis_init()
{
    const float axis_max = 15.0f;

    float axis_vertices[] = {
        //---------坐标-----------        ------颜色-------
        0.0f,      0.0f,      0.0f,      1.0f, 0.0f, 0.0f,
        axis_max,  0.0f,      0.0f,      1.0f, 0.0f, 0.0f,

        0.0f,      0.0f,      0.0f,      0.0f, 1.0f, 0.0f,
        0.0f,      axis_max,  0.0f,      0.0f, 1.0f, 0.0f,

        0.0f,      0.0f,      0.0f,      0.0f, 0.0f, 1.0f,
        0.0f,      0.0f,      axis_max,  0.0f, 0.0f, 1.0f,
    };
    unsigned int axis_indices[] = {
        0, 1,
        2, 3, 
        4, 5
    };

    axis_prog_  = new ShaderUtil::Program(axis_vertStr, axis_fragStr);
    if (!axis_prog_->isInitOk()) {
        std::cout << "init axis-prog failed" << std::endl;
        return -1;
    }

    axis_attrLocation_aPos_ = glGetAttribLocation(axis_prog_->getProgram(), "aPos");
    axis_attrLocation_aColor_ = glGetAttribLocation(axis_prog_->getProgram(), "aColor");
    axis_uniformLocation_mvp_ = glGetUniformLocation(axis_prog_->getProgram(), "aMvp");

    glGenVertexArrays(1, &axis_vao_);
    glBindVertexArray(axis_vao_);

    glGenBuffers(1, &axis_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, axis_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &axis_ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axis_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(axis_indices), axis_indices, GL_STATIC_DRAW);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    //   (注意：因为EBO绑定到同一个，所有这里的两个location使用同一个VBO上的数据，他们使用偏移量来区分)
    glVertexAttribPointer(axis_attrLocation_aPos_, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(axis_attrLocation_aPos_);
    glVertexAttribPointer(axis_attrLocation_aColor_, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(axis_attrLocation_aColor_);

    glBindVertexArray(0); //解绑
    return 0;
}

int ViewRenderer::axis_draw(glm::mat4 &mvp)
{
    axis_prog_->use();
    glBindVertexArray(axis_vao_);
    glUniformMatrix4fv(axis_uniformLocation_mvp_, 1, GL_FALSE, glm::value_ptr(mvp));

    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    glDrawElements(GL_LINE, 6, GL_UNSIGNED_INT, 0);

    axis_prog_->unuse();
    glBindVertexArray(0); //解绑
    return 0;
}

void ViewRenderer::mesh_generate()
{
    float max = 10.0f;
    float step = 1.0f;

    // vertices
    for (float v = 0.0f; v <= max; v += step) {
        mesh_vertices_.push_back(v);
        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(0.0f);

        mesh_vertices_.push_back(v);
        mesh_vertices_.push_back(max);
        mesh_vertices_.push_back(0.0f);

        mesh_vertices_.push_back(v);
        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(max);
    }
    for (float v = 0.0f; v <= max; v += step) {
        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(v);
        mesh_vertices_.push_back(0.0f);

        mesh_vertices_.push_back(max);
        mesh_vertices_.push_back(v);
        mesh_vertices_.push_back(0.0f);

        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(v);
        mesh_vertices_.push_back(max);
    }
    for (float v = 0.0f; v <= max; v += step) {
        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(v);

        mesh_vertices_.push_back(max);
        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(v);

        mesh_vertices_.push_back(0.0f);
        mesh_vertices_.push_back(max);
        mesh_vertices_.push_back(v);
    }

    // indices
    for (int i = 0; i < mesh_vertices_.size(); i += 3) {
        mesh_indices_.push_back(i);
        mesh_indices_.push_back(i+1);
        mesh_indices_.push_back(i);
        mesh_indices_.push_back(i+2);
    }
}

int ViewRenderer::mesh_init()
{
    mesh_prog_  = new ShaderUtil::Program(mesh_vertStr, mesh_fragStr);
    if (!mesh_prog_->isInitOk()) {
        std::cout << "init mesh-prog failed" << std::endl;
        return -1;
    }

    mesh_attrLocation_aPos_ = glGetAttribLocation(mesh_prog_->getProgram(), "aPos");
    mesh_uniformLocation_mvp_ = glGetUniformLocation(mesh_prog_->getProgram(), "aMvp");

    glGenVertexArrays(1, &mesh_vao_);
    glBindVertexArray(mesh_vao_);

    glGenBuffers(1, &mesh_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, mesh_vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh_vertices_.size(), mesh_vertices_.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mesh_ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh_vertices_.size(), mesh_indices_.data(), GL_STATIC_DRAW);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    //   (注意：因为EBO绑定到同一个，所有这里的两个location使用同一个VBO上的数据，他们使用偏移量来区分)
    glVertexAttribPointer(mesh_attrLocation_aPos_, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(mesh_attrLocation_aPos_);

    glBindVertexArray(0); //解绑
    return 0;
}

int ViewRenderer::mesh_draw(glm::mat4 &mvp)
{
    mesh_prog_->use();
    glBindVertexArray(mesh_vao_);
    glUniformMatrix4fv(mesh_uniformLocation_mvp_, 1, GL_FALSE, glm::value_ptr(mvp));

    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    glDrawElements(GL_LINE, mesh_indices_.size(), GL_UNSIGNED_INT, 0);

    mesh_prog_->unuse();
    glBindVertexArray(0); //解绑
    return 0;
}

int ViewRenderer::init()
{
    if (mesh_init() != 0)
        return -1;
    if (axis_init() != 0)
        return -1;
    return 0;
}

int ViewRenderer::draw(glm::mat4 &projection, glm::mat4 &view)
{
    glm::mat4 mvp = projection * view * model;

    mesh_draw(mvp);
    axis_draw(mvp);
    return 0;
}