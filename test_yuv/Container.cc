// ffmpeg -i lena_512x512.jpg -f rawvideo -pix_fmt yuv420p lena_512x512.yuv
// ffplay -f rawvideo -pixel_format yuv420p -video_size 512x512 lena_512x512.yuv

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Container.h"
#include <iostream>


int Container::init()
{   
    FileReader fileVert("../test_yuv/shader.vert");
    FileReader fileFrag("../test_yuv/shader.frag");
    prog_  = new ShaderUtil::Program(fileVert, fileFrag);
    if (!prog_->isInitOk()) {
        std::cout << "init failed" << std::endl;
        return -1;
    }

    float vertices[] = {
    //    ---- 位置 ----           - 纹理坐标 -
        0.5f,  0.5f, 0.0f,     1.0f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,     1.0f, 1.0f,   // 右下角
        -0.5f, -0.5f, 0.0f,    0.0f, 1.0f,   // 左下角
        -0.5f,  0.5f, 0.0f,    0.0f, 0.0f    // 左上角
    };

    unsigned int indices[] = { // 注意索引从0开始! 
        0, 3, 1, // 第一个三角形
        3, 2, 1  // 第二个三角形
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

    glGenTextures(3, texture_);
    for (int i = 0; i < 3; ++i) {
        glBindTexture(GL_TEXTURE_2D, texture_[i]);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    glBindTexture(GL_TEXTURE_2D, 0); //解绑
    
    uniformLocation_texture_[0] = glGetUniformLocation(prog_->getProgram(), "dataY");
    uniformLocation_texture_[1] = glGetUniformLocation(prog_->getProgram(), "dataU");
    uniformLocation_texture_[2] = glGetUniformLocation(prog_->getProgram(), "dataV");

    return 0;
}

int Container::draw()
{
    int width = 512, height = 512;
    FileReader fileYuv("../res/lena_512x512.yuv");
    if (fileYuv.is_error()) {
        std::cout << "load yuv failed" << std::endl;
        return -1;
    }
    uint8_t *y = fileYuv.data();
    uint8_t *u = y + width*height;
    uint8_t *v = y + width*height + width*height/4;

    prog_->use();

    // 加载纹理Y
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, y);
    glUniform1i(uniformLocation_texture_[0], 0);

    // 加载纹理U
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, u);
    glUniform1i(uniformLocation_texture_[1], 1);

    // 加载纹理V
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture_[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, v);
    glUniform1i(uniformLocation_texture_[2], 2);

    glBindVertexArray(VAO_);
    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    prog_->unuse();
    return 0;
}