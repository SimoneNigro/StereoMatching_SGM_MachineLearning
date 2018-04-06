#include "cost.h"

void findTotalCost(t_DSI *DSI,costi **totalCostMatrix){
	
	int i,j,d,h,w,dMax;
	int minimum = INT_MAX;
	int minimumIndex = 0;
	int index,indexL, indexR;

	h = DSI->height;
	w = DSI->width;
	dMax = DSI->dMax;

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			minimum = INT_MAX;
			minimumIndex = 0;
			for(d=0;d<dMax;d++){
				index=d*h*w+i*w+j;
				if(DSI->values[index]<minimum){
					minimum=DSI->values[index];
					minimumIndex=d;
					indexR=((d+1)*h*w+i*w+j);
					indexL=((d-1)*h*w+i*w+j);
				}
			}
			if(minimumIndex>0 && minimumIndex<(dMax-1)){
					/*Trovo il minimo in posizione diversa dalla prima e l'ultima*/
					totalCostMatrix[i][j].costoCentrale=minimum;
					totalCostMatrix[i][j].costoSinistro=DSI->values[indexL];
					totalCostMatrix[i][j].costoDestro=DSI->values[indexR];
				}
				else{
					/*Se il minimo è in ultima posizione o in prima, assegno -1 ai vicini dato che non ci sono*/
					/*Essendo i duei vicini uguali il valore subpixel sarà uguale a 0*/
					/*Utile anche per quando si disegnano gli istogrammi*/
					totalCostMatrix[i][j].costoCentrale=minimum;
					totalCostMatrix[i][j].costoSinistro=-1;
					totalCostMatrix[i][j].costoDestro=-1;
				}
			}
		}
}