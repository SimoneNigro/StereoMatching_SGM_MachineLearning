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

//prende una mappa, la matrice di test corrispondente, e genera gli attributi per poi salvarli nella matrice
void genera_test(IplImage *DispMap, Mat testing_data){

	int i, j;	

	int w=DispMap->width;
	int h=DispMap->height;

//CALCOLO MEDIANA, DEVIAZIONE STANDARD, E MAD USANDO I METODI DI OPENCV
//Innanzitutto lavoro con Mat e non con IplImage perchè alcune delle trasformazioni non accettano IplImage
//Converto quindi la mappa di disparità ricevuta in Mat, e creo tutte le Mat che mi servono

//Converto la mappa di disparità da IplImage a Mat
    Mat image = cv::cvarrToMat(DispMap);
    Mat image32f;
    
//Mat di "appoggio" per il calcolo della deviazione standard
    Mat mu_5;
//Mat di "appoggio" per il calcolo della deviazione standard
    Mat mu2_5;
//Mat che conterrà la deviazione standard
    Mat dev_std_5;
//Mat che conterrà la mediana
    Mat mediana_5;
//Mat che conterrà la deviazione mediana assoluta
    Mat MAD_5;
//Mat di appoggio per il calcolo della MAD
    Mat diff_5;
    
//Visto che devo calcolare tutto su finestre 5x5 7x7 9x9 e 11x11, creo le varie Mat anche per le altre finestre

    Mat mu_7;
    Mat mu2_7;
    Mat dev_std_7;
    Mat mediana_7;
    Mat MAD_7;
    Mat diff_7;    

    Mat mu_9;
    Mat mu2_9;
    Mat dev_std_9;
    Mat mediana_9;
    Mat MAD_9;
    Mat diff_9;
    
    Mat mu_11;
    Mat mu2_11;
    Mat dev_std_11;
    Mat mediana_11;
    Mat MAD_11;
    Mat diff_11;

//Conversione necessaria per usare i metodi di opencv, converte la mappa da 8bit unsigned a 32 ad un canale
    image.convertTo(image32f, CV_32FC1);

//La funzione blur di opencv calcola la media delle intensità in una finestra di dimensione Size(x,y), 
//la sfrutto quindi per calcolare la dev standard come visto qui 
// https://stackoverflow.com/questions/11456565/opencv-mean-sd-filter

    blur(image32f, mu_5, Size(5, 5)); 
    blur(image32f.mul(image32f), mu2_5, Size(5, 5));
    sqrt(mu2_5 - mu_5.mul(mu_5), dev_std_5); 

//la funzione medianBlur sostituisce ad ogni punto la mediana delle intensità calcolata nella finestra di dimensioni scelte che circonda il punto stesso
//nella Mat mediana_5 mi troverò quindi tutte le mediane
    medianBlur(image, mediana_5, 5);

//la deviazione mediana assoluta MAD si ottiene calcolando la mediana della differenza in valore assoluto tra un valore e la mediana "in quel punto"
    absdiff(image, mediana_5, diff_5);
    medianBlur(diff_5, MAD_5, 5);//qui ho la MAD

//avevo salvato le immagini per vederle e controllare che tutto avesse senso, ma non servono, era solo un mio controllo
//imwrite("disp_map.png", image32f);
//imwrite("dev_std_5.png", dev_std_5);
//imwrite("mediana_5.png", mediana_5);
//imwrite("MAD_5.png", MAD_5);

//ripeto il tutto per le diverse finestre

    blur(image32f, mu_7, Size(7, 7));
    blur(image32f.mul(image32f), mu2_7, Size(7, 7));
    sqrt(mu2_7 - mu_7.mul(mu_7), dev_std_7);
   // dev_std_7.convertTo(dev_std_7, CV_8U); //qui ho la deviaz standard
    medianBlur(image, mediana_7, 7);//qui ho la mediana
    absdiff(image, mediana_7, diff_7);
    medianBlur(diff_7, MAD_7, 7 );//qui ho la MAD


    blur(image32f, mu_9, Size(9, 9));
    blur(image32f.mul(image32f), mu2_9, Size(9, 9));
    sqrt(mu2_9 - mu_9.mul(mu_9), dev_std_9);
   // dev_std_9.convertTo(dev_std_9, CV_8U); //qui ho la deviaz standard
    medianBlur(image, mediana_9, 9);//qui ho la mediana
    absdiff(image, mediana_9, diff_9);
    medianBlur(diff_9, MAD_9, 9 );//qui ho la MAD


    blur(image32f, mu_11, Size(11, 11));
    blur(image32f.mul(image32f), mu2_11, Size(11, 11));
    sqrt(mu2_11 - mu_11.mul(mu_11), dev_std_11);
    //dev_std_11.convertTo(dev_std_11, CV_8U); //qui ho la deviaz standard
    medianBlur(image, mediana_11, 11);//qui ho la mediana
    absdiff(image, mediana_11, diff_11);
    medianBlur(diff_11, MAD_11, 11 );//qui ho la MAD

   //TEST PROVO A CONVERTIRE MEDIANA E MAD IN 32F così sono sicuro che siano float con un senso
   mediana_5.convertTo(mediana_5, CV_32FC1);
   mediana_7.convertTo(mediana_7, CV_32FC1);
   mediana_9.convertTo(mediana_9, CV_32FC1);
   mediana_11.convertTo(mediana_11, CV_32FC1);

   MAD_5.convertTo(MAD_5, CV_32FC1);
   MAD_7.convertTo(MAD_7, CV_32FC1);
   MAD_9.convertTo(MAD_9, CV_32FC1);
   MAD_11.convertTo(MAD_11, CV_32FC1);


//la matrice ha una riga per ciascun pixel, ed una colonna per ciascun attributo. Salvo in ogni riga i 12 attributi in quel punto
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
				testing_data.at<float>(i*w + j, 0) =  dev_std_5.at<float>(i,j);
				testing_data.at<float>(i*w + j, 1) = dev_std_7.at<float>(i,j);
				testing_data.at<float>(i*w + j, 2) = dev_std_9.at<float>(i,j);
				testing_data.at<float>(i*w + j, 3) = dev_std_11.at<float>(i,j);

				testing_data.at<float>(i*w + j, 4) = mediana_5.at<float>(i,j);
				testing_data.at<float>(i*w + j, 5) = mediana_7.at<float>(i,j);
				testing_data.at<float>(i*w + j, 6) = mediana_9.at<float>(i,j);
				testing_data.at<float>(i*w + j, 7) = mediana_11.at<float>(i,j);

				testing_data.at<float>(i*w + j, 8) = MAD_5.at<float>(i,j);
				testing_data.at<float>(i*w + j, 9) = MAD_7.at<float>(i,j);
				testing_data.at<float>(i*w + j, 10) = MAD_9.at<float>(i,j);
				testing_data.at<float>(i*w + j, 11) = MAD_11.at<float>(i,j);
							
		}
	}


}

//riceve immagine destra e sinistra, calcola 8 mappa di disparità, chiama la funzione genera_test su ciascuna di esse, e salva le DSI corrispondenti a ciascuna scanline
void prepara_test (t_DSI *normale, IplImage *LeftImage, IplImage *RightImage, IplImage** mappe, Mat *matrici_test, t_DSI** scanline_DSI, float **pesi, float **medie){

	int **matriceMinimiIndexL;
	int **matriceMinimiIndexR;

	int dMin=0;
	int dMax=127;

	IplImage *Output_Image_L= NULL;

	int w = LeftImage->width;
	int h = LeftImage->height;

	Output_Image_L=cvCreateImage(cvGetSize(LeftImage),8,1);

	printf("Matrix init\n");

	matriceMinimiIndexL=(int**)calloc(h,sizeof(int*));
	for(int i=0;i<h;i++)
	{
		matriceMinimiIndexL[i]=(int*)calloc(w,sizeof(int));
	}


	//L'ULTIMA CIFRA DEL COMPUTE E' UNA MASCHERA PER DECIDERE QUANTE SCANLINE FARE 255 = TUTTE E 8
	printf("Starting SGM...\n");
	compute(LeftImage,RightImage, dMax, 30, 300, normale, 255, scanline_DSI, pesi, medie); 
	//scanline_DSI è un vettore di DSI, ne contiene una per scanline
	printf("SGM complete\n");		

	for (int k = 0; k < 8; k++){ //ciclo sulle scanline
		
		trovaDisparitaL(scanline_DSI[k]->values,w,h,dMax,matriceMinimiIndexL);
		
		for(int z=0;z<h;z++){
			for(int j=0;j<w;j++){

				CV_IMAGE_ELEM(mappe[k],uchar,z,j) = matriceMinimiIndexL[z][j];		
			}
		}
	
	genera_test(mappe[k], matrici_test[k]);
	
	}




} 
