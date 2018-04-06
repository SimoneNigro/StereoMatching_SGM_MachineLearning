#ifndef KITTI_H
#define KITTI_H

#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>

//Evalutation area
#ifndef ALL_PIXELS
#define ALL_PIXELS 0
#endif

#ifndef ESTIMATED_PIXELS
#define ESTIMATED_PIXELS 1
#endif

IplImage* KITTI_load_image(char* KITTI_file_name);
void KITTI_save_image(IplImage *KITTI_Image, char* new_KITTI_file_name);
void KITTI_create_disparity_map(IplImage *KITTI_disparity_Image,float** data);
int KITTI_evaluate(char* path_Disparity_map,char*path_GroundTruth_noc,char*path_GroundTruth_occ);
void KITTI_interpolate_BackGround(float** data,int w, int h);

#endif