#pragma once
#include <stdint.h>

class Image
{
public:
    Image(const char *filepath);
    ~Image();
    bool isError()  { return isError_; }
    int getWidth()  { return width_; }
    int getHeight() { return height_; }
    uint8_t* getData()   { return data_; }

private:
    bool isError_;
    uint8_t *data_;
    int width_;
    int height_;
    int channels_;
};