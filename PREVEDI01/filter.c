#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "minmax.h"
#include <float.h>
#include "filter.h"
#include <fstream>

#ifndef INT_MAX
#define INT_MAX 1000000
#endif

#ifndef INT_MIN
#define INT_MIN -1000000
#endif


#ifndef BYTE 
// #define BYTE unsigned char
typedef unsigned char BYTE;
#endif

using namespace std;

//#define MANTENGO_VALORI_COSTO_UGUALE 
int UC(int **minCostMatrixL,int **disparity_left,int w, int h,int scale_factor,int **UCmatrix,int **UCmatrixBW,int **UCcard, char *filename){

	int i,j,k,old_index=0,x=0,y=0;
	int cost_value;
	int *disparity,*cost,*cardinality;
	int **index;
	int index_right=0;

	BYTE disparity_l;
	
	
	ofstream out(filename);

	disparity = (int*)malloc(w*sizeof(int));
	cost = (int*)malloc(w*sizeof(int));
	cardinality = (int*)malloc(w*sizeof(int));
	
	// matrice per tenere traccia degli indici dei punti con stesso costo
	index = (int**)malloc(w*sizeof(int*));
	for(x=0;x<w;x++){

		index[x] = (int*)malloc(w*sizeof(int));
	}


		for(i=0;i<h;i++)
		{
			
			for(k=0;k<w;k++)
			{
				cost[k] = INT_MAX;
				disparity[k] = -1;
				cardinality[k] = 0;
				
				for(y=0;y<w;y++){
					index[k][y] = -1;
				}
			}


			
			for(j=0;j<w;j++)
			{
				

				disparity_l = disparity_left[i][j];
				cost_value = minCostMatrixL[i][j];
				
				
				//check per non andare fuori dall'immagine
				if(j-disparity_l >= 0)
				{
					
					if(disparity[j-disparity_l] == -1)
					{
						//inizializzo i vettori con i valori reali
						
						index_right = j-disparity_l;
						
						disparity[index_right] = disparity_l;
						
						cost[index_right] = cost_value;
						
						cardinality[index_right]++;
					
						//inizializzo ogni nuovo punto con la mappa delle disparità
						UCmatrix[i][index_right + disparity_l] = disparity_l*scale_factor;
						UCmatrixBW[i][index_right + disparity_l] = 255;

					}
					else 
					{
						cardinality[j-disparity_l]++;
						index_right = j-disparity_l;

						// check costo minore di quello già presente nel vettore
						if(cost_value<cost[j-disparity_l])
						{
							//se matrice per quel punto è vuota procedura standard

#ifdef MANTENGO_VALORI_COSTO_UGUALE							

							if(index[index_right][0] == -1)
							{

								//risalgo al punto che in precedenza era il migliore trovato fino a quel momento
								old_index = index_right + disparity[index_right];

								//sostituisco i nuovi valori nei vettori
								cost[index_right] = cost_value;
								disparity[index_right] = disparity_l;
						
								// metto il bianco al vecchio e la disparità al nuovo
								UCmatrix[i][old_index] = 255;
								UCmatrix[i][index_right + disparity_l] = disparity_l*scale_factor;
						
								UCmatrixBW[i][old_index] = 0;
								UCmatrixBW[i][index_right + disparity_l] = 255;
							}
							
							// se matrice contiene almeno un caso in cui il costo è stato lo stesso
							else
							{
							
								//caso in cui costo_nuovo < costo_vecchio 
								//
								//per ogni indice salvato, metto il punto bianco
								//
								for(x=0;x<w;x++){

									old_index= index[index_right][x];
									UCmatrix[i][old_index] = 255;
									UCmatrixBW[i][old_index] = 0;
								}

								//salvo i nuovi valori
								cost[index_right] = cost_value;
								disparity[index_right] = disparity_l;

								// metto il valore della mappa nel nuovo punto
								UCmatrix[i][index_right + disparity_l] = disparity_l*scale_factor;
								UCmatrixBW[i][index_right + disparity_l] = 255;

								//svuto la colonna della matrice per quell'indice
								
								index[index_right][0] = index_right+disparity_l;
								
								for(x=1;x<w;x++){

									index[index_right][x] = -1;
								}


							}

								
						}

						// se ho due punti con costo uguale
						if(cost_value == cost[index_right])
						{
							//salvo nella colonna della matrice all'indice index_right tutti i punti 
							index[index_right][y]= index_right + disparity_l;

							//assegnando ad ogni punto la mappa di disparità anzivhè un punto bianco
							UCmatrix[i][index_right + disparity_l] = disparity_l*scale_factor;
							UCmatrixBW[i][index_right + disparity_l] = 255;
							y++;

						}

						else
						{
							// costo maggiore, metto il punto bianco
						
							UCmatrix[i][index_right + disparity_l] = 255;
							UCmatrixBW[i][index_right + disparity_l] = 0;
						}
					}
					
#endif

#ifndef MANTENGO_VALORI_COSTO_UGUALE

							

								//risalgo al punto che in precedenza era il migliore trovato fino a quel momento
								old_index = index_right + disparity[index_right];

								//sostituisco i nuovi valori nei vettori
								cost[index_right] = cost_value;
								disparity[index_right] = disparity_l;
						
								// metto il bianco al vecchio e la disparità al nuovo
								UCmatrix[i][old_index] = 255;
								UCmatrix[i][index_right + disparity_l] = disparity_l*scale_factor;
						
								UCmatrixBW[i][old_index] = 0;
								UCmatrixBW[i][index_right + disparity_l] = 255;
							
						
						}

						else
						{
						
						
							UCmatrix[i][index_right + disparity_l] = 255;
							UCmatrixBW[i][index_right + disparity_l] = 0;
						}
					}

				
#endif

					
				}
				//se sono fuori dal'immagine, punto bianco
				else
				{
					UCmatrix[i][j] = disparity_l*scale_factor;

					UCmatrixBW[i][j] = 0;
				}


			}
				
			for(j=0;j<w;j++)
			{
				UCcard[i][j]= cardinality[j];
				out << i << " " << j << " " << UCcard[i][j] << endl;
			}
		}
	out.close();
	return 0;
}

int LC(int *vettore,int **minCostMatrixL,int w, int h, int dMax,int **LCmatrix, char *filename)
{
	int i,j,d;
	int index,index_left,index_right;
	double value;
	double gamma= 240;
	double min=INT_MAX, max=0;

	double **LCdouble = (double**)calloc(h, sizeof(double));
	for (i = 0; i < h; i++) LCdouble[i] = (double*)calloc(w, sizeof(double));

	ofstream out(filename);

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			for(d=0;d<dMax;d++)
			{
				index = w*h*d+i*w+j;
				

				if(vettore[index] == minCostMatrixL[i][j])
				{
					if(d>0)
					{
						if(d<dMax-1)
						{
							index_left = w*h*(d-1)+i*w+j;
							index_right = w*h*(d+1)+i*w+j;

							if(vettore[index_left] >= vettore[index_right])
							{
								value= ((double)vettore[index_left]- (double)minCostMatrixL[i][j])/gamma;
								LCdouble[i][j]=value;
							}
							else
							{
								value= ((double)vettore[index_right]-(double)minCostMatrixL[i][j])/gamma;
								LCdouble[i][j]=value;
							}

						}

					}

					if(d>0)
					{
						if(d>dMax-1)
						{
							index_left = w*h*(d-1)+i*w+j;
							value=( (double)vettore[index_left]-(double)vettore[index])/gamma;
							LCdouble[i][j]=value ;
						}

					}
					
					if(d == 0)
					{
						
							index_right = w*h*(d+1)+i*w+j;
							value=( (double)vettore[index_right] - (double)vettore[index])/gamma;
							LCdouble[i][j]= value;
					

					}


					
				}
				

			}

		}

	}
	
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			if(LCdouble[i][j]< min)
			{
				min=LCdouble[i][j];
			}
			if(LCdouble[i][j]> max)
				max=LCdouble[i][j];
			{
			}
			out << i << " " << j << " " << max-LCdouble[i][j] << endl;

		}
	}
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
		
			LCmatrix[i][j] = 255.0-(((LCdouble[i][j]-min)/(max-min))*255.0);
			//printf("%f %d\n", LCdouble[i][j], LCmatrix[i][j]);
		}
	}
	printf("MIN %f MAX %f\n", min, max);
	out.close();
	return 0;
}

int NEM(double **costSumNEM,int *vettore, int w, int h , int dMax, double **NEMmatrix, char *filename){


	int i,j,d,index_vettore,index_auxiliar,v;

	double max,vec,res,value,num,min;

	double *auxiliar;

	ofstream out(filename);	

	auxiliar=(double*)calloc(w*h*dMax + h*w + w,sizeof(double));

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){

			for(d=0;d<dMax;d++)
			{

				index_vettore=w*h*dMax+i*w+j; 
				v=vettore[index_vettore];
				value=v*0.01;
				index_auxiliar=w*h*dMax+i*w+j; 
				num=(exp( - value));
				res= num/costSumNEM[i][j];
				auxiliar[index_auxiliar]=res;
				
			}
		}
	}

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){

			for(d=0;d<dMax;d++)
			{

				index_auxiliar=w*h*dMax+i*w+j;
				
				vec=auxiliar[index_auxiliar];

				value =+ (-1)*(vec*log10(vec)); 
				
			}
			NEMmatrix[i][j]=value;
		}
	}


	max=0;	
	min=INT_MAX;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			
			if(NEMmatrix[i][j]>max)
			{
				max=NEMmatrix[i][j];
			}
			if(NEMmatrix[i][j]<min)
			{
				min=(NEMmatrix[i][j]);
			}
			out << i << " " << j << " " << NEMmatrix[i][j] << endl;
		}
	}

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			
			value= (((NEMmatrix[i][j]-min)/(max-min))*255);
			NEMmatrix[i][j]= value;
		}
	}

	out.close();
	return 0;
}

int MLM(int **minCostMatrixL,double **mlmMatrix,int *vettore,int w, int h, int dMax, int **MLMmatrix, char *filename){

	int i,j;
	long double num,res=0,max=0,value,min=LDBL_MAX_10_EXP,v;

	
	ofstream out(filename);
	
	for (i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value=((minCostMatrixL[i][j])/(2*0.3*0.3))*0.0001;
			num=(exp(- value));
			res=(num/mlmMatrix[i][j]);
			

			if(res>max){
				max=res;
			}
			if(res<min){
				
				min=res;
			}
			out << i << " " << j << " " << max-res << endl;
		}
	}


	for (i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value=((minCostMatrixL[i][j])/(2*0.3*0.3))*0.0001;
			num=(exp(- value));
			res=(num/mlmMatrix[i][j]);
				

			v=((res-min)/(max-min))*255;
		
			MLMmatrix[i][j]=255 - v;

			
		}
	}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			MLMmatrix[i][j]=255-MLMmatrix[i][j];
			}
	}
*/

	out.close();
	return 0;


}

int MSM(int **minCostMatrixL,int w, int h , int **MSMmatrix, char *filename){
	int i,j;
	
	double res,value,min=INT_MAX,max=INT_MIN;
	
	ofstream out(filename);	
	
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value=-minCostMatrixL[i][j];

			if(value<min)
			{
				min=value;
			}
			if(value>max)
			{
				max=value;
			}
		}
		out << i << " " << j << " " << value << endl;
	}

printf("%f-%f\n", min, max);

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value=-minCostMatrixL[i][j];

			res=((value-min)/(max-min))*(255);
			
			MSMmatrix[i][j] =res;
			//printf("%d\n", MSMmatrix[i][j]);
		}
	}

	out.close();	
	return 0;

}

int PKRN(int **minCostMatrixL,int **secondMinL, int w, int h , int **PKRNmatrix, char *filename){
	
	int i,j;
	double res,max,value,min=INT_MAX;

	
	ofstream out(filename);	
	
	max=0;
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){
			if(minCostMatrixL[i][j]!=0)
			{
				res=secondMinL[i][j]/minCostMatrixL[i][j];
			}
			else
			{
				res=secondMinL[i][j];
			}
			
			if(res<min)
			{
				min=res;
			}
			if(res>max)
			{
				max=res;
			}

			out << i << " " << j << " " << max-res << endl;
		}
	}
	

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			if(minCostMatrixL[i][j]!=0)
			{
				res=secondMinL[i][j]/minCostMatrixL[i][j];
			}
			else
			{
				res=secondMinL[i][j];
			}
		
			value=((res-min)/(max-min))*255;
		
			PKRNmatrix[i][j]=255 - value;
				
			
			
		}
	}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			PKRNmatrix[i][j]=255-PKRNmatrix[i][j];
			}
	}
	*/

	out.close();
	return 0;
}

int PKR(int **minCostMatrixL,int **LocalMinL, int w, int h , int **PKRmatrix, char *filename){
	
	int i,j;
	double res,max,value,min=INT_MAX;
	
	max=0;
	
	ofstream out(filename);	

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			if(minCostMatrixL[i][j]!=0)
			{
				res=LocalMinL[i][j]/minCostMatrixL[i][j];
			}
			else
			{
				res=LocalMinL[i][j];
			}

			if(res>max)
			{
				max=res;
			}
			if(res<min)
			{
				min=res;
			}

			out << i << " " << j << " " << max-res << endl;
		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			if(minCostMatrixL[i][j]!=0)
			{
				res=LocalMinL[i][j]/minCostMatrixL[i][j];
			}
			else
			{
				res=LocalMinL[i][j];
			}
			
			value=(((res-min)/(max-min))*255);
			PKRmatrix[i][j]=255 - value;
		}
	}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			PKRmatrix[i][j]=255-PKRmatrix[i][j];
			}
	}
*/

	out.close();	
	return 0;


}

int WMN(int **minCostMatrixL,int **LocalMinL,int **costSum, int w, int h,int **WMNmatrix, char *filename){

	int i,j;
	double res,value,num,max=0, min=INT_MAX;

	ofstream out(filename);	

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			
			num=LocalMinL[i][j]-minCostMatrixL[i][j];
			
			if(costSum[i][j]!=0)
			{
				res=(num/(costSum[i][j]));
			}
			else
			{
				res=num/0.1;
			}
			
			if(res>max)
			{
				max=res;
			}
			if(res<min)
			{
				
				min=res;
			}

			out << i << " " << j << " " << max-res << endl;
		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			num=(LocalMinL[i][j]-minCostMatrixL[i][j]);
			
			if(costSum[i][j]!=0)
			{
				res=(num/(costSum[i][j]));
			}
			else
			{
				res=num/0.1;
			}
			
			value=(((res-min)/(max-min))*255);
			
			WMNmatrix[i][j]=255 - value;
		}
	}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			WMNmatrix[i][j]=255-WMNmatrix[i][j];
			}
	}
	return 0;
	*/

	out.close();
	return 0;
}

int WMNN(int **minCostMatrixL,int **secondMinL,int **costSum,int w, int h,int **WMNNmatrix, char *filename){
	
	int i,j;
	double max=0,res,value,num,min=INT_MAX;

	ofstream out(filename);	

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){

			num=(secondMinL[i][j]-minCostMatrixL[i][j]);
			if(costSum[i][j]!=0)
			{
				res=num/(costSum[i][j]);
			}
			else
			{
				res=num;
			}

			if(res>max){
				max=res;
			}
			if(res<min){
				min=res;
			}
			out << i << " " << j << " " << max-res << endl;
		}
	}
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){

			num=(secondMinL[i][j]-minCostMatrixL[i][j]);
			if(costSum[i][j]!=0)
			{
				res=num/(costSum[i][j]);
			}
			else
			{
				res=num;
			}
			
			value=((res-min)/(max-min))*255;
			WMNNmatrix[i][j]=255 - value;
		}
	}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			WMNNmatrix[i][j]=255-WMNNmatrix[i][j];
			}
	}
	*/

	out.close();

	return 0;
}

int MMN(int **minCostMatrixL,int **secondMinL, int w, int h, int **MMNmatrix, char *filename){
	
	int i,j;
	double res,max=0,value,min=INT_MAX;

	
	ofstream out(filename);	

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{

			res=secondMinL[i][j]-minCostMatrixL[i][j];
			
			if(res>max)
			{
				max=res;
			}
			if(res<min)
			{
				min=res;
			}

			out << i << " " << j << " " << max-res << endl;
		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++){

			res=secondMinL[i][j]-minCostMatrixL[i][j];
			value=((res-min)/(max-min))*255;
			MMNmatrix[i][j]= 255 - value;
		}
	}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value= 255-MMNmatrix[i][j];
			MMNmatrix[i][j]=value;
			
		}
	}
*/	
	out.close();
	return 0;
}

int CUR(int **minCostMatrixL,int *vettore, int w, int h , int dMax, int **CURmatrix, char *filename){
	
	int i,j,d;
	int minimus,v,index,v_right,v_left;
	double max=0,res,value,min=INT_MAX;

	ofstream out(filename);	


	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			minimus=minCostMatrixL[i][j];
			for(d=0;d<dMax;d++)
			{
				index= w*h*d+i*w+j;
				v=vettore[index];
				
				if(v == minimus)
				{
					if(d>0)
					{
						if(d<(dMax-1))
						{
							v_right=(d+1)*w*h +i*w+j;
							v_left=(d-1)*w*h +i*w+j;
							
							res=(((-2)*minCostMatrixL[i][j])+ vettore[v_left]+vettore[v_right]);
							CURmatrix[i][j]=res;
						}
					}


					if(d>0)
					{
						if(d>(dMax-1))
						{
							v_left=(d-1)*w*h +i*w+j;
							res=((-(2)*minCostMatrixL[i][j])+(2* vettore[v_left]));
							CURmatrix[i][j]=res;
						}
					}

					if(d == 0)
					{
						v_right=(d+1)*w*h +i*w+j;
						res=((-(2)*minCostMatrixL[i][j])+(2*vettore[v_right]));
						CURmatrix[i][j]=res;
					}
				}
			}
		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			if(CURmatrix[i][j]>max){

				max=CURmatrix[i][j];
			}

			if(CURmatrix[i][j]<min){
				min=CURmatrix[i][j];
			}

			out << i << " " << j << " " << max-CURmatrix[i][j] << endl;
		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			
			value=((CURmatrix[i][j]-min)/(max-min))*255;
			CURmatrix[i][j]=255 - value;
		}
	}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value= 255-CURmatrix[i][j];
			CURmatrix[i][j]=value;
			
		}
	}

*/
	out.close();
	return 0;
}

int NOI( int w, int h , int **CardMatrix,int **NOImatrix, char *filename){

	int i,j;
	double max=0,min=INT_MAX,value,res;

	ofstream out(filename);	

		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				
				if(CardMatrix[i][j]>max){

					max=CardMatrix[i][j];
				}

				if(CardMatrix[i][j]<min){

					min=CardMatrix[i][j];
				}	
				
				out << i << " " << j << " " << CardMatrix[i][j] << endl;
			}
		}

	
		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
			
				value=(CardMatrix[i][j]-min);
				res=(value/(max-min))*255;
				NOImatrix[i][j]= res;
				
				
			}
		}

	out.close();
		return 0;
}

int AML(int w, int h ,double **amlMatrix,int **AMLmatrix, char *filename){

	int i,j;
	double res=0,max=0,value,min=LDBL_MAX_10_EXP;

	
	ofstream out(filename);	

	for (i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			res=((1.0)/amlMatrix[i][j])*0.000001;
			

			if(res>max){
				max=res;
			}
			if(res<min){
				min=res;
			}
			out << i << " " << j << " " << max-res << endl;
		}
	}

	for (i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			res=((1.0)/amlMatrix[i][j])*0.000001;
			value=((res-min)/(max-min))*255;

			AMLmatrix[i][j]= 255 - value;
			
			}
		}
/*		
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value= 255-AMLmatrix[i][j];
			AMLmatrix[i][j]=value;
			
		}
	}
*/

	out.close();
	return 0;
}

int LRC(int **disparity_left,int **disparity_right, int w, int h ,int scale_factor, int **LRCmatrix, int **B_and_W, char *filename){
	
	int i,j;
	double max= INT_MIN,min=INT_MAX,value=0,res;
	BYTE disparity_l,disparity_r;
	
	ofstream out(filename);	

		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				
				disparity_l=disparity_left[i][j];

				if(j-disparity_l>=0){
				disparity_r=disparity_right[i][j-disparity_l];
				}
				else{
					disparity_r=0;
				}
				value= disparity_l-disparity_r;

				
		
				#define THRESHOLD 2

				if( abs(value)<=THRESHOLD){
					
					LRCmatrix[i][j]=disparity_left[i][j]*scale_factor;
					B_and_W[i][j]=255;
			
				
				}
				else{
					LRCmatrix[i][j]=255;
					B_and_W[i][j]=0;
				}
			out << i << " " << j << " " << LRCmatrix[i][j] << endl;
			}
		}
	out.close();
		return 0;
}

int LRD(int **minCostMatrixL,int **disparity_left, int **secondMinL, int **minCostMatrixR, int w, int h , int ** LRDmatrix, char *filename){

	int i,j;
	double max=0,min=INT_MAX,value=0,res=0,result;
	BYTE disparity_l;
	
	ofstream out(filename);	
	

	for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				disparity_l=disparity_left[i][j];
				
				if(j-disparity_l>=0)
				{
					value= (abs(minCostMatrixL[i][j]-minCostMatrixR[i][j-disparity_l]));
					
					if(value==0)
					{
						res=((secondMinL[i][j]-minCostMatrixL[i][j]));
					}
					else
					{			
						res=(secondMinL[i][j]-minCostMatrixL[i][j])/value;
					}
				}
				else
				{
					res=((secondMinL[i][j]-minCostMatrixL[i][j]));
				}

				if(res>max)
				{
					max=res;
				}
				if(res<min)
				{
					min=res;
				}
				
			}
		}

		for(i=0;i<h;i++)
		{
			for(j=0;j<w;j++)
			{
				disparity_l=disparity_left[i][j];
				
				if(j-disparity_l>=0)
				{
					value= (abs(minCostMatrixL[i][j]-minCostMatrixR[i][j-disparity_l]));

					if(value==0)
					{
						res=((secondMinL[i][j]-minCostMatrixL[i][j]));
					}
					else
					{			
						res=(secondMinL[i][j]-minCostMatrixL[i][j])/value;
					}
				}
				else
				{
					res=((secondMinL[i][j]-minCostMatrixL[i][j]));
				}
				out << i << " " << j << " " << max-res << endl;
					
				result= (((res-min)/(max-min))*255);
				LRDmatrix[i][j]=255 - result;

			}
		}
/*
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			value= 255-LRDmatrix[i][j];
			LRDmatrix[i][j]=value;
			
		}
	}
	*/
	out.close();
		return 0;

}

int DTS(IplImage *left,int w, int h, int dMax, int dMin, int **DTSmatrix, char *filename)
{
	int i,j,d;
	IplImage *gray;

	if (left->nChannels == 3)
	{
		gray = cvCreateImage(cvGetSize(left), 8, 1);		
		cvCvtColor(left, gray, CV_BGR2GRAY);
	}	
	//else gray = cvCloneImage(left);

	ofstream out(filename);
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			int min = INT_MAX, minD = 0;
			for(d = (dMin - dMax < j) ? dMin - dMax : j; d < dMax - dMin && j + d < w;d++)
			{
				if (abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(left, uchar, i, j + d)) < min && d != 0)
				{
					min = abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(left, uchar, i, j + d));
					minD = d;
				}
			}
			DTSmatrix[i][j] = min;	
			out << i << " " << j << " " << DTSmatrix[i][j] << endl;
		}
	}
	//cvReleaseImage(&gray);
	out.close();
	return 0;
}

int DSM(int **minCostMatrixL, int **minIndexMatrixL, int **DTSmatrixL, int **DTSmatrixR, int w, int h, int dMax, int dMin, int **DSMmatrix, char *filename)
{
	int i,j,d,index;
	float min = INT_MAX, max = 0;
	float **temp = (float**)calloc(h,sizeof(float*));
	for(i=0;i<h;i++) temp[i] = (float*)calloc(w,sizeof(float));
	ofstream out(filename);
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			d = minIndexMatrixL[i][j];
			temp[i][j] = (float)DTSmatrixL[i][j] * (float)DTSmatrixR[i][j - d];

			if (minCostMatrixL[i][j] != 0)
				temp[i][j] = temp[i][j] / ((float)minCostMatrixL[i][j]);
			else if (temp[i][j] != 0) temp[i][j] = 1;

			if(temp[i][j]< min)
				min=temp[i][j];
			if(temp[i][j]> max)
				max=temp[i][j];
			out << i << " " << j << " " << temp[i][j] << endl;
		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			float value = (((temp[i][j]-min)/(max-min))*255.0);
			DSMmatrix[i][j] = value;
		}
	}
	out.close();
	return 0;
}

int SAMM(IplImage *left, IplImage *right, int **minIndexMatrixL, int dMax, int dMin, int **SAMMMmatrix, char *filename)
{
	int i,j,d,index,k;
	int h = left->height;
	int w= left->width;
	float min = INT_MAX, max = 0;
	int totLL= 0, totLR = 0, countLL, countLR;
	float meanLL, meanLR, varLL, varLR;

	IplImage *grayL;	

	if (left->nChannels == 3)
	{
		grayL = cvCreateImage(cvGetSize(left), 8, 1);	
		cvCvtColor(left, grayL, CV_BGR2GRAY);
	}	
	//else grayL = cvCloneImage(left);

	IplImage *grayR;
	if (right->nChannels == 3)
	{
		grayR = cvCreateImage(cvGetSize(left), 8, 1);		
		cvCvtColor(right, grayR, CV_BGR2GRAY);
	}	
	//else grayR = cvCloneImage(right);
	
	ofstream out(filename);

	float **temp = (float**)calloc(h,sizeof(float*));
	for(i=0;i<h;i++) temp[i] = (float*)calloc(w,sizeof(float));

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			totLL = 0;
			totLR = 0;
			countLL = 0;
			countLR = 0;
			for (k = (dMin - dMax < j) ? dMin - dMax : j; k < dMax - dMin && j + k < w; k++)
			{ 
				totLL += abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(left, uchar, i, j + k));
				countLL++;

				totLR += abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(right, uchar, i, j + k));
				countLR++;	
			}

			meanLL = (float)totLL / (float)countLL;
			meanLR = (float)totLR / (float)countLR;

			//printf("%f %f %d %d\n", meanLL, meanLR, countLL, countLR);

			totLL = 0;
			totLR = 0;

			for (k = (dMin - dMax < j) ? dMin - dMax : j; k < dMax - dMin && j + k < w; k++)
			{ 
				totLL += (abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(left, uchar, i, j + k)) - meanLL) * (abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(left, uchar, i, j + k)) - meanLL);

				totLR += (abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(right, uchar, i, j + k)) - meanLR) * (abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(right, uchar, i, j + k)) - meanLR);	
		}

			varLL = (float)totLL / float(countLL);
			varLR = (float)totLR / float(countLR);

			//printf("%f %f\n", varLL, varLR);

			for (k = (dMin - dMax < j) ? dMin - dMax : j; k < dMax - dMin && j + k < w; k++)
			{ 
				temp[i][j] += (abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(right, uchar, i, j + k - minIndexMatrixL[i][j])) - meanLR) * (abs(CV_IMAGE_ELEM(left, uchar, i, j) - CV_IMAGE_ELEM(left, uchar, i, j + k)) - meanLL); 
			}
				temp[i][j] /= (varLL * varLR);

			out << i << " " << j << " " << temp[i][j] << endl;
			
			if(temp[i][j]< min)
				min=temp[i][j];
			if(temp[i][j]> max)
				max=temp[i][j];

		}
	}

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			SAMMMmatrix[i][j] = (((temp[i][j]-min)/(max-min))*255);
		}
	}

	out.close();
	return 0;
}

int PER(int *DSI, int **minCostMatrixL, int **minIndexMatrixL, int w, int h, int dMax, int dMin, int **PERmatrix, char *filename)
{
	int i,j,d,index,k;
	float min = INT_MAX, max = 0;

	float **temp = (float**)calloc(h,sizeof(float*));
	for(i=0;i<h;i++) temp[i] = (float*)calloc(w,sizeof(float));

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			for(d=0;d<dMax;d++)
			{
				int cost, cost1;
				int index = w*h*d+i*w+j;
				cost = DSI[index];
				cost1 = minCostMatrixL[i][j];
				if (d != minIndexMatrixL[i][j])	temp[i][j] = temp[i][j] + exp(-(cost - cost1)*(cost - cost1) / ((float)255*(float)255*(float)255*(float)255));
			}
			printf("%f\n", temp[i][j]);

			if(temp[i][j]< min)
				min=temp[i][j];
			if(temp[i][j]> max && temp[i][j] < FLT_MAX)
				max=temp[i][j];

		}
	}

	printf("MAX %f MIN %f\n", max, min);

	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			PERmatrix[i][j] = (((temp[i][j]-min)/(max-min))*255);
		}
	}
	return 0;
	
}




