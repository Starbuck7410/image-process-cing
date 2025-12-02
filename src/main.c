#include "../include/fft.h"
#include "../include/windows.h"
#include "../include/types.h"
#include "../include/bmp.h"
int main(){
    FILE * bitmap_file = fopen("test.bmp", "r");
    FILE * output_file = fopen("output.bmp", "wb");
    image_T image = load_bmp_from_file(bitmap_file);
    printf("Image size: %ux%u, depth: %u\n", image.header.ih_height, image.header.ih_width, image.header.ih_bits_per_pixel);
    
    write_bmp(image, output_file);
    fclose(output_file);
    fclose(bitmap_file);
    free(image.data);
}