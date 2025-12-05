#pragma once
#include "../include/dft.h"
#include "../include/bmp.h"
#include "../include/dft.h"

typedef struct dgray_image_T{ // Double Gray Image
    size_t height;
    size_t width;
    double * data;
} dgray_image_T;

typedef struct cd_image_T{ // Complex Double Image
    size_t height;
    size_t width;
    cdouble_T * data;
} cd_image_T;


image_T convert_dgray_to_image(dgray_image_T dgray_image);
dgray_image_T convert_image_to_dgray(image_T image); // Double Gray -> dgray
cd_image_T calculate_2d_dft(dgray_image_T dgray_image);
image_T convert_cd_to_image(cd_image_T cd_image);