#ifndef DISPARITY_H
#define DISPARITY_H

#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>

int **findDisparity(int *v,int w, int h,int dMax);

void getDisparityMap(int *v,int w, int h,int dMax,int offset, IplImage *disparityImage);

#endif