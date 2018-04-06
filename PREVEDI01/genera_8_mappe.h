#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>

#include "matrix.h"
#include "kitti.h"
#include "SGM.h"
#include "disparity.h"

#include <math.h>

#define BYTE unsigned char

using namespace std;
using namespace cv;

void genera_test(IplImage *DispMap, Mat testing_data);

void prepara_test (t_DSI *normale, IplImage *LeftImage, IplImage *RightImage, IplImage** mappe, Mat *matrici_test, t_DSI** scanline_DSI, float **pesi, float **medie);
