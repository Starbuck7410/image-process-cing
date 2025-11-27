#include "../include/loadbmp.h"

// everything will be stored as 4 byte thingies. uint32_t
uint32_t load_n_bytes(FILE * bitmap_file, size_t n){
    uint8_t buffer[n];  
    uint32_t result = 0;
    fread(buffer, sizeof(uint8_t), n, bitmap_file);
    for (size_t i = 0; i < n; i++){
        result |= buffer[i] << i*8;
    }
    return result;
}

header_T load_bmp_header(FILE * bitmap_file){
    header_T header = {
        .field_sizes = HEADER_FIELD_SIZES
    };

    uint32_t * header_pointer = &header.h_signature;
    for(size_t i = 0; i < HEADER_FIELD_COUNT; i++){
        header_pointer[i] = load_n_bytes(bitmap_file, header.field_sizes[i]);
    }
    rewind(bitmap_file);
    return header;
}

uint8_t * load_image_data(FILE * bitmap_file, header_T header){
    fseek(bitmap_file, header.h_data_offset, SEEK_SET);
    int bits_per_pixel = header.ih_bits_per_pixel / sizeof(uint8_t);
    int bytes_in_data = header.ih_width * header.ih_height * bits_per_pixel;
    uint8_t * image_data = malloc(sizeof(uint8_t) * bytes_in_data);
    fread(image_data, sizeof(uint8_t), bytes_in_data, bitmap_file);
    return image_data;
}

image_T load_bmp_from_file(FILE * bitmap_file){
    image_T image;
    header_T header = load_bmp_header(bitmap_file);
    printf("Image size: %ux%u, depth: %u\n", header.ih_height, header.ih_width, header.ih_bits_per_pixel);
    if(header.ih_bits_per_pixel < 24){
        printf("Unsupported format. Only 24 bit color images supported.\n");
        return (image_T) {0};
    }
    image.height = header.ih_height;
    image.width = header.ih_width;
    image.depth = header.ih_bits_per_pixel;
    image.data = load_image_data(bitmap_file, header);
    return image;
}

