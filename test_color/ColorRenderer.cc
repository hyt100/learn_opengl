#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ColorRenderer.h"
#include "config.h"

int ColorRenderer::Init()
{
    prog_light_  = ShaderUtil::CreateSharedProgramFromFile(TEST_PATH "light.vs", TEST_PATH "light.fs");
    prog_object_ = ShaderUtil::CreateSharedProgramFromFile(TEST_PATH "object.vs", TEST_PATH "object.fs");
    if (!prog_light_->isInitOk() || !prog_object_->isInitOk()) {
        std::cout << "init prog failed" << std::endl;
        return -1;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
    // first, configure the cube's VAO (and VBO)
    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &objectVAO_);
    glBindVertexArray(objectVAO_);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &lightVAO_);
    glBindVertexArray(lightVAO_);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    init_ok_ = true;
    return 0;
}

int ColorRenderer::Draw(const glm::mat4 &projection, const glm::mat4 &view)
{
    // be sure to activate shader when setting uniforms/drawing objects
    prog_object_->use();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp = projection * view * model;
    prog_object_->setMat4("mvp", mvp);
    prog_object_->setVec3("objectColor", glm::vec3(0.5f, 0.0f, 0.0f));
    prog_object_->setVec3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));

    // render the object
    glBindVertexArray(objectVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // also draw the lamp object
    prog_light_->use();
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    model = glm::translate(glm::mat4(1.0f), lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    mvp = projection * view * model;
    prog_light_->setMat4("mvp", mvp);

    glBindVertexArray(lightVAO_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    prog_light_->unuse();

    ShaderUtilCheckError();
    return 0;
}
