#include "interpolation.h"

float parabolaInterpolation(int leftC, int c, int rightC){

	float dispSubPixel, p, leftDif, rightDif;

	leftDif = leftC - c;
	rightDif = rightC - c;

	if(leftDif<=rightDif){
		p = leftDif/rightDif;
		dispSubPixel = -0.5 + (p/(p+1));
	}
	else{
		p = rightDif/leftDif;
		dispSubPixel = 0.5 -(p/(p+1));
	}

	return dispSubPixel;

}

float equiangularInterpolation(int leftC, int c, int rightC){

	float dispSubPixel, p, leftDif, rightDif;

	leftDif = leftC - c;
	rightDif = rightC - c;

	if(leftDif<=rightDif){
		p = leftDif/rightDif;
		dispSubPixel = -0.5 + (p/(2));
	}
	else{
		p = rightDif/leftDif;
		dispSubPixel = 0.5 - (p/(2));
	}

	return dispSubPixel;

}