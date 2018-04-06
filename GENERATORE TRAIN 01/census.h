#ifndef CENSUS_H
#define CENSUS_H

#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>

uchar hamming_Distance(uchar left, uchar right);


int census_Transform_Binary(IplImage *image, IplImage *result,int window);

int census_Transform_Ternary(IplImage *image, IplImage *result,int window);

int census_Transform_Binary_Cross(IplImage *image, IplImage *result,int window);

int census_Transform_Ternary_Cross(IplImage *image, IplImage *result,int window);

int sparse_Census_Transform(IplImage *image, IplImage *result,int window);

int mini_Census_Transform(IplImage *image, IplImage *result);

int hamming_Distance_bis(int left, int right);

int hamming_Aggregation(IplImage *L_Trasformed, IplImage *R_Trasformed,int x_L,int y_L,int x_R, int y_R);

#endif 