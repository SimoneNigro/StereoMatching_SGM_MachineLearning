#ifndef COST_H
#define COST_H

#include <stdlib.h>
#include "DSI.h"

typedef struct 
{
	int costoCentrale;
	int costoDestro;
	int costoSinistro;
} costi;

void findTotalCost(t_DSI *DSI,costi **totalCostMatrix);

#endif
