#pragma once
#include "ShaderUtil.h"
#include "FileReader.h"
#include <vector>
#include <memory>

class ColorRenderer
{
 public:
    ColorRenderer()
    {
        init_ok_ = false;
    }
    ~ColorRenderer()
    {
        if (init_ok_) {
            glDeleteVertexArrays(1, &lightVAO_);
            glDeleteVertexArrays(1, &objectVAO_);
            glDeleteBuffers(1, &VBO_);
        }
    }

    int Init();
    int Draw(const glm::mat4 &projection, const glm::mat4 &view);

 private:
    ShaderUtil::SharedProgram prog_light_;
    ShaderUtil::SharedProgram prog_object_;
    bool init_ok_;
    unsigned int lightVAO_;
    unsigned int objectVAO_;
    unsigned int VBO_;
};