#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdint.h>


class ViewMat
{
public:
    ViewMat(int viewWidth, int viewHeight);
    ~ViewMat() = default;

    glm::mat4 &getProjectionMat();
    glm::mat4 &getViewMat();

    void zoom_in();
    void zoom_out();

    void up();
    void down();
    void left();
    void right();

private:
    void compute_view();

    float fov_;
    float R_Min_;
    float R_Max_;
    float R_;     // 由上而下构建球体（右手坐标系，Y轴正方向为上，Y轴负方向为下）
    float theta_; // theta为球体上当前位置向量与+Y轴的夹角，取值0-180
    float gama_;  // gama为圆周上当前位置向量与+Z轴的逆时针方向夹角，取值0-360

    glm::mat4 projection_;
    glm::mat4 view_;
};