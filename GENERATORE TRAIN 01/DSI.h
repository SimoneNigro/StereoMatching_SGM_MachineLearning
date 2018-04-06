#ifndef DSI_H
#define DSI_H

#include <stdlib.h>
#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>
#include "typeSS.h"

typedef struct _DSI
{
	int width;
	int height;
	int dMax;
	int  *values;
} t_DSI;

t_DSI* create_DSI(int width, int height, int dmax);

void free_DSI(t_DSI * DSI);

#define DSI_ELEMENT(DSI,x,y,d) *(DSI->values+ d*DSI->width*DSI->height +y*DSI->width +x);

void add_costs_array_DSI(t_DSI* DSI, int x, int y, int* costs_array);

void disparity_map(t_DSI* DSI, IplImage* disparity_map);

void printDSI(t_DSI * DSI, char* filename);

t_DSI* readDSI(char* filename);

#endif
