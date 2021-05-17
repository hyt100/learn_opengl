#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <iostream>
#include <cmath>
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

            textureX = textureDeltaX * j;
            texturePos_.push_back(textureX);
            texturePos_.push_back(textureY);
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

            indices_.push_back(A);
            indices_.push_back(B);
            indices_.push_back(D);
            indices_.push_back(D);
            indices_.push_back(B);
            indices_.push_back(C);
        }
    }
}

int Renderer::init()
{
    prog_  = ShaderUtil::CreateProgramFromFile("../test_sphere/shader.vert", "../test_sphere/shader.frag");
    if (!prog_->isInitOk()) {
        std::cout << "init prog failed" << std::endl;
        return -1;
    }
    generateSphere();

    attrLocation_aPos_ = glGetAttribLocation(prog_->getProgram(), "aPos");
    uniformLocation_transform_ = glGetUniformLocation(prog_->getProgram(), "transform");

    vao_.addVertex3(attrLocation_aPos_, vertices_.data(), vertices_.size());
    vao_.setVertexIndex(indices_.data(), indices_.size());
 
    return 0;
}

int Renderer::draw()
{
    static int degree = 0;
    degree++;
    degree = degree > 360 ? 0 : degree;

    glm::mat4 trans; //初始化为一个零矩阵（所有元素均为0）
    // 绕Z轴旋转，使用GLFW的时间函数来获取不同时间的角度（GLM使用弧度制，可以使用glm::radians将角度转化为弧度）
    trans = glm::rotate(trans, (float)glm::radians((float)degree), glm::vec3(1.0f, 0.0f, 0.0f));

    prog_->use();
    vao_.bind();
    glUniformMatrix4fv(uniformLocation_transform_, 1, GL_FALSE, glm::value_ptr(trans));

    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

    vao_.unbind();
    prog_->unuse();
    return 0;
}