#include "../include/fft.h"
#include "../include/windows.h"
#include "../include/types.h"
#include "../include/loadbmp.h"
int main(){
    FILE * bitmap_file = fopen("test.bmp", "r");
    image_T image = load_bmp_from_file(bitmap_file);
    printf("%u %u %u\n", (uint32_t) image.data[0], (uint32_t) image.data[1], (uint32_t) image.data[2]);
    fclose(bitmap_file);
    free(image.data);
}