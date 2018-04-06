#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>

#include "standard_Stereo.h"

int StandardStereo_DSI(IplImage *Left, IplImage *Right, int dmin, int dmax, int radius, int TAD_threshold, int *output_DSI)
{
  int x,y,d;
  int Width, Height, WidthStep;


  int **DSI;


  Width = Left->width;
  Height = Left->height;
  WidthStep = Left->widthStep;


  DSI = (int**)calloc ((Width*Height), sizeof (int*));
  if (DSI==NULL) {
    printf("ABORTING... CAN'T ALLOCATE DSI MATRIX\n\n");
    return -1;
  }
  for(x=0; x<Width*Height; x++)
  {
    DSI[x] = (int*)calloc(dmax+1, sizeof(int));
    if (DSI[x]==NULL) {
      printf("ABORTING... CAN'T ALLOCATE DSI VECTORS \n\n");
      return -1;
    }
  }



  // TAD computation

  box_filtering_TAD_stereo_color(Width, Height, dmax+1, 0, Left, Right, DSI, radius, TAD_threshold);
  //park_box_filtering_TAD_stereo_color(Width, Height, dmax+1, 0, Left, Right, DSI, radius, TAD_threshold, 5);
  printf("Box filtering executed\n");

  // memset(Result_LR->imageData, 255, sizeof(BYTE)*Result_LR->widthStep*Result_LR->height);
  // memset(Result_RL->imageData, 255, sizeof(BYTE)*Result_RL->widthStep*Result_RL->height);
  // memset(Result_LR_SMP->imageData, 255, sizeof(BYTE)*Result_RL->widthStep*Result_RL->height);


  for (y=0;y<Height;y++)
    for (x=0; x<Width; x++)
    {
        for (d=dmin; d<=dmax; d++)
        {
          output_DSI[Width*Height*d+y*Width+x] = DSI[y*Width+x][d];
        } // end d
    } // for x

  for (x=0; x<Width*Height;x++)
    free(DSI[x]);

  free(DSI);
  return 0;
}
