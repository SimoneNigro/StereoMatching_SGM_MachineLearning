#include <opencv/cv.h>      
#include <opencv/ml.h>		

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


int main( int argc, char** argv )
{

    printf ("OpenCV version %s (%d.%d.%d)\n",
            CV_VERSION,
            CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
    
//prende in ingresso un id numerico che identifica la coppia di immagini di KITTI con cui voglio lavorare

	if (argc != 2)
	{
		printf("Usage: %s id\n", argv[0]);
		return 0;
	}

        char name[500];

	int id = atoi(argv[1]);

//sistema l'id per evitare problemi con il numero di cifre
	char q[10];
	if(id < 10){sprintf(q, "00%d", id); }
	if(id >= 10 && id <= 99){sprintf(q, "0%d", id); }
	if(id >= 100){sprintf(q, "%d", id); }

//salto le immagini usate per il training, nel caso in cui abbia scelto proprio quegli id
	if(id == 43 || id == 71 ||id == 82 || id == 87 ||id == 94 || id == 120 || id == 122 ||id == 180){
	
	printf("Salto la %d perchè l'ho usata nel training\n", id);	
	return 0;
	}

//apro immagine sinistra, destra, e gt
	IplImage* LeftImage = NULL;	
	IplImage* RightImage = NULL;
	IplImage *GroundTruth = NULL;

	sprintf(name, "/home/simo/KITTI/training/image_0/000%s_10.png", q);
		
		LeftImage = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
	
	sprintf(name, "/home/simo/KITTI/training/image_1/000%s_10.png", q);

		RightImage = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);

	sprintf(name, "/home/simo/KITTI/training/disp_noc/000%s_10.png", q);

		GroundTruth = cvLoadImage(name,CV_LOAD_IMAGE_GRAYSCALE);
	
	int w= (*LeftImage).width;
	int h = (*LeftImage).height;

	int dMax = 127;

//creo il vettore con le 8 mappe di disparità ottenute dalle 8 diverse scanline
	IplImage* mappe[8];
//creo il vettore con le matrici di test, una per ciascuna mappa di ciascuna scanline
	Mat matrici_test[8];	
//creo il vettore con le varie DSI
	t_DSI* scanline_DSI[8];
//creo una matrice di vettori contenenti i risultati per ogni pixel per ciascuna mappa. In ciascun punto avrò 8 risultati
   	float** results = new float *[w*h];
	 for (size_t p = 0; p < w*h; p++) {
	      results[p] = new float[8];
	   }
	t_DSI* normale = create_DSI(w, h, dMax);	

//Creo le immagini dove salvare le mappe di disparità ottenute con i vari metodi 

	IplImage *risultanteScelti = cvCreateImage(cvGetSize(LeftImage),8,1);
	IplImage *risultanteSomma8Pesata = cvCreateImage(cvGetSize(LeftImage),8,1);
	IplImage *risultanteNormale = cvCreateImage(cvGetSize(LeftImage),8,1);
	IplImage *risultanteSomma4 = cvCreateImage(cvGetSize(LeftImage),8,1);
	IplImage *risultanteSomma4Pesata = cvCreateImage(cvGetSize(LeftImage),8,1);
	IplImage *risultanteMisto8 = cvCreateImage(cvGetSize(LeftImage),8,1);

//Creo ed alloco la memoria per le matrici dei minimi usate per ottenere le diverse mappe di disparità

	int** matriceMinimiScelti;
	int** matriceMinimiSomma8Pesata;
	int** matriceMinimiSomma4;
	int** matriceMinimiSomma4Pesata;
	int** matriceMinimiMisto8;
	int** matriceMinimiNormale;
int i;
	matriceMinimiScelti= (int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++)
	{
		matriceMinimiScelti[i]=(int*)calloc(w,sizeof(int));
	}

	matriceMinimiSomma8Pesata=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++)
	{
		matriceMinimiSomma8Pesata[i]=(int*)calloc(w,sizeof(int));
	}
	matriceMinimiNormale=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++)
	{
		matriceMinimiNormale[i]=(int*)calloc(w,sizeof(int));
	}

	matriceMinimiSomma4=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++)
	{
		matriceMinimiSomma4[i]=(int*)calloc(w,sizeof(int));
	}

	matriceMinimiSomma4Pesata=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++)
	{
		matriceMinimiSomma4Pesata[i]=(int*)calloc(w,sizeof(int));
	}
	
	matriceMinimiMisto8=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++)
	{
		matriceMinimiMisto8[i]=(int*)calloc(w,sizeof(int));
	}

//-----------------------------------------------------------------------//

//creo il vettore con le matrici per il testing e le immagini nel vettore di mappe di disparità

	for (int i = 0; i < 8 ; i++){

   		matrici_test[i] = Mat(w*h, 12, CV_32FC1); // w*h righe, una per ogni pixel, e 12 colonne, una per attributo. Praticamente rappresenta un file di test
		mappe[i] = cvCreateImage(cvGetSize(LeftImage),8,1); //8 mappe di disparità
	}
	

//prende le 2 immagini, genera le 8 mappe e le 8 matrici corrispondenti con i dati di test con un procedimento analogo a quanto fatto per generare il file di train
//calcolando dev standard, mediana, MAD su 4 finestre di dimensioni diverse
//"normale" è la DSI che conterrà il risultato dell'algoritmo SGM normale, viene riempita da prepara_test
    prepara_test(normale, LeftImage, RightImage, mappe, matrici_test, scanline_DSI, NULL, NULL ); //pesi e medie a null per fargli fare la normale
//la funzione è definita nel file genera_8_mappe.cpp

//SALVO LE MAPPE PER DEBUG
	/*for (int s = 0; s < 8 ; s++){
	
	char nome[20];
	sprintf(nome, "Disp_%d.png", s);

   	cvSaveImage(nome, mappe[s]);

	}*/

//Creo l'oggetto foresta 
        CvRTrees* rtree = new CvRTrees;
//Carico la foresta già addestrata in precedenza	
	rtree->load("/home/simo/Dropbox/TESI/GENERA FORESTA/foresta.xml");

	//legge l'importanza data a ciascun attributo
	Mat var_importance = rtree->get_var_importance();
	cout << "Importanza = " << endl << " " << var_importance << endl << endl;

        Mat test_sample;

//faccio il test su ognuna delle 8 matrici di test.
//ciclo da 0 a w*h per analizzare ogni riga e, quindi, ogni pixel
//poi, all'interno del primo ciclo, ciclo da 0 ad 8 per testare i risultati prodotti da ciascuna scanline
	
	int z; int tsample; int count_non_validi = 0;
printf("Inizio i test...\n");

	for (tsample = 0; tsample < w*h; tsample++)
	{
		for (z = 0; z < 8; z++){
		    
		    //prendo una riga, quindi gli attributi in quel punto, e li passo alla random forest, poi salvo il risultato
		    //nel vettore result che ha tante righe quanti pixel delle immagini e 8 colonne, una per i risultati di ciascuna scanline
		    test_sample = matrici_test[z].row(tsample);

		    results[tsample][z] = rtree->predict(test_sample, Mat());
		}
	
	}

//Creo una DSI per ciascuna metodologia che voglio provare
	t_DSI* sceltiDSI = create_DSI(w, h, dMax);	
	t_DSI* somma4DSI = create_DSI(w, h, dMax);	
	t_DSI* sommaPesata8DSI = create_DSI(w, h, dMax);	
	t_DSI* sommaPesata4DSI = create_DSI(w, h, dMax);	
	t_DSI* misto8DSI = create_DSI(w, h, dMax);	

	float *pesi = new float[8]; 
	float totale_pesi;
	
	//ciclo sui risultati e compongo le dsi
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
		
			//i pesi per le somme pesate altro non sono che il risultato restituito dalla foresta in quel punto per le 8 scanline
			pesi[0] = results[i*w+j][0];
			pesi[1] = results[i*w+j][1];
			pesi[2] = results[i*w+j][2];
			pesi[3] = results[i*w+j][3];
			pesi[4] = results[i*w+j][4];
			pesi[5] = results[i*w+j][5];
			pesi[6] = results[i*w+j][6];
			pesi[7] = results[i*w+j][7];
			totale_pesi = pesi[0]+pesi[1]+pesi[2]+pesi[3]+pesi[4]+pesi[5]+pesi[6]+pesi[7];
		
			//cerco la scanline con il peso maggiore, quindi quella ritenuta migliore
			int max_index = 0;
				int index1 = 0, index2=1, index3=2, index4=3;

				for(int z = 0; z < 8; z++){
				
					if (results[i*w+j][z] > results[i*w+j][max_index])
					{
						max_index = z; 
					}
				}


			//qui salvo in index1, index2, index3, index4 gli indici delle 4 scanline migliori
			for(int v = 0; v < 8; v++){
					
				if(pesi[v] >= pesi[index1]){ index2=index1; index3=index2; index4=index3; index1=v; continue;}
				if(pesi[v] >= pesi[index2]){ index3=index2; index4=index3;index2=v;  continue;}
				if(pesi[v] >= pesi[index3]){ index4=index3; index3=v; continue;}
				if(pesi[v] >= pesi[index4]){ index4=v; continue;}

			}

//INIZIO A COMPORRE LE DSI NEI DIVERSI MODI

			//qui prendo i valori dalla DSI del punto migliore
			int d;
			for (d = 0; d < dMax+1; d++)
			{
				int *v;
				v = (sceltiDSI->values+ d*sceltiDSI->width*sceltiDSI->height +i*sceltiDSI->width +j); 
				*v = DSI_ELEMENT(scanline_DSI[max_index],j,i,d); 
			}
			

			//qui somma pesata dei valori di tutte le DSI in quel punto
			for (int d = 0; d < dMax+1; d++)
			{

				int valore0 = DSI_ELEMENT(scanline_DSI[0],j,i,d);
				int valore1 = DSI_ELEMENT(scanline_DSI[1],j,i,d);
				int valore2 = DSI_ELEMENT(scanline_DSI[2],j,i,d);
				int valore3 = DSI_ELEMENT(scanline_DSI[3],j,i,d);
				int valore4 = DSI_ELEMENT(scanline_DSI[4],j,i,d);
				int valore5 = DSI_ELEMENT(scanline_DSI[5],j,i,d);
				int valore6 = DSI_ELEMENT(scanline_DSI[6],j,i,d);
				int valore7 = DSI_ELEMENT(scanline_DSI[7],j,i,d);
				int *v;
				v = (sommaPesata8DSI->values+ d*sommaPesata8DSI->width*sommaPesata8DSI->height +i*sommaPesata8DSI->width +j); 
				*v = (valore0*pesi[0] + valore1*pesi[1] + valore2*pesi[2] + valore3*pesi[3] +
					valore4*pesi[4] + valore5*pesi[5] + valore6*pesi[6] + valore7*pesi[7]);
			   
			}

			//qui sommo i valori delle 4 migliori DSI in quel punto
			for (d = 0; d < dMax+1; d++)
			{	
				int *v;
				v = (somma4DSI->values+ d*somma4DSI->width*somma4DSI->height +i*somma4DSI->width +j); 
				*v = DSI_ELEMENT(scanline_DSI[index1],j,i,d) + DSI_ELEMENT(scanline_DSI[index2],j,i,d)
					+ DSI_ELEMENT(scanline_DSI[index3],j,i,d) + DSI_ELEMENT(scanline_DSI[index4],j,i,d);
			}

			//qui somma pesata dei valori delle 4 migliori DSI in quel punto
			for (d = 0; d < dMax+1; d++)
			{	
				int valore1 = DSI_ELEMENT(scanline_DSI[index1],j,i,d);
				int valore2 = DSI_ELEMENT(scanline_DSI[index2],j,i,d);
				int valore3 = DSI_ELEMENT(scanline_DSI[index3],j,i,d);
				int valore4 = DSI_ELEMENT(scanline_DSI[index4],j,i,d);
				int *v;
				v = (sommaPesata4DSI->values+ d*sommaPesata4DSI->width*sommaPesata4DSI->height +i*sommaPesata4DSI->width +j); 
				*v = (valore1*pesi[index1] + valore2*pesi[index2] + valore3*pesi[index3] +valore4*pesi[index4]);
			}

		}
	}

//popolo le matrici dei minimi cercando in ogni DSI la disparità minore in ciascun punto
printf("Ora chiamo trovaDisparita\n");

	trovaDisparitaL(sceltiDSI->values,w,h,dMax,matriceMinimiScelti);
        trovaDisparitaL(sommaPesata8DSI->values,w,h,dMax,matriceMinimiSomma8Pesata);
        trovaDisparitaL(normale->values,w,h,dMax,matriceMinimiNormale);
	trovaDisparitaL(somma4DSI->values,w,h,dMax,matriceMinimiSomma4);
	trovaDisparitaL(sommaPesata4DSI->values,w,h,dMax,matriceMinimiSomma4Pesata);

//Genero le immagini vere e proprie a partire dalle matrici dei minimi
printf("Sono dopo trovadisparita, inizio il ciclo per assegnare alle iplimage i minimi\n");

	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			
			CV_IMAGE_ELEM(risultanteScelti,uchar,i,j) = matriceMinimiScelti[i][j]; 
			CV_IMAGE_ELEM(risultanteSomma8Pesata,uchar,i,j) = matriceMinimiSomma8Pesata[i][j];
			CV_IMAGE_ELEM(risultanteNormale,uchar,i,j) = matriceMinimiNormale[i][j];
			CV_IMAGE_ELEM(risultanteSomma4,uchar,i,j) = matriceMinimiSomma4[i][j];
			CV_IMAGE_ELEM(risultanteSomma4Pesata,uchar,i,j) = matriceMinimiSomma4Pesata[i][j];
		}
	}

//Salvo le immagini
printf("Ciclo finito, provo a salvare le immagini\n");
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteScelti.png", q);
	cvSaveImage(name, risultanteScelti);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultante.png", q);
	cvSaveImage(name, risultanteSomma8Pesata);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteNormale.png", q);
	cvSaveImage(name, risultanteNormale);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteSomma4.png", q);
	cvSaveImage(name, risultanteSomma4);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%srisultanteSomma4Pesata.png", q);
	cvSaveImage(name, risultanteSomma4Pesata);
	sprintf(name, "/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/%sconfidenza.png", q);

        return 0;

    return -1;
}
/******************************************************************************/

