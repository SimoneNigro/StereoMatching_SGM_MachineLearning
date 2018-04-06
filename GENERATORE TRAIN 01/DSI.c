#include "DSI.h"
#pragma warning (disable : 4996)

void free_DSI(t_DSI* DSI)
{
	free(DSI->values);
	free(DSI);
}

t_DSI* create_DSI(int width, int height, int dMax)
{
	int *values;
	t_DSI* DSI=(t_DSI*)malloc(sizeof(t_DSI));
	DSI->dMax=dMax;
	DSI->width=width;
	DSI->height=height;
	values=(int*)malloc(sizeof(int)*(dMax+1)*width*height);
	DSI->values=values;

	//Valori del DSI settati a -1
	memset(DSI->values,-1,(dMax+1)*width*height*sizeof(int));

	return DSI;
}

void add_costs_array_DSI(t_DSI* DSI, int x, int y, int* costs_array)
{
	//Al primo inserimento i campi saranno uguali a -1, quindi viene salvato il valore 
	//Altrimenti viene sommato al valore precedentemente salvato

	int d;
	int * v;
	for(d=0;d<DSI->dMax+1;d++)
	{
		v=(DSI->values+ d*DSI->width*DSI->height +y*DSI->width +x);
		if(*v<0)
			*v=costs_array[d];
		else
			*v+=costs_array[d];
	}
}

void disparity_map(t_DSI* DSI, IplImage* disparity_map)
{
	int x, y, d, scale_factor;
	scale_factor=(pow(2,disparity_map->depth))/DSI->dMax;

	for(y=0;y<DSI->height;y++)
	{
		for(x=0;x<DSI->width;x++)
		{
			int best_d=0;
			int best_c=INT_MAX;
			for(d=0;d<(DSI->dMax+1);d++)
			{
				if(*(DSI->values + d * DSI->width * DSI->height + y*DSI->width + x)<best_c)
				{
					best_c=*(DSI->values + d * DSI->width * DSI->height + y*DSI->width + x);
					best_d=d;
				}
			}
			CV_IMAGE_ELEM(disparity_map,ushort,y,x)=best_d*scale_factor;
		}
	}
}

void printDSI(t_DSI * DSI, char* filename)
{
	int x,y,d;
	FILE *f=fopen(filename,"w");
	fprintf(f,"%d %d %d\n",DSI->width,DSI->height,DSI->dMax);
	for(y=0; y<DSI->height;y++)
	{
		for(x=0;x<DSI->width;x++)
		{
			for(d=0; d<DSI->dMax+1;d++)
			{
				fprintf(f,"%d ",*(DSI->values+ d*DSI->width*DSI->height +y*DSI->width +x));
			}
			fprintf(f,"\n");
		}
	}
	fclose(f);
				
}

t_DSI* readDSI(char* filename)
{
	int x,y,d,width=0,height=0,dMax=0;
	t_DSI* DSI;
	FILE *f=fopen(filename,"r");
	fscanf(f,"%d %d %d",  &width,&height,&dMax);
	DSI=create_DSI(width,height,dMax);
	for(y=0; y<DSI->height;y++)
	{
		for(x=0;x<DSI->width;x++)
		{
			for(d=0; d<DSI->dMax+1;d++)
			{
				fscanf(f,"%d",(DSI->values+ d*DSI->width*DSI->height +y*DSI->width +x));
			}
		}
	}
	fclose(f);
	return DSI;
}

