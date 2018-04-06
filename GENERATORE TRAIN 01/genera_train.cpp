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
#include <omp.h>

#define BYTE unsigned char

using namespace std;
using namespace cv;

#define RADIUS 4

//prende una mappa di disparità, la ground truth, calcola tutte le varie features, il risultato, e genera il csv
void crea_csv(IplImage *DispMap, IplImage *GroundTruth){

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


//creo il file vero e proprio
    FILE* f = fopen( "MioFilePuntoTrain.train", "a" ); //apre in append se esiste, sennò lo crea 
    if( !f )
    {
        printf("ERROR: cannot create file\n");
        exit(0); // all not OK
    }

//ciclo su tutti i punti delle immagini/Mat, che ovviamente hanno tutte le stesse dimensioni
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
//se il pixel in esame della ground truth non ha intensita 0 lo considero, altrimenti:
			if (CV_IMAGE_ELEM(GroundTruth, uchar, i, j) != 0) 
			{	
				float result = 0.0;
				
//se l'intensità del pixel della mappa di disparità è uguale a quella del pixel corrispondente della GT entro una soglia 3, il risultato è 1, 0 altrimenti
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(DispMap, uchar, i, j)) <= 3  )
				{	
					result = 1.0;				
				}

//per ciascun punto, salvo una riga nel file. Ciascuna riga sarà l'elenco dei 12 attributi calcolati in quel punto separati da virgole, più il risultato 0 o 1
				fprintf(f, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
				dev_std_5.at<float>(i,j),dev_std_7.at<float>(i,j),dev_std_9.at<float>(i,j),dev_std_11.at<float>(i,j), 							mediana_5.at<float>(i,j), mediana_7.at<float>(i,j), mediana_9.at<float>(i,j), mediana_11.at<float>(i,j), 								MAD_5.at<float>(i,j),  MAD_7.at<float>(i,j), MAD_9.at<float>(i,j), MAD_11.at<float>(i,j),result);

			}	
					
		}
	} 
	printf("File Scritto\n");

//libero memoria
     image32f.release();
    
     mu_5.release();
     mu2_5.release();
     dev_std_5.release();
     mediana_5.release();
     MAD_5.release();
     diff_5.release();
    
     mu_7.release();
     mu2_7.release();
     dev_std_7.release();
     mediana_7.release();
     MAD_7.release();
     diff_7.release();    

     mu_9.release();
     mu2_9.release();
     dev_std_9.release();
     mediana_9.release();
     MAD_9.release();
     diff_9.release();
    
     mu_11.release();
     mu2_11.release();
     dev_std_11.release();
     mediana_11.release();
     MAD_11.release();
     diff_11.release();

	fclose(f);

}

int main(int argc, char **argv){

//dMin e dMax delle DSI
	int dMin=0;
	int dMax=127;

	int res,i,j,d=0,z;
	int scale_factor=16;

//matrici dei minimi per SGM
	int **matriceMinimiIndexL;

	char nome[20];

//vettore di 8 DSI, una per ciascuna delle 8 direzioni usate da SGM
	t_DSI* scanline_DSI[8];

	IplImage *OutputImageL=NULL;
	IplImage *OutputImageR=NULL;
	IplImage *KITTI_Output_Image_L= NULL;
	IplImage *KITTI_Output_Image_R= NULL;

//dal momento che tutte le immagini di KITTI sono in un path fisso ed i nomi hanno lo stesso formato, prendo in ingresso un numero id che indica con quale coppia 
//di KITTI lavorerò. In questo modo per generare un train da più immagini basta lanciare un for da terminale e via

	if (argc != 2)
	{
		printf("Usage: %s id\n", argv[0]);
		return 0;
	}
	
	IplImage *KITTI_left;
	IplImage *KITTI_right;
	IplImage *GroundTruth;

	char name[500]; char* q = (argv[1]);

//Comando utile in caso lanciate il programma con un ciclo for nel terminale
printf("Sono alla %s\n", q);

//Apro immagine sinistra, destra, e Ground Truth
	sprintf(name, "/home/simo/KITTI/training/image_0/000%s_10.png", q);
		KITTI_left = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
	sprintf(name, "/home/simo/KITTI/training/image_1/000%s_10.png", q);
		KITTI_right = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
	sprintf(name, "/home/simo/KITTI/training/disp_noc/000%s_10.png", q);
		GroundTruth = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);

	int w=KITTI_left->width;
	int h=KITTI_left->height;


//Creo un'immagine dove salvare di volta in volta la mappa di disparità ottenuta da una delle 8 scanline

	KITTI_Output_Image_L=cvCreateImage(cvGetSize(KITTI_left),8,1);

	printf("Matrix init\n");

//Alloco memoria per la matrice dei minimi
	matriceMinimiIndexL=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++)
	{
		matriceMinimiIndexL[i]=(int*)calloc(w,sizeof(int));
	}

//Creo una DSI 
	t_DSI* DSI=create_DSI(KITTI_left->width, KITTI_right->height, dMax);


//L'ULTIMA CIFRA DEL COMPUTE E' UNA MASCHERA PER DECIDERE QUANTE SCANLINE FARE 255 = TUTTE E 8
//La funzione compute si trova nel file SGM.c
	printf("Starting SGM...\n");
	compute(KITTI_left,KITTI_right, dMax, 30, 300, DSI, 255, scanline_DSI);
//scanline_DSI è un vettore di DSI, ne contiene una per scanline, al termine dell'esecuzione di compute avrò in scanline_DSI le 8 DSI relative alle 8 scanline
	printf("SGM complete\n");	

//ciclo sulle 8 DSI contenute in scanline_DSI, per ciascuna genero la mappa di disparità vera e propria per poi invocare la funzione crea_csv che mi generererà il file
	for (i = 0; i<8; i++){ 
		
		matriceMinimiIndexL=trovaDisparitaL(scanline_DSI[i]->values,w,h,dMax);
		
		for(z=0;z<h;z++)
		{
			for(j=0;j<w;j++)
			{		
				CV_IMAGE_ELEM(KITTI_Output_Image_L,uchar,z,j) = matriceMinimiIndexL[z][j];
			}
		}

	crea_csv(KITTI_Output_Image_L, GroundTruth); 
	
	}

}
