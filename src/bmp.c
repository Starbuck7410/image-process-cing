#include "../include/bmp.h"

// everything will be stored as 4 byte thingies. uint32_t
uint32_t load_n_bytes(FILE * bitmap_file, size_t n){
    uint32_t result = 0;
    fread(&result, n, 1, bitmap_file);
    return result;
}

// writes ONE value, not a whole buffer
void write_n_bytes(FILE * bitmap_file, uint32_t value, size_t n){
    fwrite(&value, n, 1, bitmap_file);
}

header_T load_bmp_header(FILE * bitmap_file){
    size_t field_sizes[HEADER_FIELD_COUNT] = HEADER_FIELD_SIZES;
    header_T header;
    uint32_t * header_pointer = &header.h_signature;
    for(size_t i = 0; i < HEADER_FIELD_COUNT; i++){
        header_pointer[i] = load_n_bytes(bitmap_file, field_sizes[i]);
    }
    rewind(bitmap_file);
    return header;
}

uint8_t * load_image_data(FILE * bitmap_file, header_T header){
    fseek(bitmap_file, header.h_data_offset, SEEK_SET);
    int bits_per_pixel = header.ih_bits_per_pixel / 8;
    int bytes_in_data = header.ih_width * header.ih_height * bits_per_pixel;
    uint8_t * image_data = malloc(sizeof(uint8_t) * bytes_in_data);
    fread(image_data, sizeof(uint8_t), bytes_in_data, bitmap_file);
    return image_data;
}

image_T load_bmp_from_file(FILE * bitmap_file){
    image_T image;
    header_T header = load_bmp_header(bitmap_file);
    if(header.ih_bits_per_pixel < 24){
        printf("Unsupported format. Only 24 bit color images supported.\n");
        return (image_T) {0};
    }
    image.header = header;
    image.data = load_image_data(bitmap_file, header);
    return image;
}

void write_bmp_header(FILE * bitmap_file, header_T header){
    size_t field_sizes[HEADER_FIELD_COUNT] = HEADER_FIELD_SIZES;
    uint32_t * header_pointer = &header.h_signature;
    for(size_t i = 0; i < HEADER_FIELD_COUNT; i++){
        write_n_bytes(bitmap_file, header_pointer[i], field_sizes[i]);
    }
}

void write_bmp(image_T image, FILE * bitmap_file){
    rewind(bitmap_file);
    write_bmp_header(bitmap_file, image.header);
    fseek(bitmap_file, image.header.h_data_offset, SEEK_SET);
    int bits_per_pixel = image.header.ih_bits_per_pixel / 8;
    int bytes_in_data = image.header.ih_width * image.header.ih_height * bits_per_pixel;
    fwrite(image.data, sizeof(uint8_t), bytes_in_data, bitmap_file);
}