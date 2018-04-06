#include <stdio.h>
#include <stdlib.h>
#include <opencv/highgui.h>
#include "cost.h"






int **trovaMinimi(int *v,int w, int h,int dMax){

	int i,j,x=0,z=0;
	int **minimusL;
	
	int minimum = INT_MAX;
	int minimumIndex = 0;
	int index,d;
	//Alloco matrice dei minimi
	minimusL=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++){
		minimusL[i]=(int*)calloc(w,sizeof(int));
	}


	
	
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			minimum = INT_MAX;
			minimumIndex = 0;
			for(d=0;d<dMax;d++){
				
				index=d*w*h+i*w+j;
				if(v[index]<minimum){
					minimum=v[index];
					minimumIndex=d;				
				}
			}
			minimusL[i][j]=minimum;
			
		}
	
	}
	return minimusL;
	

}

int **trovaDisparitaL(int *v,int w, int h,int dMax){

	int i,j,x=0,z=0;
	int **minimusIndexL;
	
	int minimum = INT_MAX;
	int minimumIndex = 0;
	int index,d;
	//Alloco matrice dei minimi
	minimusIndexL=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++){
		minimusIndexL[i]=(int*)calloc(w,sizeof(int));
	}


	
	
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			minimum = INT_MAX;
			minimumIndex = 0;
			for(d=0;d<dMax+1;d++){
				
				index=d*w*h+i*w+j;
				if(v[index]<minimum){
					minimum=v[index];
					minimumIndex=d;				
				}
			}
			minimusIndexL[i][j]=minimumIndex;
			
		}
	
	}
	return minimusIndexL;
	
}

int **trovaDisparitaR(int *v,int w, int h,int dMax){

	int i,j,x=0,z=0;
	int **minimusIndexR;
	
	int minimum = INT_MAX;
	int minimumIndex = 0;
	int index,d;
	//Alloco matrice dei minimi
	minimusIndexR=(int**)calloc(h,sizeof(int*));
	for(i=0;i<h;i++){
		minimusIndexR[i]=(int*)calloc(w,sizeof(int));
	}


	//(DSI->values+ d*w*h +i*w+i+d);
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			minimum = INT_MAX;
			minimumIndex = 0;
			
			for(d=0;d<dMax && d+j<w;d++)
			{
				

						index=d*w*h+(i*w)+(d+j);
						if(v[index]<minimum)
						{
							minimum=v[index];
							minimumIndex=d;				
						}

			}
			minimusIndexR[i][j]=minimumIndex;
			
		}
			
	}

	return minimusIndexR;
}



int findSecondMinimums(int *vettore,int w, int h,int dMax, int** secondMinimumMatrix){

	int i,j,d,v,index;
	int minimum = INT_MAX;
	int secondMinimum = INT_MAX;
	
	

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			minimum = INT_MAX;
			for(d=0;d<dMax;d++){
			
				index=d*w*h +i*w+j;
				v= vettore[index];
				
				if(v<minimum)
				{
					secondMinimum=minimum;
					minimum=v;
				}

				else if(v>=minimum)
				{
					if(v<=secondMinimum)
					{
						secondMinimum=v;
					}
				}
				
			}
			secondMinimumMatrix[i][j]=secondMinimum;
		}
	}
	return 0;
}

int findLocalMinimum(int *vettore,int w, int h,int dMax, int** localMinimumMatrix){
	
	int i,j,d;
	int minimum;
	int LocalMinimum;
	int SecondMinimum;
	int minInd, secMinIn,secMinInOld;
	int v_right,v_left,v,index,index_right,index_left;



	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			minimum = INT_MAX;
			LocalMinimum=0;
			SecondMinimum=INT_MAX;
			minInd=0;
			secMinIn=0;
			for(d=0;d<dMax;d++)
			{
				
				index=d*w*h +i*w+j;
				 v=vettore[index];

				if(v<minimum)
				{
					SecondMinimum=minimum;
					minimum=v;
				
					secMinIn=minInd;
					minInd=d;	

					if(secMinIn>0)
					{
						if(secMinIn<(dMax-1) )
						{
							index_left=(secMinIn-1)*w*h +i*w+j;
							index_right=(secMinIn+1)*w*h +i*w+j;
							index=secMinIn*w*h +i*w+j;
							
							v=vettore[index];
							v_left=vettore[index_left];
							v_right=vettore[index_right];
							
							if(v_left>v && v_right>v)
							{
								LocalMinimum=SecondMinimum;
							}
						}
					}
				}
				
				if(v>minimum)
				{
					if(v<SecondMinimum)
					{
						secMinInOld=secMinIn;
						secMinIn=d;
						SecondMinimum=v;
						
							index_left=(secMinIn-1)*w*h +i*w+j;
							index_right=(secMinIn+1)*w*h +i*w+j;
							
							
							
							v_left=vettore[index_left];
							v_right=vettore[index_right];
							
						if(v_left>v && v_right>v)
						{
							LocalMinimum=v;
						}
			
					}
					
					else if(v_left>v && v_right>v)
					{
						if(v<LocalMinimum)
						{
							LocalMinimum=v;
						}
					}
				}
			}
		
			localMinimumMatrix[i][j]=LocalMinimum;
		}
	}
	return 0;
}
