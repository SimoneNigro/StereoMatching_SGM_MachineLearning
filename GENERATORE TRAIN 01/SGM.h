#ifndef SGM_H
#define SGM_H

#include "DSI.h"
#include "kitti.h"

void SGM();

void compute(IplImage* L, IplImage* R, int dMax, float P1, float P2, t_DSI* DSI, int mask, t_DSI** scanline_DSI);

void single_scanline(IplImage* L, IplImage* R, int dMax, float P1, float P2, t_DSI* DSI, int type);

void init_xy(int*x, int*y, int width, int height, int typeSS);

int scan_xy(int* x,int* y,int width,int height, int typeSS);

void global_costs(IplImage *L, IplImage* R, int x, int y, int dMax, float P1, float P2, int* previous_global_costs, int* global_costs);

int cost(IplImage* L, IplImage* R, int x, int y, int d);

float adjust_P2(IplImage* image, int x, int y, int previous_x, int previous_y, float P2);

#endif
