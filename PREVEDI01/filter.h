#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>

int UC(int **minCostMatrixL,int **disparity_left,int w, int h,int scale_factor,int **UCmatrix,int **UCmatrixBW,int **UCcard, char *filename);

int LC(int *vettore,int **minCostMatrixL,int w, int h, int dMax,int **LCmatrix, char *filename);

int NEM(double **costSumNEM,int *vettore, int w, int h, int dMax,double **NEMmatrix, char *filename);

int MLM(int **minCostMatrixL,double **mlmMatrix,int *vettore,int w, int h, int dMax, int **MLMmatrix, char *filename);

int MSM(int **minCostMatrixL,int w, int h , int **MSMmatrix, char *filename);

int PKRN(int **minCostMatrixL,int **secondMinL, int w, int h , int **PKRNmatrix, char *filename);

int PKR(int **minCostMatrixL,int **LocalMinL, int w, int h , int **PKRmatrix, char *filename);

int WMN(int **minCostMatrixL,int **LocalMinL,int **costSum, int w, int h,int **WMNmatrix, char *filename);

int WMNN(int **minCostMatrixL,int **secondMinL,int **costSum,int w, int h,int **WMNNmatrix, char *filename);

int MMN(int **minCostMatrixL,int **secondMinL, int w, int h, int **MMNmatrix, char *filename);

int CUR(int **minCostMatrixL,int *vettore, int w, int h , int dMax, int **CURmatrix, char *filename);

int NOI( int w, int h , int **CardMatrix,int **NOImatrix, char *filename);

int AML(int w, int h ,double **amlMatrix,int **AMLmatrix, char *filename);

int LRC(int **disparity_left,int **disparity_right, int w, int h ,int scale_factor, int **LRCmatrix, int **B_and_W, char *filename);

int LRD(int **minCostMatrixL,int **disparity_left, int **secondMinL, int **minCostMatrixR, int w, int h , int ** LRDmatrix, char *filename);

int DTS(IplImage *left,int w, int h, int dMax, int dMin, int **DTSmatrix, char *filename);

int DSM(int **minCostMatrixL, int **minIndexMatrixL, int **DTSmatrixL, int **DTSmatrixR, int w, int h, int dMax, int dMin, int **DSMmatrix, char *filename);

int SAMM(IplImage *left, IplImage *right, int **minIndexMatrixL, int dMax, int dMin, int **SAMMMmatrix, char *filename);

int PER(int *DSI, int **minCostMatrixL, int **minIndexMatrixL, int w, int h, int dMax, int dMin, int **PERmatrix, char *filename);
