#include "ViewMat.h"

ViewMat::ViewMat(int viewWidth, int viewHeight)
    :fov_(90.0f), 
    R_Min_(1.0f), R_(1.0f), R_Max_(10.0f),
    theta_(90.0f), gama_(0.0f)
{
    projection_ = glm::perspective(glm::radians(fov_), 1.0f * viewWidth / viewHeight, 0.1f, 100.0f);
    compute_view();
}

void ViewMat::compute_view()
{
    //计算相机位置
    float theta = glm::radians(theta_);
    float gama = glm::radians(gama_);
    float layerRaduis = R_ * sin(theta);
    float pos_x = layerRaduis * sin(gama);
    float pos_y = R_ * cos(theta);
    float pos_z = layerRaduis * cos(gama);

    //计算up向量
    theta = glm::radians(90 - theta_);
    gama = glm::radians(gama_ + 180);
    float layerRaduis_up = R_ * sin(theta);
    float up_x = layerRaduis_up * sin(gama);
    float up_y = R_ * cos(theta);
    float up_z = layerRaduis_up * cos(gama);
    glm::vec3 up = glm::normalize(glm::vec3(up_x, up_y, up_z));

    view_ = glm::lookAt(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(0.0f, 0.0f, 0.0f), up);
}

glm::mat4 &ViewMat::getProjectionMat()
{
    return projection_;
}

glm::mat4 &ViewMat::getViewMat()
{
    return view_;
}

void ViewMat::zoom_in()
{
    R_ -= 0.5f;
    if (R_ < R_Min_)
        R_ = R_Min_;
    compute_view();
}

void ViewMat::zoom_out()
{
    R_ += 0.5f;
    if (R_ > R_Max_)
        R_ = R_Max_;
    compute_view();
}

void ViewMat::up()
{
    theta_ -= 1.0f;
    if (theta_ < 0.0f)
        theta_ = 0.0f;
    compute_view();
}

void ViewMat::down()
{
    theta_ += 1.0f;
    if (theta_ > 180.0f)
        theta_ = 180.0f;
    compute_view();
}

void ViewMat::left()
{
    gama_ -= 1.0f;
    if (gama_ < 0.0f)
        gama_ += 360.0f;
    compute_view();
}

void ViewMat::right()
{
    gama_ += 1.0f;
    if (gama_ > 360.0f)
        gama_ -= 360.0f;
    compute_view();
}
