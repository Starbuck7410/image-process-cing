#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define M_PI 3.14159265358979323846

typedef struct cdouble_T{
    double re;
    double im;

} cdouble_T;


cdouble_T cmplx_multiply(cdouble_T x, cdouble_T y);
cdouble_T cis(double theta);
cdouble_T cmplx_add(cdouble_T x, cdouble_T y);
void cmplx_print(cdouble_T number);
cdouble_T cmplx_normalize(cdouble_T number);
double cmplx_abs(cdouble_T number);
double cmplx_angle(cdouble_T number);
cdouble_T * calculate_real_dft(double * data , uint32_t N);
cdouble_T * calculate_dft(cdouble_T * data , uint32_t N);
cdouble_T cmplx_magnify(double magnitude, cdouble_T number);