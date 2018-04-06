#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>
#include "kitti.h"

#pragma warning (disable : 4996)

int isValid(float** data,int row, int col){

	int result = -1;
	if(data[row][col]>=0){
		result=0;
	}
	return result;
}

float getDisp(float** data, int row, int col){
	return data[row][col];
}

void setDisp(float** data, int row, int col,float value){
	 data[row][col]=value;
}

float myAbs(float value){
	return value >= 0 ? value : -value;
}

float myMax(float value1,float value2){
	return value1 >= value2 ? value1 : value2;
}

float myMin(float value1,float value2){
	return value1 <= value2 ? value1 : value2;
}

void KITTI_interpolate_BackGround(float** data,int w, int h){

	int i, j, jCurr, j1, j2, i2, countInvalid;
	float idPol;
	
	for(i=0;i<h;i++){
		
		countInvalid = 0;

		for(j=0;j<w;j++){
		
			if(isValid(data,i,j)==0){

				if(countInvalid>=1){
				
					//first and last value for interpolation
					j1 = j-countInvalid;
					j2 = j-1;

					if(j1>0 && j2<w-1){
						idPol = getDisp(data,i,j1-1)<=getDisp(data,i,j2+1)?
								getDisp(data,i,j1-1):getDisp(data,i,j2+1);
						for(jCurr=j1;jCurr<=j2;jCurr++){
							setDisp(data,i,jCurr,idPol);
						}
					}

				}

				countInvalid = 0;

			}
			else{
				countInvalid++;
			}		
		}

		 // to the left
		for(j=0;j<w;j++){
			if(isValid(data,i,j)==0){
				for(j2=0;j2<j;j2++){
					setDisp(data,i,j2,getDisp(data,i,j));
				}
				break;
			}
		}

		// to the right
		for(j=w-1;j>=0;j--){
			if(isValid(data,i,j)==0){
				for(j2=j+1;j2<=w-1;j2++){
					setDisp(data,i,j2,getDisp(data,i,j));
				}
				break;
			}
		}
	}

	for(j=0;j<w;j++){

		//to the top
		for(i=0;i<h;i++){
			if(isValid(data,i,j)==0){
				for(i2=0;i2<i;i2++){
					setDisp(data,i2,j,getDisp(data,i,j));
				}
				break;
			}
		}
		//to the bottom
		for(i=h-1;i>=0;i--){
			if(isValid(data,i,j)==0){
				for(i2=i+1;i2<=h-1;i2++){
					setDisp(data,i2,j,getDisp(data,i,j));
				}
				break;
			}
		}
	}
}

int KITTI_disparityErrorsAverage(float** data_GroudTruth, float** data_disparity, float** data_interpolate_bg, float *errors,int h, int w){

	float error;
	int allPixels=0, estimatedPixels = 0;
	int i,j;	

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(isValid(data_GroudTruth,i,j)==0){
				error = myAbs(getDisp(data_GroudTruth,i,j)-getDisp(data_interpolate_bg,i,j));
				errors[ALL_PIXELS] += error;
				allPixels++;
				if(isValid(data_disparity,i,j)==0){
					errors[ESTIMATED_PIXELS] += error;
					estimatedPixels++;
				}
			}
		}
	}

	errors[ALL_PIXELS] =errors[ALL_PIXELS]/myMax((float)allPixels,1.0);
	errors[ESTIMATED_PIXELS] /= myMax((float)estimatedPixels,1.0);

	return 0;

}

int KITTI_disparityErrorsOutlier(float** data_GroudTruth, float** data_disparity,float **KITTI_interpolate_bg,float* errors,int h, int w){

	float error;
	int allPixels=0, estimatedPixels = 0;
	int i,j,k;

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(isValid(data_GroudTruth,i,j)==0){
				error = myAbs(getDisp(data_GroudTruth,i,j)-getDisp(KITTI_interpolate_bg,i,j));
				for(k=0;k<5;k++){
					if(error>(float)(k+1)){
						errors[k*2+0]++;
					}
				}
				allPixels++;
				if(isValid(data_disparity,i,j)==0){
					for(k=0;k<5;k++){
						if(error>(float)(k+1)){
							errors[k*2+1]++;
					}
				}
				estimatedPixels++;
				}
			}
		}
	}
	return 0;
}

float KITTI_density(float** data_GroudTruth, float** data_disparity,int h, int w){

	int allPixels=0, estimatedPixels = 0;
	int i,j;

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(isValid(data_GroudTruth,i,j)==0){
				allPixels++;
				if(isValid(data_disparity,i,j)==0){
					estimatedPixels++;
				}
			}
		}
	}

	return (float)estimatedPixels/myMax((float)allPixels,1.0);
}

IplImage* KITTI_load_image(char* KITTI_file_name){

	IplImage *KITTI_image = cvLoadImage(KITTI_file_name,CV_LOAD_IMAGE_UNCHANGED);
	return KITTI_image;

}

void KITTI_save_image(IplImage *KITTI_Image, char* new_KITTI_file_name){

	int compression_parameters[3];
	
	compression_parameters[0]=CV_IMWRITE_PNG_COMPRESSION;
	compression_parameters[1]=0;
	compression_parameters[2]=0;

	cvSaveImage(new_KITTI_file_name,KITTI_Image,compression_parameters);

}

void KITTI_create_disparity_map(IplImage *KITTI_disparity_Image,float** data){

	int i=0, j=0,h,w;
	unsigned short value=0;
	h=KITTI_disparity_Image->height;
	w=KITTI_disparity_Image->width;


	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			value = CV_IMAGE_ELEM(KITTI_disparity_Image,unsigned short,i,j);
			if(value==0){
				data[i][j]=-1.0;
			}
			else{
				data[i][j]=(CV_IMAGE_ELEM(KITTI_disparity_Image,unsigned short,i,j)/256.0);
			}
		}
	}
}

int KITTI_evaluate(char* path_Disparity_map,char*path_GroundTruth_noc,char*path_GroundTruth_occ){

	IplImage *imageDisparity = NULL;
	IplImage *imageGT_noc = NULL;
	IplImage *imageGT_occ = NULL;
	IplImage *imageInterpolate_bg = NULL;

	int i,k,y;
	char *fileName;
	char *path_GT_occ;
	char *path_GT_noc;
	char *path_disp;
	float* errors;
	float* errorsOut;
	float **data;
	float **dataGt_noc;
	float **dataGt_occ;
	float **dataIpol;

	FILE *errorsAverageFile;
	FILE *errorsOutlierFile;
	FILE *errorsAverageFileOcc;
	FILE *errorsOutlierFileOcc;


	fileName = (char*)calloc(13,sizeof(char));
	path_GT_occ = (char*)calloc(13,sizeof(char));
	path_GT_noc = (char*)calloc(13,sizeof(char));
	path_disp = (char*)calloc(13,sizeof(char));


	errorsAverageFile = fopen("errorsAverageNoc.txt","w");
	errorsOutlierFile = fopen("errorsOutlierNoc.txt","w");
	errorsAverageFileOcc = fopen("errorsAverageOcc.txt","w");
	errorsOutlierFileOcc = fopen("errorsOutlierOcc.txt","w");
	strcpy(path_disp,path_Disparity_map);
	strcpy(path_GT_noc,path_GroundTruth_noc);
	strcpy(path_GT_occ,path_GroundTruth_occ);


	for(k=0;k<6;k++){

		sprintf(fileName,"%06d_10.png",k);
		strcat(path_Disparity_map,fileName);
		strcat(path_GroundTruth_noc,fileName);
		strcat(path_GroundTruth_occ,fileName);

		imageDisparity = KITTI_load_image(path_Disparity_map);
		imageInterpolate_bg = KITTI_load_image(path_Disparity_map);
		imageGT_noc = KITTI_load_image(path_GroundTruth_noc);
		imageGT_occ = KITTI_load_image(path_GroundTruth_occ);

		errors = (float*)calloc(2,sizeof(float));
		errorsOut = (float*)calloc(10,sizeof(float));

		data=(float**)calloc(imageDisparity->height,sizeof(int*));
		for(i=0;i<imageDisparity->height;i++){
			data[i]=(float*)calloc(imageDisparity->width,sizeof(float));
		}
		dataGt_occ=(float**)calloc(imageGT_occ->height,sizeof(int*));
			for(i=0;i<imageGT_occ->height;i++){
				dataGt_occ[i]=(float*)calloc(imageGT_occ->width,sizeof(float));
		}
		dataGt_noc=(float**)calloc(imageGT_noc->height,sizeof(int*));
			for(i=0;i<imageGT_noc->height;i++){
				dataGt_noc[i]=(float*)calloc(imageGT_noc->width,sizeof(float));
		}
		dataIpol=(float**)calloc(imageInterpolate_bg->height,sizeof(int*));
			for(i=0;i<imageInterpolate_bg->height;i++){
				dataIpol[i]=(float*)calloc(imageInterpolate_bg->width,sizeof(float));
		}

		KITTI_create_disparity_map(imageDisparity,data);
		KITTI_create_disparity_map(imageGT_noc,dataGt_noc);
		KITTI_create_disparity_map(imageGT_occ,dataGt_occ);
		KITTI_create_disparity_map(imageInterpolate_bg,dataIpol);

		KITTI_interpolate_BackGround(dataIpol,imageInterpolate_bg->width,imageInterpolate_bg->height);

		KITTI_disparityErrorsAverage(dataGt_noc,data,dataIpol,errors,imageDisparity->height,imageDisparity->width);
		fprintf(errorsAverageFile,"Image Name: %s\t All pixels: %f\t Estimated pixels: %f\n",fileName,errors[ALL_PIXELS],errors[ESTIMATED_PIXELS]);

		KITTI_disparityErrorsAverage(dataGt_occ,data,dataIpol,errors,imageDisparity->height,imageDisparity->width);
		fprintf(errorsAverageFileOcc,"Image Name: %s\t All pixels: %f\t Estimated pixels: %f\n",fileName,errors[ALL_PIXELS],errors[ESTIMATED_PIXELS]);

		KITTI_disparityErrorsOutlier(dataGt_noc,data,dataIpol,errorsOut,imageDisparity->height,imageDisparity->width);
		fprintf(errorsOutlierFile,"\nImage Name: %s\t All pixels: ",fileName);
		for(y=0;y<10;y+=2){
			fprintf(errorsOutlierFile,"%f\t",errorsOut[y]);
		}
		fprintf(errorsOutlierFile,"\nImage Name: %s\t Estimated pixels: ",fileName);
		for(y=1;y<10;y+=2){
			fprintf(errorsOutlierFile,"%f\t",errorsOut[y]);
		}

		KITTI_disparityErrorsOutlier(dataGt_occ,data,dataIpol,errorsOut,imageDisparity->height,imageDisparity->width);
		fprintf(errorsOutlierFileOcc,"\nImage Name: %s\t All pixels: ",fileName);
		for(y=0;y<10;y+=2){
			fprintf(errorsOutlierFileOcc,"%f\t",errorsOut[y]);
		}
		fprintf(errorsOutlierFileOcc,"\nImage Name: %s\t Estimated pixels: ",fileName);
		for(y=1;y<10;y+=2){
			fprintf(errorsOutlierFileOcc,"%f\t",errorsOut[y]);
		}

		strcpy(path_Disparity_map,path_disp);
		strcpy(path_GroundTruth_noc,path_GT_noc);
		strcpy(path_GroundTruth_occ,path_GT_occ);

		free(data);
		free(dataIpol);
		free(dataGt_noc);
		free(dataGt_occ);
	}

	fclose(errorsAverageFile);
	fclose(errorsAverageFileOcc);
	fclose(errorsOutlierFile);
	fclose(errorsOutlierFileOcc);
}

