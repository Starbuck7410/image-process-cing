#include "../include/2d-dft.h"
#include "../include/dft.h"


dgray_image_T convert_image_to_dgray(image_T image){ // Double Gray -> dgray
    size_t image_size_px = image.header.ih_height * image.header.ih_width;
    double * dgray_image_data = malloc(image_size_px * sizeof(double));

    for (uint32_t i = 0; i < image.header.ih_width * image.header.ih_height * 3; i+=3){
        double gray = ((77 * image.data[i] +          // B
                                150 * image.data[i+1] +        // G
                                29 * image.data[i+2]) >> 8);   // R
        gray /= 256;
        dgray_image_data[i/3] = gray;
    }
    dgray_image_T dgray_image = {
        .data = dgray_image_data,
        .height = image.header.ih_height,
        .width = image.header.ih_width
    };
    return dgray_image;
}


header_T create_header_for_dgray(dgray_image_T dgray_image){
    uint32_t image_size_bytes = dgray_image.height * dgray_image.height * 3
                              + BMP_HEADER_SIZE + BMP_INFOHEADER_SIZE; 
    header_T header = {
        .h_signature = 'B' | 'M' << 8,
        .h_file_size = image_size_bytes,
        .h_unused_0 = 0,
        .h_data_offset = BMP_HEADER_SIZE + BMP_INFOHEADER_SIZE,
        .ih_size = BMP_INFOHEADER_SIZE,
        .ih_width = dgray_image.width,
        .ih_height = dgray_image.height,
        .ih_planes = 1,
        .ih_bits_per_pixel = 24,
        .ih_compression = 0,
        .ih_image_size = 0,
        .ih_x_pixels_per_m = 3000,
        .ih_y_pixels_per_m = 3000,
        .ih_colors_used = 0,
        .ih_important_colors = 0
    };
    return header;
}

header_T create_header_for_cd(cd_image_T cd_image){
    uint32_t image_size_bytes = cd_image.height * cd_image.height * 3
                              + BMP_HEADER_SIZE + BMP_INFOHEADER_SIZE; 
    header_T header = {
        .h_signature = 'B' | 'M' << 8,
        .h_file_size = image_size_bytes,
        .h_unused_0 = 0,
        .h_data_offset = BMP_HEADER_SIZE + BMP_INFOHEADER_SIZE,
        .ih_size = BMP_INFOHEADER_SIZE,
        .ih_width = cd_image.width,
        .ih_height = cd_image.height,
        .ih_planes = 1,
        .ih_bits_per_pixel = 24,
        .ih_compression = 0,
        .ih_image_size = 0,
        .ih_x_pixels_per_m = 3000,
        .ih_y_pixels_per_m = 3000,
        .ih_colors_used = 0,
        .ih_important_colors = 0
    };
    return header;
}

image_T convert_dgray_to_image(dgray_image_T dgray_image){ 
    header_T header = create_header_for_dgray(dgray_image);
    image_T image = {
        .header = header
    };
    size_t image_size_px = image.header.ih_height * image.header.ih_width;
    size_t bytes_per_pixel = image.header.ih_bits_per_pixel / 8;
    uint8_t * image_data = malloc(image_size_px * bytes_per_pixel * sizeof(uint8_t));

    for (uint32_t i = 0; i < image_size_px * bytes_per_pixel; i+=3){
        image_data[i] = (int) (dgray_image.data[i/3] * 256);
        image_data[i + 1] = (int) (dgray_image.data[i/3] * 256);
        image_data[i + 2] = (int) (dgray_image.data[i/3] * 256);
    }
    image.data = image_data;
    return image;
}

// hue must be between 0 and 6 for odd comfort
void hv_to_rgb(double hue, double value, uint8_t * red, uint8_t * green, uint8_t * blue){ 
    uint8_t x = (uint8_t) (256.0 * value * (1 - fabs(fmod(hue, 2.0) - 1)));
    if      (hue < 1) { *red = (uint8_t) (value * 256.0); *green = x; *blue = 0; }
    else if (hue < 2) { *red = x; *green = (uint8_t) (value * 256.0); *blue = 0; }
    else if (hue < 3) { *red = 0; *green = (uint8_t) (value * 256.0); *blue = x; }
    else if (hue < 4) { *red = 0; *green = x; *blue = (uint8_t) (value * 256.0); }
    else if (hue < 5) { *red = x; *green = 0; *blue = (uint8_t) (value * 256.0); }
    else              { *red = (uint8_t) (value * 256.0); *green = 0; *blue = x; }
}

image_T convert_cd_to_image(cd_image_T cd_image){ 
    header_T header = create_header_for_cd(cd_image);
    image_T image = {
        .header = header
    };
    size_t image_size_px = image.header.ih_height * image.header.ih_width;
    size_t bytes_per_pixel = image.header.ih_bits_per_pixel / 8;
    uint8_t * image_data = malloc(image_size_px * bytes_per_pixel * sizeof(uint8_t));

    double max = 0;
    for(size_t i = 0; i < image_size_px; i++){
        double contender = cmplx_abs(cd_image.data[i]);
        max = (contender > max) ? contender : max;
    } 

    for (uint32_t i = 0; i < image_size_px * bytes_per_pixel; i+=3){
        double magnitude = 1 + cmplx_abs(cd_image.data[i/3]);
        double value = log10(magnitude) / log10(1 + max);
        double hue = (3 * cmplx_angle(cd_image.data[i/3])) / M_PI; 
        uint8_t red, green, blue;
        if (hue < 0) hue += 6.0; // So it's between 0 and 6
        hv_to_rgb(hue, value, &red, &green, &blue);
        image_data[i] = blue;
        image_data[i + 1] = green;
        image_data[i + 2] = red;
    }
    image.data = image_data;
    return image;
}



cdouble_T * get_cd_column(cd_image_T cd_image, size_t column){
    cdouble_T * column_data = malloc(cd_image.height * sizeof(cdouble_T));
    for(size_t i = 0; i < cd_image.height; i++){
        column_data[i] = cd_image.data[column + i * cd_image.width];
    }
    return column_data;
}

void store_cd_column(cd_image_T cd_image, size_t column, cdouble_T * column_data){
    for(size_t i = 0; i < cd_image.height; i++){
        cd_image.data[column + i * cd_image.width] = column_data[i];
    }
    free(column_data);
}

void cd_fft_shift(cd_image_T cd_image){
    cdouble_T * temp_row = malloc(cd_image.width * sizeof(cdouble_T));
    for(size_t i = 0; i < cd_image.height / 2; i++){
        cdouble_T * cd_row_bottom = cd_image.data + cd_image.width * i;
        cdouble_T * cd_row_top = cd_image.data + cd_image.width * (i + cd_image.height / 2) ;
        memcpy(temp_row, cd_row_top, cd_image.width * sizeof(cdouble_T));
        memcpy(cd_row_top, cd_row_bottom, cd_image.width * sizeof(cdouble_T));
        memcpy(cd_row_bottom, temp_row, cd_image.width * sizeof(cdouble_T));
    } 
    free(temp_row);
    cdouble_T * temp_col = malloc(cd_image.height * sizeof(cdouble_T));
    for(size_t i = 0; i < cd_image.width / 2; i++){
        cdouble_T * cd_col_left = get_cd_column(cd_image, i);
        cdouble_T * cd_col_right = get_cd_column(cd_image, i + cd_image.width/2);
        memcpy(temp_col, cd_col_left, cd_image.height * sizeof(cdouble_T));
        store_cd_column(cd_image, i, cd_col_right);
        store_cd_column(cd_image, i + cd_image.width/2, cd_col_left);
    } 
    free(temp_col);
}

cd_image_T calculate_2d_dft(dgray_image_T dgray_image){
    cdouble_T * cd_image_data = malloc(dgray_image.height * dgray_image.width * sizeof(cdouble_T));
    cd_image_T cd_image = {
        .width = dgray_image.width,
        .height = dgray_image.height,
        .data = cd_image_data
    };
    for(size_t i = 0; i < dgray_image.height; i++){
        double * d_row = dgray_image.data + dgray_image.width * i;
        cdouble_T * cd_row = cd_image.data + cd_image.width * i;
        cdouble_T * row_dft = calculate_real_dft(d_row, dgray_image.width);
        memcpy(cd_row, row_dft, dgray_image.width * sizeof(cdouble_T));
        free(row_dft);
    } 
    for(size_t i = 0; i < dgray_image.width; i++){
        cdouble_T * cd_col = get_cd_column(cd_image, i);
        cdouble_T * col_dft = calculate_dft(cd_col, cd_image.height);
        store_cd_column(cd_image, i, col_dft);
        free(cd_col);
    } 
    cd_fft_shift(cd_image);
    return cd_image; 
}

