#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"
#include "Image.h"
#include "ViewMat.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct CameraPose
{
    float trans;
    float pitch;
    float yaw;
    float roll;
};

class PhotoRenderer
{
public:
    PhotoRenderer()
        :prog_(nullptr), image_(nullptr)
    {}

    ~PhotoRenderer()
    {
        delete prog_;
    }

    int init(Image *image);

    int draw(ViewMat *viewMat);

private:
    ShaderUtil::Program *prog_;
    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;
    GLint uniformLocation_mvp_;
    Image *image_;
};