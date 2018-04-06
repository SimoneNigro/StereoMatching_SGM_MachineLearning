#include <opencv/cv.h>      
#include <opencv/ml.h>		
#include <opencv2/contrib/contrib.hpp>

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "read_data_csv.h"
#include "genera_8_mappe.h"
#include "matrix.h"
#include <math.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <omp.h>
#include "confronta.h"

using namespace cv;
using namespace std;

/******************************************************************************/

//Prende una mappa di disparità e da b/n la fa diventare a colori

int main( int argc, char** argv )
{

 printf ("OpenCV version %s (%d.%d.%d)\n",
            CV_VERSION,
            CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
    
	if (argc != 2)
	{
		printf("Usage: %s immagine\n", argv[0]);
		return 0;
	}
/*
        char name[500];

	int id = atoi(argv[1]);

	char q[10];
	if(id < 10){sprintf(q, "00%d", id); }
	if(id >= 10 && id <= 99){sprintf(q, "0%d", id); }
	if(id >= 100){sprintf(q, "%d", id); }

	//salto le immagini usate per il training
	if(id == 43 || id == 71 ||id == 82 || id == 87 ||id == 94 || id == 120 || id == 122 ||id == 180){
	
	printf("Salto la %d perchè l'ho usata nel training\n", id);	
	return 0;
	}

	Mat risultanteSomma8Pesata;
	Mat risultanteNormale;
	Mat risultanteSomma4;
	Mat risultanteScelti;
	Mat risultanteSomma4Pesata;
	Mat GroundTruth;

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteNormale.png", q);
		
		risultanteNormale = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(risultanteNormale, risultanteNormale, 0, 300, NORM_MINMAX, CV_8U);
	
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultante.png", q);

		risultanteSomma8Pesata = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(risultanteSomma8Pesata, risultanteSomma8Pesata, 0, 380, NORM_MINMAX, CV_8U);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteSomma4Pesata.png", q);

		risultanteSomma4Pesata = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(risultanteSomma4Pesata, risultanteSomma4Pesata, 0, 300, NORM_MINMAX, CV_8U);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteScelti.png", q);

		risultanteScelti = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(risultanteScelti, risultanteScelti, 0, 480, NORM_MINMAX, CV_8U);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteSomma4.png", q);

		risultanteSomma4 = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(risultanteSomma4, risultanteSomma4, 0, 480, NORM_MINMAX, CV_8U);

	sprintf(name, "/home/simo/KITTI/training/disp_noc/000%s_10.png", q);

		GroundTruth = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(GroundTruth, GroundTruth, 0, 255, NORM_MINMAX, CV_8U);

	Mat temp;

	applyColorMap(risultanteNormale,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOLrisultanteNormale.png", q);
	imwrite(name, temp);	

	applyColorMap(risultanteSomma8Pesata,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOLrisultante.png", q);
	imwrite(name, temp);	

	applyColorMap(risultanteSomma4Pesata,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOLrisultanteSomma4Pesata.png", q);
	imwrite(name, temp);	

	applyColorMap(risultanteSomma4,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOLrisultanteSomma4.png", q);
	imwrite(name, temp);	

	applyColorMap(risultanteScelti,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOLrisultanteSscelti.png", q);
	imwrite(name, temp);	

	applyColorMap(GroundTruth,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOL_GT.png", q);
	imwrite(name, temp);	

	Mat MODrisultanteSomma8Pesata;
	Mat MODrisultanteNormale;
	Mat MODrisultanteSomma4;
	Mat MODrisultanteScelti;
	Mat MODrisultanteSomma4Pesata;

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteNormale.png", q);
		
		MODrisultanteNormale = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(MODrisultanteNormale, MODrisultanteNormale, 0, 300, NORM_MINMAX, CV_8U);
	
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultante.png", q);

		MODrisultanteSomma8Pesata = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(MODrisultanteSomma8Pesata, MODrisultanteSomma8Pesata, 0, 255, NORM_MINMAX, CV_8U);
//IplImage tmp=MODrisultanteSomma8Pesata;
		//cvAddS(&tmp, cv::Scalar(100), &tmp, NULL);
//MODrisultanteSomma8Pesata = &tmp;

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteSomma4Pesata.png", q);

		MODrisultanteSomma4Pesata = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(MODrisultanteSomma4Pesata, MODrisultanteSomma4Pesata, 0, 300, NORM_MINMAX, CV_8U);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteScelti.png", q);

		MODrisultanteScelti = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(MODrisultanteScelti, MODrisultanteScelti, 0, 480, NORM_MINMAX, CV_8U);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteSomma4.png", q);

		MODrisultanteSomma4 = imread(name,CV_LOAD_IMAGE_GRAYSCALE);
		normalize(MODrisultanteSomma4, MODrisultanteSomma4, 0, 480, NORM_MINMAX, CV_8U);

if(!countNonZero(MODrisultanteSomma4) < 1){

	applyColorMap(MODrisultanteNormale,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOL_MODrisultanteNormale.png", q);
	imwrite(name, temp);	

	applyColorMap(MODrisultanteSomma8Pesata,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOL_MODrisultante.png", q);
	imwrite(name, temp);	

	applyColorMap(MODrisultanteSomma4Pesata,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOL_MODrisultanteSomma4Pesata.png", q);
	imwrite(name, temp);	

	applyColorMap(MODrisultanteSomma4,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOL_MODrisultanteSomma4.png", q);
	imwrite(name, temp);	

	applyColorMap(MODrisultanteScelti,temp,COLORMAP_JET);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sCOL_MODrisultanteSscelti.png", q);
	imwrite(name, temp);	




}
*/


	Mat immagine = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	Mat temp;
	normalize(immagine, immagine, 0, 255, NORM_MINMAX, CV_8U);
	applyColorMap(immagine,temp,COLORMAP_JET);

	imwrite("colorata.png", temp);
 	return 0;
}





