#include<stdlib.h>
#include<stdio.h>
#include <math.h>


int findCostSumNEM(int *vettore,int w, int h, int dMax,double **costSumNEM){
	
	// trovo il denominatore di p(d)

	int i,j,d,v,index;

	double res,value,sum;
	
	
	
	
	
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			sum = 0;
			for(d=0;d<dMax;d++){
				
				index=w*h*d+i*w+j;
				v= vettore[index];
				value=v*0.01;
				res=exp(-value);
				sum += res ;
			}
			costSumNEM[i][j]=sum;
		}
	}
	return 0;
}

int mlmUtility(int *vettore,int w, int h, int dMax, double **mlmMatrix){

	int i,j,d,v,index;

	double res=0,value,parz=0,sum=0;
	
	
	for (i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			for(d=0;d<dMax;d++)
			{

				index=d*w*h +i*w+j;
				v=vettore[index];
				
				
				value=((v/(2*0.3*0.3)))*0.0001;
			    res =  exp( - value );
				sum =+ res;
			}

			mlmMatrix[i][j]=sum;
		}
	}

	return 0;
}

int amlUtility(int *vettore,int w, int h, int dMax, int **minCostMatrixL,double **amlMatrix){

	int i,j,d,v,index;

	double res=0,num,dn,max=0,value=0;

	
	
	for (i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			res=0;
			for(d=0;d<dMax;d++)
			{
				index=d*w*h +i*w+j;
				v=vettore[index]; 
				num=((v-minCostMatrixL[i][j])*(v-minCostMatrixL[i][j]));
				dn=num/(2*0.1*0.1)*0.000001;
				res +=  exp(-dn);

			}
		amlMatrix[i][j]=res;
		}
	}
	return 0;

}


int findValueNOI(int *vettore,int w, int h,int dMax, int **CardMatrix){

	int i,j,d,index,index_right,index_left,v,v_right,v_left;
	int res=0;


	

	
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			res=0;
			for(d=1;d<6;d++)
			{
				index= d*w*h +i*w+j;
				index_right=(d+1)*w*h +i*w+j;
				index_left=(d-1)*w*h +i*w+j;

				v=vettore[index];
				v_right= vettore[index_right];
				v_left=vettore[index_left];
				

				if(d>0)
				{
					if(d<(dMax-1))
					{
						
						if(v_left>v)
						{ 
						
							if(v_right>v)
							{
								
								res++;
							}
						}
					}
				}
			}
			CardMatrix[i][j]=-res;
		}
	}
	return 0;
}

int findCostSum(int *vettore,int w, int h,int dMax,int **costSum){
	
	int i,j,d,sum,v,index;

	
	
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			
			sum = 0;
			for(d=0;d<dMax;d++)
			{
				index= d*w*h +i*w+j;
				
				v=vettore[index];

				sum += v;
			}
			costSum[i][j]=sum;
		}
	}
	return 0;
}
