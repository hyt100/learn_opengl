#pragma once
#include <string>
#include "FileReader.h"

class Program
{
public:
    Program(const char *vertexShaderSource, const char *fragmentShaderSource);
    Program(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
        : Program(vertexShaderSource.c_str(), fragmentShaderSource.c_str())
    {
        // std::cout << "init 1" << std::endl;
    }
    Program(const FileReader &vertexShaderSource, const FileReader &fragmentShaderSource)
       : Program((const char *)vertexShaderSource.data(), (const char *)fragmentShaderSource.data())
    {
        // std::cout << "init 2" << std::endl;
    }
    ~Program();

    bool isInitOk() { return isInitOk_; }
    void use();
    void unuse();
    unsigned int getProgram() { return programObject_; }

private:
    bool isInitOk_;
    unsigned int programObject_;  //handle to a progream object
};
