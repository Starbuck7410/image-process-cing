#include "../include/dft.h"

cdouble_T cmplx_multiply(cdouble_T x, cdouble_T y){
    cdouble_T product = {0,0};
    product.re = x.re * y.re - x.im * y.im;
    product.im = x.re * y.im + y.re * x.im;
    return product;
}

cdouble_T cmplx_magnify(double magnitude, cdouble_T number){
    cdouble_T out = {number.re, number.im};
    out.re *= magnitude;
    out.im *= magnitude;
    return out;
}

cdouble_T cmplx_add(cdouble_T x, cdouble_T y){
    cdouble_T sum = {0,0};
    sum.re = x.re + y.re;
    sum.im = x.im + y.im;
    return sum;
}

cdouble_T cis(double theta){
    cdouble_T cis = {0, 0};
    cis.re = cos(theta);
    cis.im = sin(theta);
    return cis;
}


double cmplx_abs(cdouble_T number){
    return sqrt(pow(number.re, 2) + pow(number.im, 2));
}

double cmplx_angle(cdouble_T number){
    return atan2(number.im, number.re);
}

cdouble_T cmplx_normalize(cdouble_T number){
    cdouble_T normalized = {0.0, 0.0};
    double abs = cmplx_abs(number);
    normalized.re = number.re / abs;
    normalized.im = number.im / abs;
    return normalized;
}

void cmplx_print(cdouble_T number){
    printf("%.3f + %.3fi\n", number.re, number.im);
    return;
}

int is_power_of_two(int number){
    return (number & (number - 1)) == 0;
}


void fft_recursive(cdouble_T * X, uint32_t N) {
    if (N <= 1) return;

    cdouble_T * even = malloc((N / 2) * sizeof(cdouble_T));
    cdouble_T * odd  = malloc((N / 2) * sizeof(cdouble_T));

    if(even == NULL || odd == NULL){
        printf("ERROR: fft_recursive() - Failed to allocate memory.\n");
        return;
    }

    for (uint32_t i = 0; i < N / 2; i++) {
        even[i] = X[2 * i];
        odd[i]  = X[2 * i + 1];
    }

    fft_recursive(even, N/2);
    fft_recursive(odd, N/2);

    for (uint32_t k = 0; k < N/2; k++) {
        cdouble_T exp = cis((double) -2 * M_PI * k / N);
        cdouble_T adj_odd = cmplx_multiply(exp, odd[k]);
        X[k] = cmplx_add(even[k], adj_odd);
        adj_odd = cmplx_magnify(-1, adj_odd);
        X[k + N/2] = cmplx_add(even[k], adj_odd);
    }

    free(even);
    free(odd);
}

int real_fft(double * data, uint32_t N, cdouble_T * out){
    cdouble_T * X = (cdouble_T * ) malloc((N) * sizeof(cdouble_T));
    for(size_t i = 0; i < N; i++){
        X[i].re = data[i];
        X[i].im = 0;
    }  

    fft_recursive(X, N);

    memcpy(out, X, N * sizeof(cdouble_T));
    free(X);
    return 0; 
}


int real_dft(double * data, uint32_t N, cdouble_T * out){
    for(size_t i = 0; i < N; i++){
        out[i].re = 0;
        out[i].im = 0;
    }
    for (uint32_t k = 0; k < N; k++){
        for (uint32_t i = 0; i < N; i++){ 
            cdouble_T exp = cis(-i * 2 * M_PI * ((double) k)/N);
            out[k] = cmplx_add(out[k], cmplx_magnify(data[i], exp));
        }
    }
    return 0;
}

void fft(cdouble_T * data, uint32_t N, cdouble_T * out){
    memcpy(out, data, N * sizeof(cdouble_T));
    fft_recursive(out, N); 
    return;
}


void dft(cdouble_T * data, uint32_t N, cdouble_T * out){
    for(size_t i = 0; i < N; i++){
        out[i].re = 0;
        out[i].im = 0;
    }
    
    for (uint32_t k = 0; k < N; k++){
        for (uint32_t i = 0; i < N; i++){ 
            cdouble_T exp = cis(-i * 2 * M_PI * ((double) k)/N);
            out[k] = cmplx_add(out[k], cmplx_multiply(data[i], exp));
        }
    }
    return;
}

cdouble_T * calculate_real_dft(double * data , uint32_t N){
    if(N < 1){
        printf("ERROR: real_dft() - N cannot be 0.\n");
        return NULL;
    }
    cdouble_T * X = (cdouble_T * ) malloc(N * sizeof(cdouble_T));
    for (uint32_t i = 0; i < N; i++){
        X[i].re = 0;
        X[i].im = 0;
    }
    if(is_power_of_two(N) != 0){
        real_fft(data, N, X);
    }else{
        real_dft(data, N, X);
    }
    return X;
}

cdouble_T * calculate_dft(cdouble_T * data , uint32_t N){
    if(N < 1){
        printf("ERROR: real_dft() - N cannot be 0.\n");
        return NULL;
    }
    cdouble_T * X = (cdouble_T * ) malloc(N * sizeof(cdouble_T));
    for (uint32_t i = 0; i < N / 2; i++){
        X[i].re = 0;
        X[i].im = 0;
    }
    if(is_power_of_two(N) != 0){
        fft(data, N, X);
    }else{
        dft(data, N, X);
    }
    return X;
}