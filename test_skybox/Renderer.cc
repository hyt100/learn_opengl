#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <iostream>
#include <cmath>
#include "Image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int Renderer::init()
{
    Image top("../res/skybox/top.jpg");
    Image bottom("../res/skybox/bottom.jpg");
    Image left("../res/skybox/left.jpg");
    Image right("../res/skybox/right.jpg");
    Image front("../res/skybox/front.jpg");
    Image back("../res/skybox/back.jpg");
    
    if (top.isError() || bottom.isError() || left.isError() || right.isError() || front.isError() || back.isError())
        return -1;
    prog_  = ShaderUtil::CreateProgramFromFile("../test_skybox/shader.vert", "../test_skybox/shader.frag");
    if (!prog_->isInitOk()) {
        std::cout << "init prog failed" << std::endl;
        return -1;
    }

    attrLocation_aPos_ = glGetAttribLocation(prog_->getProgram(), "aPos");
    uniformLocation_mvp_ = glGetUniformLocation(prog_->getProgram(), "aMvp");
    uniformLocation_ourTexture_ = glGetUniformLocation(prog_->getProgram(), "cubeMap");

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    verNUm_ = sizeof(skyboxVertices) / sizeof(float);

    //参数：属性编号, 属性的数据个数, 数据的类型, 步长, 起始位置的偏移量
    //   (注意：因为EBO绑定到同一个，所有这里的两个location使用同一个VBO上的数据，他们使用偏移量来区分)
    glVertexAttribPointer(attrLocation_aPos_, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(attrLocation_aPos_);

    glBindVertexArray(0); //解绑

    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, top.getWidth(), top.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, top.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, bottom.getWidth(), bottom.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, bottom.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, left.getWidth(), left.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, left.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, right.getWidth(), right.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, right.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, front.getWidth(), front.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, front.getData());
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, back.getWidth(), back.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, back.getData());
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0); //解绑

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

    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3(0.0f, 0.0f, -1.0f), 
           glm::vec3(0.0f, 1.0f, 0.0f));
    float rotateZ = sin(2.0 * M_PI * yaw_ / 360);
    float rotateX = cos(2.0 * M_PI * yaw_ / 360);
    view = glm::rotate(view, (float)glm::radians((float)yaw_), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, (float)glm::radians((float)pitch_), glm::vec3(rotateX,  0.0f, rotateZ));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 100.0f);

    mvp = projection * view * model;

    glDepthMask(GL_FALSE); //天空盒作为背景，永远绘制在其他物体后面，因此需要禁用深度写入
    prog_->use();
    glBindVertexArray(VAO_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    glUniformMatrix4fv(uniformLocation_mvp_, 1, GL_FALSE, glm::value_ptr(mvp));

    //参数：图元类型, 绘制顶点的个数, 索引的类型, 指定EBO中的偏移量
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDrawArrays(GL_TRIANGLES, 0, verNUm_);

    prog_->unuse();
    glDepthMask(GL_TRUE);
    return 0;
}