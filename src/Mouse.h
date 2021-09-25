#pragma once

class Mouse
{
 public:
    float xoffset;
    float yoffset;

    Mouse()
        :isFirst_(true), xoffset(0), yoffset(0)
    {}
    ~Mouse() = default;

    void UpdateMovePos(double xpos, double ypos)
    {
        if (isFirst_)
        {
            lastX_ = xpos;
            lastY_ = ypos;
            isFirst_ = false;
        }

        xoffset = xpos - lastX_;
        yoffset = lastY_ - ypos; // reversed since y-coordinates go from bottom to top

        lastX_ = xpos;
        lastY_ = ypos;
    }

 private:
    bool isFirst_;
    float lastX_;
    float lastY_;
};