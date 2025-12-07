#include "../include/dft.h"
#include "../include/windows.h"
#include "../include/types.h"
#include "../include/bmp.h"
#include "../include/2d-dft.h"

int main(){
    FILE * bitmap_file = fopen("test.bmp", "r");
    FILE * output_file = fopen("output.bmp", "wb");
    FILE * testout_file = fopen("testout.bmp", "wb");
    image_T image = load_bmp_from_file(bitmap_file);
    printf("Image size: %ux%u, depth: %u\n", 
                        image.header.ih_height, 
                        image.header.ih_width, 
                        image.header.ih_bits_per_pixel);
    dgray_image_T dgray_image = convert_image_to_dgray(image);
    cd_image_T cd_fft_image = calculate_2d_dft(dgray_image);
    image_T fft_image = convert_cd_to_image(cd_fft_image);
    cdouble_T * test_data = malloc(sizeof(cdouble_T) * 100 * 100);
    cd_image_T test_cd_image = {
        .height = 100,
        .width = 100,
        .data = test_data
    };
    cdouble_T value = {
        .re = 0.0,
        .im = 1.0
    };
    printf("Complex angle of value: %f\n", cmplx_angle(value));
    for(int i = 0; i < 100 * 100; i++){
        
        test_cd_image.data[i] = value;
    }
    image_T testout_image = convert_cd_to_image(test_cd_image);
    // image_T gray_image = convert_dgray_to_image(dgray_image);
    write_bmp(fft_image, output_file);
    write_bmp(testout_image, testout_file);
    fclose(output_file);
    fclose(bitmap_file);
    fclose(testout_file);
    free(dgray_image.data);
    // free(gray_image.data);
    free(image.data);
}