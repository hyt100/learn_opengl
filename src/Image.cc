#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Image.h"
#include <iostream>

Image::Image(const char *filepath)
{
    data_ = stbi_load(filepath, &width_, &height_, &channels_, 0);
    if (!data_) {
        std::cout << "Failed to load texture" << std::endl;
        isError_ = true;
    } else {
        std::cout << "load texture ok (" << width_ << "x" << height_ << "  " << channels_ << ")" << std::endl;
        isError_ = false;
    }
}

Image::~Image()
{
    if (data_) {
        stbi_image_free(data_);
    }
}