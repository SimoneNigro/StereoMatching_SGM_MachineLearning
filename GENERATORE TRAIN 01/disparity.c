#include "disparity.h"

int **findDisparity(int *v,int w, int h,int dMax){

	int i,j,d;
	int **resultMatrix;
	int minimum = INT_MAX;
	int minimumIndex = 0;
	int index;

	//Alloco matrice dei minimi
	resultMatrix=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++){
		resultMatrix[i]=(int*)calloc(w,sizeof(int));
	}
	
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			minimum = INT_MAX;
			minimumIndex = 0;
			for(d=0;d<dMax;d++){
				index=d*h*w+i*w+j;
				if(v[index]<minimum){
					minimum=v[index];
					minimumIndex=d;
				}
			}
			resultMatrix[i][j]=minimumIndex;
		}
	}

	return resultMatrix;
}

void getDisparityMap(int *v,int w, int h,int dMax,int offset,IplImage *disparityImage){

	int i,j,d,temp;
	int minimum = INT_MAX;
	int minimumIndex = 0;
	int index;
	
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			minimum = INT_MAX;
			minimumIndex = 0;
			for(d=0;d<dMax;d++){
				index=d*h*w+i*w+j;
				if(v[index]<minimum){
					minimum=v[index];
					minimumIndex=d;
				}
			}
			disparityImage->imageData[i*disparityImage->widthStep+j]=minimumIndex*offset;
			//CV_IMAGE_ELEM(disparityImage,ushort,i,j) = minimumIndex*offset*255; //per kitty
		}
	
	}
}

