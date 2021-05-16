#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <iostream>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Renderer::generateSphere()
{
    float ballRadius = 0.5;    // 球体半径
    int layerNum = 80;       // 水平切割层数
    int perLayerNum = 80;    // 每层分割的份数
    float thetaNum  = M_PI / layerNum;
    float gamaNum = 2*M_PI / perLayerNum;
    float textureDeltaX = 1.0 / perLayerNum;
    float textureDeltaY = 1.0 / layerNum;

    float x, y, z;
    float textureX, textureY;

    // 生成顶点数据和纹理坐标
    for (int i = 0; i <= layerNum; ++i) {
        float theta =  thetaNum * i;
        float layerRaduis = ballRadius * sin(theta);
        y = ballRadius * cos(theta);
        textureY = textureDeltaY * i;

        for (int j = 0; j <= perLayerNum; ++j) {
            float gama = gamaNum * j;
            x = layerRaduis * sin(gama);
            z = layerRaduis * cos(gama);

            vertices_.push_back(x);
            vertices_.push_back(y);
            vertices_.push_back(z);

            textureX = 1 - textureDeltaX * j;
            vertices_.push_back(textureX);
            vertices_.push_back(textureY);
        }
    }

    //生成顶点索引
    //      D-------A
    //      |       |
    //      C-------B
    int pointPerLayerNum  = layerNum + 1;
    for (int i = 0; i < layerNum; ++i) {
        for (int j = 0; j < perLayerNum; ++j) {
            unsigned int A = pointPerLayerNum * i + j;
            unsigned int B = pointPerLayerNum * (i + 1) + j;
            unsigned int C = pointPerLayerNum * (i + 1) + 1 + j;
            unsigned int D = pointPerLayerNum * i + 1 + j;

            // indices_.push_back(A);
            // indices_.push_back(B);
            // indices_.push_back(D);
            // indices_.push_back(D);
            // indices_.push_back(B);
            // indices_.push_back(C);

            indices_.push_back(A);
            indices_.push_back(D);
            indices_.push_back(B);
            indices_.push_back(D);
            indices_.push_back(C);
            indices_.push_back(B);
        }
    }
}

int Renderer::init()
{
    int width, height, nrChannels;
    uint8_t *data = stbi_load("../res/fullview.jpg", &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    } else {
        std::cout << "load texture ok (" << width << "x" << height << "  " << nrChannels << ")" << std::endl;
    }
    prog_  = ShaderUtil::CreateProgramFromFile("../test_fullview/shader.vert", "../test_fullview/shader.frag");
    if (!prog_->isInitOk()) {
        std::cout << "init prog failed" << std::endl;
        stbi_image_free(data);
        return -1;
    }
    generateSphere();

    attrLocation_aPos_ = glGetAttribLocation(prog_->getProgram(), "aPos");
    attrLocation_aTexCoord_ = glGetAttribLocation(prog_->getProgram(), "aTexCoord");
    uniformLocation_mvp_ = glGetUniformLocation(prog_->getProgram(), "aMvp");
    uniformLocation_ourTexture_ = glGetUniformLocation(prog_->getProgram(), "ourTexture");

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &EBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices_.size(), indices_.data(), GL_STATIC_DRAW);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    //   (注意：因为EBO绑定到同一个，所有这里的两个location使用同一个VBO上的数据，他们使用偏移量来区分)
    glVertexAttribPointer(attrLocation_aPos_, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attrLocation_aPos_);
    glVertexAttribPointer(attrLocation_aTexCoord_, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(attrLocation_aTexCoord_);

    glBindVertexArray(0); //解绑

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0); //解绑

    stbi_image_free(data);
    return 0;
}

int Renderer::setRotate(float pitch, float yaw)
{
    pitch_ = pitch;
    yaw_ = yaw;
    return 0;
}

int Renderer::draw()
{
    glm::mat4 mvp; //初始化为一个单位矩阵

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(10.0f, 30.0f, 50.0f)); //物体从原点开始平移一段距离

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3(0.0f, 0.0f, -1.0f), 
           glm::vec3(0.0f, 1.0f, 0.0f));
    float rotateZ = sin(2.0 * M_PI * yaw_ / 360);
    float rotateX = cos(2.0 * M_PI * yaw_ / 360);
    view = glm::rotate(view, (float)glm::radians((float)yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, (float)glm::radians((float)pitch_), glm::vec3(rotateX,  0.0f, rotateZ));
    //摄像机也要平移相同距离，保持在球体中心 （注意：摄像机位移向量的x/y/z值必须添加一个负号，反方向移动）
    view = glm::translate(view, glm::vec3(-10.0f, -30.0f, -50.0f)); 

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 100.0f);

    mvp = projection * view * model;

    prog_->use();
    glBindVertexArray(VAO_);
    glBindTexture(GL_TEXTURE_2D, texture_);
    glUniformMatrix4fv(uniformLocation_mvp_, 1, GL_FALSE, glm::value_ptr(mvp));

    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

    prog_->unuse();
    return 0;
}