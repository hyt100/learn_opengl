#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Rotate.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.h"

static const char *vertStr = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 transform;

void main()
{
  gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
  TexCoord = aTexCoord;
}
)";

static const char *fragStr = R"(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord); 
}
)";

int Rotate::init()
{
    Image image("../res/lena_512x512.jpg");
    if (image.isError())
        return -1;
    prog_  = new ShaderUtil::Program(vertStr, fragStr);
    if (!prog_->isInitOk()) {
        std::cout << "init prog failed" << std::endl;
        return -1;
    }

    float vertices[] = {
    //    ---- 位置 ----           - 纹理坐标 -
        0.5f,  0.5f, -2.0f,     1.0f, 0.0f,   // 右上角
        0.5f, -0.5f, -2.0f,     1.0f, 1.0f,   // 右下角
        -0.5f, -0.5f, -2.0f,    0.0f, 1.0f,   // 左下角
        -0.5f,  0.5f, -2.0f,    0.0f, 0.0f    // 左上角
    };

    unsigned int indices[] = { // 注意要用逆时针方向，和renderer的球体保持一致，否则面剔除开启后会被剔除掉
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

int Rotate::setRotate(float pitch, float yaw)
{
    pitch_ = pitch;
    yaw_ = yaw;
    return 0;
}

int Rotate::draw()
{
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 100.0f);

    glm::mat4 mvp; //初始化为一个单位矩阵

    glm::mat4 view = glm::lookAt(glm::vec3(10.0f, 30.0f, 50.0f), 
           glm::vec3(0.0f + 10.0f, 0.0f + 30.0f, -1.0f + 50.0f),
           glm::vec3(0.0f, 1.0f, 0.0f));

    // 相机的位置是固定的，改变的是model矩阵，旋转的时候需要确保画面移动的时候是沿着球体竖直的经线方向，所以旋转矩阵必须这么写:
    glm::mat4 ry = glm::rotate(glm::mat4(), (float)glm::radians((float)yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rx = glm::rotate(glm::mat4(), (float)glm::radians((float)pitch_), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(10.0f, 30.0f, 50.0f)); //物体从原点开始平移一段距离
    glm::mat4 model = translate * rx * ry;  
    
    mvp = projection * view * model;

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    prog_->use();
    unsigned int transformLoc = glGetUniformLocation(prog_->getProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindVertexArray(VAO_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    prog_->unuse();
    return 0;
}