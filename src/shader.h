#pragma once
#include <string>

class Shader
{
public:
    Shader(const char *vertexShaderSource, const char *fragmentShaderSource);
    Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
    ~Shader();

    bool isInitOk() { return isInitOk_; }
    void use();

private:
    bool isInitOk_;
    unsigned int programObject_;  //handle to a progream object
};