#include "SGM.h"
#include "census.h"
#include <omp.h>

/*void SGM()
{
	char* filenameL = NULL;
	char* filenameR = NULL;
	IplImage* L = NULL;
	IplImage* R = NULL;

	//Filename of pair image
	printf("Filename left image: ");
	scanf("%s", filenameL);
	printf("Filename right image: ");
	scanf("%s", filenameR);

	//Open image
	openImage(L,R,filenameL,filenameR);
}


void openImage(IplImage* L, IplImage* R, char* filenameL, char* filenameR)
{
	L=cvLoadImage(filenameL,CV_LOAD_IMAGE_UNCHANGED);
	R=cvLoadImage(filenameR,CV_LOAD_IMAGE_UNCHANGED);
}*/

//modificata per farmi restituire il vettore con le 8 DSI
void compute(IplImage* L, IplImage* R, int dMax, float P1, float P2, t_DSI* DSI, int mask, t_DSI** scanline_DSI)
{
	int i;
	int x, y, d;

	IplImage* censusL=cvCreateImage(cvSize(L->width,L->height),L->depth,L->nChannels),* censusR=cvCreateImage(cvSize(R->width,R->height),R->depth,R->nChannels);
	
	//census_Transform(L,censusL,9);
	//census_Transform(R,censusR,9);

	census_Transform_Binary(L, censusL,9);
	census_Transform_Binary(R, censusR,9);


	clock_t start = clock();
//la pragma mi fa andare il ciclo in parallelo sui vari core, così riduco di molto i tempi di esecuzione
	 #pragma omp parallel for private (i)
	for(i=0;i<8;i++)
	{
		printf("Running scanline %d\n", i);
		//Maschera per selezionare quale/i singola/e scanline utilizzare per la costruzione del DSI
		if((mask >> i) & 1)
		{
			scanline_DSI[i] = create_DSI(L->width, R->height, dMax);	

			//Invocazione della singola scanline
			single_scanline(censusL, censusR, dMax, P1, P2, scanline_DSI[i], i);
		}
	}

//qui mi sommerebbe i contributi per farne una, ma non voglio che lo faccia visto che non mi interessa sgm classico
	/*for (i = 0; i < 8; i++)
		for (y = 0; y < L->height; y++)
			for (x = 0; x < L->width; x++)
				for (d = 0; d < dMax; d++)
				{
					int *v;
					v = (DSI->values+ d*DSI->width*DSI->height +y*DSI->width +x); 
					*v = DSI_ELEMENT(DSI,x,y,d) + DSI_ELEMENT(scanline_DSI[i],x,y,d);
				}*/
	clock_t end = clock();
	printf("Time for scanline %d: %f seconds\n", i, (end - start) / (float)CLOCKS_PER_SEC);


}

void single_scanline(IplImage* L, IplImage* R, int dMax, float P1, float P2, t_DSI* DSI, int type)
{
	//Dichiarazione delle variabili locali
	int x,y,scan,previous_x,previous_y,dim,h,w,i,j;
	int* previous_global_costs;
	int* current_global_costs;
	int height=L->height;
	int width=L->width;

	IplImage *temp_L = cvCreateImage(cvSize(L->width+(5-1),L->height+(5-1)),8,1);
	IplImage *temp_R = cvCreateImage(cvSize(R->width+(5-1),R->height+(5-1)),8,1);

	//Creazione immagini per aggregazione dei costi
	dim=5/2;
	h=temp_L->height;
	w=temp_L->width;

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(i<dim || j<dim || i>=h-dim || j>=w-dim){
				CV_IMAGE_ELEM(temp_L,uchar,i,j)=0;
				CV_IMAGE_ELEM(temp_R,uchar,i,j)=0;
			}else{
				CV_IMAGE_ELEM(temp_L,uchar,i,j)=CV_IMAGE_ELEM(L,uchar,i-dim,j-dim);
				CV_IMAGE_ELEM(temp_R,uchar,i,j)=CV_IMAGE_ELEM(R,uchar,i-dim,j-dim);
			}
		}
	}

	//Allocazione dell'array dei costi precedenti
	previous_global_costs=(int*)calloc(sizeof(int),(dMax+1));

	//Inizializzazione delle variabili posizionali d'appoggio in base al tipo di scanline
	init_xy(&x, &y, width, height, type);
	previous_x=x;
	previous_y=y;

	//Ciclo unico per tutta la singola scanline, la variabile di ritorno della funzione scan_xy() indica se la scanline è conclusa,
	// se è necessario resettare l'array dei costi precedenti o se procedere senza interventi

	while(scan=scan_xy(&x,&y,width,height,type))
	{
		float adjusted_P2;

		if(scan>1)
			//Reset dell'array dei costi precedenti
			memset(previous_global_costs,0,(dMax+1)*sizeof(int));

		//Allocazione dell'array dei costo correnti
		current_global_costs=(int*)calloc(sizeof(int),(dMax+1));

		//Calcolo dello smoothness penalty P2 dinamico
		adjusted_P2=adjust_P2(L, x, y, previous_x, previous_y, P2);

		//Calcolo dei costi globali
		global_costs(temp_L, temp_R, x, y, dMax, P1, adjusted_P2, previous_global_costs, current_global_costs);

		//Aggiunta dell'array al DSI
		add_costs_array_DSI(DSI, x, y, current_global_costs);

		//L'array dei costi correnti viene copiato nell'array dei costi precedenti
		memcpy(previous_global_costs, current_global_costs, (dMax+1)*sizeof(int));

		//Deallocazione dell'array dei costi correnti
		free(current_global_costs);

		previous_x=x;
		previous_y=y;
	}
	free(previous_global_costs);
	
}

void init_xy(int*x, int*y, int width, int height, int typeSS)
{
	if(typeSS==LEFT_RIGHT)
	{
		*x=-1;
		*y=0;
		return;
	}

	if(typeSS==RIGHT_LEFT)
	{
		*x=width;
		*y=0;
		return;
	}

	if(typeSS==UP_DOWN) 
	{
		*x=0;
		*y=-1;
		return;
	}

	if(typeSS==DOWN_UP)
	{
		*x=0;
		*y=height;
		return;
	}
	if(typeSS==UPLEFT_DOWNRIGHT)
	{
		*x=-1;
		*y=height-2;
		return;
	}

	if(typeSS==DOWNRIGHT_UPLEFT)
	{
		*x=width;
		*y=1;
		return;
	}

	if(typeSS==DOWNLEFT_UPRIGHT)
	{
		*x=-1;
		*y=1;
		return;
	}

	if(typeSS==UPRIGHT_DOWNLEFT)
	{
		*x=width;
		*y=height-2;
		return;
	}
}

int scan_xy(int* x,int* y,int width,int height, int typeSS)
{
	/*
	Per ogni invocazione restituisce: 
	0 se la scanline è conclusa
	2 se è conclusa una riga e quindi è necessario resettare l'array d'appoggio
	1 altrimenti
	*/

	if(typeSS==LEFT_RIGHT)
	{
		if(*y>=height-1 && *x>=width-1)
			return 0;
		if(*x>=width-1)
		{
			*y+=1;
			*x=0;
			return 2;
		}
		else
			*x+=1;	
		return 1;
	}

	if(typeSS==RIGHT_LEFT)
	{
		if(*y>=height-1 && *x<=0)
			return 0;
		if(*x<=0)
		{
			*y+=1;
			*x=width-1;
			return 2;
		}
		else
			*x-=1;
		return 1;
	}

	if(typeSS==UP_DOWN)
	{
		if(*x>=width-1 && *y>=height-1)
			return 0;
		if(*y>=height-1)
		{
			*x+=1;
			*y=0;
			return 2;
		}
		else
			*y+=1;
		return 1;
	}

	if(typeSS==DOWN_UP)
	{
		if(*x>=width-1 && *y<=0)
			return 0;
		
		if(*y<=0)
		{
			*x+=1;
			*y=height-1;
			return 2;
		}
		else
			*y-=1;
		return 1;
	}

	if(typeSS==UPLEFT_DOWNRIGHT)
	{
		if(*x>=width-1 && *y<=0)
			return 0;
		if(*x>=width-1 || *y>=height-1)
		{
			if(*x<height-1)
			{
				*y-=(*x+1);
				*x=0;
			}
			else
			{
				*x-=(*y-1);
				*y=0;
			}
			return 2;
		}
		else
		{
			*x+=1;
			*y+=1;
			return 1;
		}
	}

	if(typeSS==DOWNRIGHT_UPLEFT)
	{
		if(*x<=0 && *y>=height-1)
			return 0;
		if(*y<=0 || *x<=0)
		{
			if(*x>width-height)
			{
				*y=width-*x;
				*x=width-1;
			}
			else
			{
				*x+=(height-2-*y);
				*y=height-1;
			}
			return 2;
		}
		else
		{
			*x-=1;
			*y-=1;
		}
		return 1;
	}

	if(typeSS==DOWNLEFT_UPRIGHT)
	{
		if(*x>=width-1 && *y>=height-1)
			return 0;
		if(*x>=width-1 || *y<=0)
		{
			if(*x<height-1)
			{
				*y=*x+1;
				*x=0;
			}
			else
			{
				*x-=(height-2-*y);
				*y=height-1;
			}
			return 2;
		}
		else
		{
			*y-=1;
			*x+=1;
		}
		return 1;
	}

	if(typeSS==UPRIGHT_DOWNLEFT)
	{
		if(*x<=0 && *y<=0)
			return 0;
		if(*x<=0 ||*y>=height-1)
		{
			if(*x>width-height)
			{
				*y=height-(width-*x+1);
				*x=width-1;
			}
			else
			{
				*x+=(*y-1);
				*y=0;
			}
			return 2;
		}
		else
		{
			*x-=1;
			*y+=1;
		}
		return 1;
	}
}

void global_costs(IplImage *L, IplImage* R, int x, int y, int dMax, float P1, float P2, int* previous_global_costs, int* global_costs)
{
	/*
	Non è necessario considerare tutti i valori delle disparità del punto precedente, 
	ma al più 4 valori per trovare il costo minore:

	1)Costo del punto precedente alla disparità corrente
	2)Costo del punto precedente alla disparità strettamente minore alla disparità corrente (se non sto considerando la disparità 0)
	3)Costo del punto precedente alla disparità strettamente maggiore alla disparità corrente (se non sto considerando la disparità dMax)
	4)Costo minore del punto precedente + P2 che sarà il valore massimo che può assumere il costo corrente a meno del costo puntuale
	*/

	int i,threshold_cost=INT_MAX;
	for(i=0;i<dMax+1;i++)
	{
		if(previous_global_costs[i]<threshold_cost)
			threshold_cost=previous_global_costs[i];
	}
	threshold_cost+=P2;
	for(i=0;i<dMax+1;i++)
	{
		int best_cost=previous_global_costs[i];
		if(i>0 && previous_global_costs[i-1]+P1<best_cost)
			best_cost=previous_global_costs[i-1]+P1;
		if(i<dMax && previous_global_costs[i+1]+P1<best_cost)
			best_cost=previous_global_costs[i+1]+P1;
		if(threshold_cost<best_cost)
			best_cost=threshold_cost;
		global_costs[i]=cost(L, R, x, y, i)+best_cost-(threshold_cost-P2);
	}
}

int cost(IplImage* L, IplImage* R, int x, int y, int d)
{
	int x_R=x-d;
	if(x_R<0)
		x_R=0;
	return hamming_Aggregation(L,R,x,y,x_R,y);//hamming_Distance(CV_IMAGE_ELEM(L,uchar,y,x*L->nChannels),CV_IMAGE_ELEM(R,uchar,y,x_R*L->nChannels))*20;
}

float adjust_P2(IplImage* image, int x, int y, int previous_x, int previous_y, float P2)
{
	return P2;
	/*
	float adj_P2=P2;
	float a=0.5;
	float c=200;
	if(previous_x<0)
		previous_x=0;
	if(previous_y<0)
		previous_y=0;
	if(previous_x>=image->width)
		previous_x--;
	if(previous_y>=image->height)
		previous_y--;
	adj_P2=P2-abs(CV_IMAGE_ELEM(image,uchar,y,x)-CV_IMAGE_ELEM(image,uchar,previous_y,previous_x));
	if(adj_P2<50)
		return 50;
	return adj_P2;*/
}
