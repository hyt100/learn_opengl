#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Triangle.h"
#include <iostream>

int Triangle::init()
{
    FileReader fileVert("../test/triangle.vert");
    FileReader fileFrag("../test/triangle.frag");
    Shader *shader  = new Shader((char *)fileVert.data(), (char *)fileFrag.data());
    if (!shader->isInitOk()) {
        std::cout << "init failed" << std::endl;
        return -1;
    }
    shader_ = shader;

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); //解绑
    return 0;
}

int Triangle::draw()
{
    shader_->use();
    glBindVertexArray(VAO_);
    //参数：图元类型, 顶点数组的起始索引, 绘制顶点的个数
    glDrawArrays(GL_TRIANGLES, 0, 3);
    return 0;
}