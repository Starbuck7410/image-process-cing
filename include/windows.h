#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define M_PI 3.14159265358979323846

typedef struct window_T {
    double * data;
    char type[6];
    int fade;
    int length; 
    int hop;
} window_T;


void create_window(window_T * window);