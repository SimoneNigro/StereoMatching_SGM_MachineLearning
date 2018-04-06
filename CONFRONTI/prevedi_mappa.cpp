#include <opencv/cv.h>      
#include <opencv/ml.h>		

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <omp.h>
#include "confronta.h"

using namespace cv;
using namespace std;

/******************************************************************************/

//Preso l'id apre le mappe di disparità ottenute con sgm e con i vari metodi provati, poi chiama la funzione
//confronta che calcola gli errori rispetto alla gt a varie soglie e compila un csv 

int main( int argc, char** argv )
{
    // lets just check the version first

    printf ("OpenCV version %s (%d.%d.%d)\n",
            CV_VERSION,
            CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
    
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

	//salto le immagini usate per il training
	if(id == 43 || id == 71 ||id == 82 || id == 87 ||id == 94 || id == 120 || id == 122 ||id == 180){
	
	printf("Salto la %d perchè l'ho usata nel training\n", id);	
	return 0;
	}

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
	

//faccio i test

	confronta(risultanteSomma8Pesata,risultanteNormale, risultanteScelti, risultanteSomma4, risultanteSomma4Pesata, GroundTruth, q);

        return 0;

}
/******************************************************************************/

