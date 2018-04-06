#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include "minmax.h"
#include "census.h"

int TAD(int value, int threshold)
{


 if (value<threshold)
  return value;
 else
  return threshold;


}

void park_box_filtering_TAD_stereo_color(int w, int h, int nDisp, int offs_x, IplImage *L, IplImage *R, int** out, int r, int threshold, int census_size)
{
int x,y,i,j,d;
int *acc = (int *) calloc(nDisp, sizeof(int));
int *row1 = (int *) calloc(nDisp, sizeof(int));
int *row2 = (int *) calloc(nDisp, sizeof(int));
int *pos0 = (int *) calloc(nDisp, sizeof(int));
int n = 2*r+1;
int sqn = n*n;

int **V;
V = (int **)calloc(w, sizeof(int*));


for(d=0; d<w; d++){
V[d] = (int *)calloc(nDisp, sizeof(int));
}

IplImage *Lgray = cvCreateImage(cvGetSize(L), 8, 1);
IplImage *Rgray = cvCreateImage(cvGetSize(R), 8, 1);

cvCvtColor(L, Lgray, CV_BGR2GRAY);
cvCvtColor(R, Rgray, CV_BGR2GRAY);

IplImage *censusL = cvCreateImage(cvGetSize(L), 8, 1);
IplImage *censusR = cvCreateImage(cvGetSize(R), 8, 1);

census_Transform_Binary(Lgray, censusL, census_size);
census_Transform_Binary(Rgray, censusR, census_size);

cvShowImage("Census L", censusL);
cvWaitKey();


//############### PRIMA RIGA #################
//FASE "0": PRIMA POSIZIONE A SX
for(d=0; d<nDisp; d++){
V[0][d]=0;
for(j=0; j<r+1; j++)
V[0][d] +=
TAD(abs(((unsigned char)L->imageData[j*L->widthStep])-((unsigned char)R->imageData[j*R->widthStep]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+1])-((unsigned char)R->imageData[j*R->widthStep+1])) +
abs(((unsigned char)L->imageData[j*L->widthStep+2])-((unsigned char)R->imageData[j*R->widthStep+2])),threshold);


acc[d]=V[0][d];


for(i=1; i<r+1; i++)
for(j=0; j<r+1; j++)
acc[d] +=
TAD(abs(((unsigned char)L->imageData[j*L->widthStep+3*i])-((unsigned char)R->imageData[j*R->widthStep+3*(i-MIN(i,d+offs_x))]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*i+1])-((unsigned char)R->imageData[j*R->widthStep+3*(i-MIN(i,d+offs_x))+1]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*i+2])-((unsigned char)R->imageData[j*R->widthStep+3*(i-MIN(i,d+offs_x))+2])),threshold);
out[0][d] = acc[d];


pos0[d] = acc[d];
}


//FASE 1: bordo sinistro
for(x=1; x<r+1; x++){
for(d=0; d<nDisp; d++){
V[x+r][d]=0;
for(j=0; j<r+1; j++)
V[x+r][d] +=
TAD(abs(((unsigned char)L->imageData[j*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[j*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[j*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[j*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold);


acc[d] = acc[d] + V[x+r][d];
out[x][d] = acc[d];
V[x][d]=0;
for(j=0; j<r+1; j++)
V[x][d] +=
TAD(abs(((unsigned char)L->imageData[j*L->widthStep+3*x])-((unsigned char)R->imageData[j*R->widthStep+3*(x-MIN(x,d+offs_x))]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*x+1])-((unsigned char)R->imageData[j*R->widthStep+3*(x-MIN(x,d+offs_x))+1]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*x+2])-((unsigned char)R->imageData[j*R->widthStep+3*(x-MIN(x,d+offs_x))+2])),threshold);
}//d
}//x


//FASE 2: parte centrale
for(x=r+1; x<w-r; x++){
for(d=0; d<nDisp; d++){


V[x+r][d]=0;
for(j=0; j<r+1; j++)
V[x+r][d] +=
TAD(abs(((unsigned char)L->imageData[j*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[j*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[j*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1]))
+ abs(((unsigned char)L->imageData[j*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[j*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold);


acc[d] = acc[d] + V[x+r][d] - V[x-r-1][d];
out[x][d] = acc[d];
}//d
}//x


//FASE 3: bordo destro
for(x=w-r; x<w; x++){
for(d=0; d<nDisp; d++){


acc[d] = acc[d] - V[x-r-1][d];
out[x][d] = acc[d];
}//d
}//x


//######## BORDO IN ALTO ############
for(y=1; y<r+1; y++){
//FASE "0": PRIMA POSIZIONE A SX
for(d=0; d<nDisp; d++){
V[0][d] = V[0][d] +
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep])-((unsigned char)R->imageData[(y+r)*R->widthStep]))
+ abs(((unsigned char)L->imageData[(y+r)*L->widthStep+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+1]))
+ abs(((unsigned char)L->imageData[(y+r)*L->widthStep+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+2])),threshold);


row2[d] = 0;
for(i=0; i<r+1; i++)
row2[d] +=
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*i])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(i-MIN(i,d+offs_x))]))
+ abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*i+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(i-MIN(i,d+offs_x))+1]))
+ abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*i+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(i-MIN(i,d+offs_x))+2])),threshold);


acc[d] = pos0[d] + row2[d];
pos0[d] = acc[d];


out[y*w][d] = acc[d];
}


//FASE 1: bordo sinistro
for(x=1; x<r+1; x++){
for(d=0; d<nDisp; d++){
V[x+r][d] = V[x+r][d] +
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))]))
+ abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1]))
+ abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold);


acc[d] = acc[d] + V[x+r][d];
out[y*w+x][d] = acc[d];
V[x][d] = V[x][d] +
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*x])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x-MIN(x,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*x+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x-MIN(x,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*x+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x-MIN(x,d+offs_x))+2])),threshold);
}
}//x


//FASE 2: parte centrale
for(x=r+1; x<w-r; x++){
for(d=0; d<nDisp; d++){
V[x+r][d] = V[x+r][d] +
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold);


acc[d] = acc[d] + V[x+r][d] - V[x-r-1][d];
out[y*w+x][d] = acc[d];
}
}//x


//FASE 3: bordo destro
for(x=w-r; x<w; x++){
for(d=0; d<nDisp; d++){
acc[d] = acc[d] - V[x-r-1][d];
out[y*w+x][d] = acc[d];
}
}//x
}//y


//######## PARTE CENTRALE ############
for(y=r+1; y<h-r; y++){


//FASE "0": PRIMA POSIZIONE A SX
for(d=0; d<nDisp; d++){
V[0][d] = V[0][d]
- ( TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep])-((unsigned char)R->imageData[(y-r-1)*R->widthStep])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+2])),threshold))
+
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep])-((unsigned char)R->imageData[(y+r)*R->widthStep])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+1])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+2])),threshold);



row1[d] = 0;
for(i=0; i<r+1; i++)


row1[d] +=
TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*i])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(i-MIN(i,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*i+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(i-MIN(i,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*i+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(i-MIN(i,d+offs_x))+2])),threshold);


row2[d] = 0;
for(i=0; i<r+1; i++)
row2[d] +=
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*i])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(i-MIN(i,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*i+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(i-MIN(i,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*i+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(i-MIN(i,d+offs_x))+2])),threshold);


acc[d] = pos0[d] - row1[d] + row2[d];
pos0[d] = acc[d];


out[y*w][d] = acc[d];
}


//FASE 1: bordo sinistro
for(x=1; x<r+1; x++){
for(d=0; d<nDisp; d++){
V[x+r][d] = V[x+r][d]
- (TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))])) +
  abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1])) +
  abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold))
+
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold);
acc[d] = acc[d] + V[x+r][d];
out[y*w+x][d] = acc[d];
V[x][d] = V[x][d]
- (TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*x])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x-MIN(x,d+offs_x))])) +
  abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*x+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x-MIN(x,d+offs_x))+1])) +
  abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*x+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x-MIN(x,d+offs_x))+2])),threshold))
+
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*x])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x-MIN(x,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*x+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x-MIN(x,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*x+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x-MIN(x,d+offs_x))+2])),threshold);
}
}//x


//FASE 2: parte centrale
for(x=r+1; x<w-r; x++){
for(d=0; d<nDisp; d++){
V[x+r][d] = V[x+r][d] -
(TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold))
+
TAD(abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y+r)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y+r)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold);


acc[d] = acc[d] + V[x+r][d] - V[x-r-1][d];
out[y*w+x][d] = acc[d];
}
}//x


//FASE 3: bordo destro
for(x=w-r; x<w; x++){
for(d=0; d<nDisp; d++){
acc[d] = acc[d] - V[x-r-1][d];
out[y*w+x][d] = acc[d];
}
}//x
}//y


//######## BORDO IN BASSO ############
for(y=h-r; y<h; y++){


//FASE "0": PRIMA POSIZIONE A SX
for(d=0; d<nDisp; d++){
V[0][d] = V[0][d] -
(TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep])-((unsigned char)R->imageData[(y-r-1)*R->widthStep])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+2])),threshold));


row1[d] = 0;
for(i=0; i<r+1; i++)
row1[d] +=
TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*i])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(i-MIN(i,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*i+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(i-MIN(i,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*i+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(i-MIN(i,d+offs_x))+2])),threshold);
acc[d] = pos0[d] - row1[d];
pos0[d] = acc[d];


out[y*w][d] = acc[d];
}


//FASE 1: bordo sinistro
for(x=1; x<r+1; x++){
for(d=0; d<nDisp; d++){
V[x+r][d] = V[x+r][d] -
(TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold));


acc[d] = acc[d] + V[x+r][d];
out[y*w+x][d] = acc[d];


V[x][d] = V[x][d] -
(TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*x])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x-MIN(x,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*x+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x-MIN(x,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*x+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x-MIN(x,d+offs_x))+2])),threshold));
}
}//x


//FASE 2: parte centrale
for(x=r+1; x<w-r; x++){
for(d=0; d<nDisp; d++){
V[x+r][d] = V[x+r][d] -
(TAD(abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+1])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+1])) +
abs(((unsigned char)L->imageData[(y-r-1)*L->widthStep+3*(x+r)+2])-((unsigned char)R->imageData[(y-r-1)*R->widthStep+3*(x+r-MIN(x+r,d+offs_x))+2])),threshold));
acc[d] = acc[d] + V[x+r][d] - V[x-r-1][d];
out[y*w+x][d] = acc[d];
}
}//x


//FASE 3: bordo destro
for(x=w-r; x<w; x++){
for(d=0; d<nDisp; d++){
acc[d] = acc[d] - V[x-r-1][d];
out[y*w+x][d] = acc[d];
}
}//x
}//y


for(d=0; d<w;d++){
free(V[d]);
}
free(V);
free(acc);
free(row1);
free(row2);
free(pos0);
}
