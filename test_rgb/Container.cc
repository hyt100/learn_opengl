#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Container.h"
#include <iostream>
#include "Image.h"

int Container::init()
{
    Image image("../res/container.jpg");
    if (image.isError())
        return -1;

    prog_ = ShaderUtil::CreateProgramFromFile("../test_rgb/shader.vert", "../test_rgb/shader.frag");
    if (!prog_->isInitOk()) {
        std::cout << "init failed" << std::endl;
        return -1;
    }

    float vertices[] = {
    //    ---- 位置 ----           - 纹理坐标 -
        0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // 右上角
        0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // 右下角
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // 左下角
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    //   (注意：因为EBO绑定到同一个，所有这里的两个location使用同一个VBO上的数据，他们使用偏移量来区分)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); //解绑

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());

    return 0;
}

int Container::draw()
{
    prog_->use();
    glBindVertexArray(VAO_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    prog_->unuse();
    return 0;
}