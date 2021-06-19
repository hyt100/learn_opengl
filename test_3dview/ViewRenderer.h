#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ViewRenderer
{
public:
    ViewRenderer()
        :mesh_prog_(nullptr), axis_prog_(nullptr)
    {}

    ~ViewRenderer()
    {
        delete mesh_prog_;
        delete axis_prog_;
    }

    int init();

    int draw(glm::mat4 &projection, glm::mat4 &view);

private:
    void mesh_generate();
    // void mesh_generate2();
    int mesh_init();
    int mesh_draw(glm::mat4 &mvp);

    int axis_init();
    int axis_draw(glm::mat4 &mvp);

    // mesh
    ShaderUtil::Program *mesh_prog_;
    std::vector<float> mesh_vertices_;
    std::vector<unsigned int> mesh_indices_;
    unsigned int mesh_vao_;
    unsigned int mesh_vbo_;
    unsigned int mesh_ebo_;
    GLint mesh_attrLocation_aPos_;
    GLint mesh_uniformLocation_mvp_;

    // axis
    ShaderUtil::Program *axis_prog_;
    unsigned int axis_vao_;
    unsigned int axis_vbo_;
    unsigned int axis_ebo_;
    GLint axis_attrLocation_aPos_;
    GLint axis_attrLocation_aColor_;
    GLint axis_uniformLocation_mvp_;
    
    glm::mat4 model = glm::mat4(1.0f);
};