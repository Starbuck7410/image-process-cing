#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#define BMP_HEADER_SIZE 14
#define BMP_INFOHEADER_SIZE 40
#define HEADER_FIELD_COUNT 15
#define HEADER_FIELD_SIZES {/* H */2, 4, 4, 4, /* IH */ 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4}


// h for header, ih for infoheader. More info in here:
// https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

typedef struct header_T{ 
    uint32_t h_signature;
    uint32_t h_file_size;
    uint32_t h_unused_0;
    uint32_t h_data_offset;
    uint32_t ih_size;
    uint32_t ih_width;
    uint32_t ih_height;
    uint32_t ih_planes;
    uint32_t ih_bits_per_pixel;
    uint32_t ih_compression;
    uint32_t ih_image_size;
    uint32_t ih_x_pixels_per_m;
    uint32_t ih_y_pixels_per_m;
    uint32_t ih_colors_used;
    uint32_t ih_important_colors;
} header_T;

typedef struct image_T {
    header_T header;
    uint8_t * data;
} image_T;



image_T load_bmp_from_file(FILE * bitmap_file);
void write_bmp(image_T image, FILE * bitmap_file);