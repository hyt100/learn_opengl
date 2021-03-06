#include "PointRenderer.h"
#include <iostream>
#include <stdio.h>

static const char *vertStr = R"(
#version 330 core
in vec3 aPos;
in vec3 aColor;
uniform mat4 aMvp;

out vec3 Color;

void main()
{
  gl_Position = aMvp * vec4(aPos, 1.0);
  gl_PointSize = 8.0;
  Color = aColor;
}
)";

static const char *fragStr = R"(
#version 330 core
in vec3 Color;
out vec4 FragColor;

void main()
{
    FragColor = vec4(Color, 1.0); 
}
)";

int PointRenderer::init()
{
    prog_  = new ShaderUtil::Program(vertStr, fragStr);
    if (!prog_->isInitOk()) {
        std::cout << "init point-prog failed" << std::endl;
        return -1;
    }

    float vertices[] = {
    //    ---- 位置 ----       ----- 颜色 ----
        +3.0f, +0.0f, +0.0f,   1.0f, 0.0f, 0.0f,
        // +3.0f, +3.0f, -3.0f,   0.0f, 1.0f, 0.0f,
    };
    point_num_ = sizeof(vertices) / sizeof(vertices[0]) / 6;

    attrLocation_aPos_ = glGetAttribLocation(prog_->getProgram(), "aPos");
    attrLocation_aColor_ = glGetAttribLocation(prog_->getProgram(), "aColor");
    uniformLocation_mvp_ = glGetUniformLocation(prog_->getProgram(), "aMvp");

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    //   (注意：因为EBO绑定到同一个，所有这里的两个location使用同一个VBO上的数据，他们使用偏移量来区分)
    glVertexAttribPointer(attrLocation_aPos_, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attrLocation_aPos_);
    glVertexAttribPointer(attrLocation_aColor_, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(attrLocation_aColor_);

    glBindVertexArray(0); //解绑

    ShaderUtilCheckError();
    return 0;
}

int PointRenderer::draw(glm::mat4 &projection, glm::mat4 &view)
{
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 2.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // glm::mat4 r1 = glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // glm::mat4 r2 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    model = translate * rotate * model;
    
    glm::mat4 mvp = projection * view * model;

    prog_->use();
    glUniformMatrix4fv(uniformLocation_mvp_, 1, GL_FALSE, glm::value_ptr(mvp));
    glBindVertexArray(vao_);

    // 允许程序设置点的大小
    glEnable(GL_PROGRAM_POINT_SIZE);

    // 查询点大小的范围和步长
    float sizes[2] = {0.0f, 0.0f};
    float step = 1.0f;
    glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
    glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
    // printf("range: %f %f   step: %f \n", sizes[0], sizes[1], step);

    // 设置点的大小，也可以通过着色器内建变量gl_PointSize来设置（这里便是这样）
    // glPointSize(4.0f);

    //参数：图元类型, 顶点数组的起始索引, 绘制顶点的个数
    glDrawArrays(GL_POINTS, 0, point_num_);

    prog_->unuse();
    glBindVertexArray(0); //解绑

    ShaderUtilCheckError();
    return 0;
}