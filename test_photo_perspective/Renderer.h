#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"
#include "Image.h"
// #include "ViewMat.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

struct ModelPose
{
    float fov;
    float pitch;
    float yaw;
    float roll;

    ModelPose()
        :fov(90.0f), pitch(0.0f), yaw(0.0f), roll(0.0f)
    {}

    void left()  { if (yaw > -45) yaw -= 1; }
    void right() { if (yaw < 45) yaw += 1; }
    void up()    { if (pitch > -45) pitch -= 1; }
    void down()  { if (pitch < 45) pitch += 1; }
    void zoom_in()    { if (fov > 30) fov -= 0.5; }
    void zoom_out()   { if (fov <= 100) fov += 0.5; }
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

    int draw(ModelPose &pose);

private:
    ShaderUtil::Program *prog_;
    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int texture_;
    GLint uniformLocation_mvp_;
    Image *image_;
};