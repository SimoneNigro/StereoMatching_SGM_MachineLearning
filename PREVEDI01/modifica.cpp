#include <opencv/cv.h>       // opencv general include file
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std; // OpenCV API is in the C++ "cv" namespace
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
/******************************************************************************/

void confronta(IplImage* DispMap, IplImage* Normale, IplImage* GroundTruth)
{
    
    int w = DispMap->width;
    int h = DispMap->height;

    	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			//se il pixel della ground truth ha intensita 0, rendo 0 anche quella delle mappe
			if (CV_IMAGE_ELEM(GroundTruth, uchar, i, j) == 0) 
			{	
				CV_IMAGE_ELEM(DispMap, uchar, i, j) = 0;
				CV_IMAGE_ELEM(Normale, uchar, i, j) = 0;
			}	
					
		}
	}
 
}

int main( int argc, char** argv )
{

	if (argc != 2)
	{
		printf("Usage: %s id\n", argv[0]);
		return 0;
	}

        char name[500];

	int id = atoi(argv[1]);

	char q[10];
	if(id < 10){sprintf(q, "00%d", id); }
	if(id >= 10 && id <= 99){sprintf(q, "0%d", id); }
	if(id >= 100){sprintf(q, "%d", id); }

	IplImage *risultanteSomma8Pesata = NULL;
	IplImage *risultanteNormale = NULL;
	IplImage *risultanteSomma4 = NULL;
	IplImage *risultanteScelti = NULL;
	IplImage *risultanteSomma4Pesata = NULL;
	IplImage *GroundTruth = NULL;

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteNormale.png", q);
		
		risultanteNormale = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
	
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultante.png", q);

		risultanteSomma8Pesata = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteSomma4Pesata.png", q);

		risultanteSomma4Pesata = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteScelti.png", q);

		risultanteScelti = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteSomma4.png", q);

		risultanteSomma4 = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);

	sprintf(name, "/home/simo/KITTI/training/disp_noc/000%s_10.png", q);

		GroundTruth = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);

	confronta(risultanteSomma8Pesata, risultanteNormale, GroundTruth); //basta passarne 2 alla volta assieme alla GT
	confronta(risultanteSomma4Pesata, risultanteSomma4, GroundTruth);
	confronta(risultanteSomma4Pesata, risultanteScelti, GroundTruth);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultante.png", q);
	cvSaveImage(name, risultanteSomma8Pesata);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteNormale.png", q);
	cvSaveImage(name, risultanteNormale);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sGT.png", q);	
	cvSaveImage(name, GroundTruth);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteSomma4.png", q);
	cvSaveImage(name, risultanteSomma4);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteSomma4Pesata.png", q);
	cvSaveImage(name, risultanteSomma4Pesata);

	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sMODrisultanteScelti.png", q);
	cvSaveImage(name, risultanteScelti);
}
