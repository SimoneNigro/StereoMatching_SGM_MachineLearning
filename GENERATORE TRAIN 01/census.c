#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <opencv/cv.h>
#include "census.h"


uchar evaluate_Census_Binary(uchar center,uchar left,uchar up,uchar right,uchar down){

	uchar census=0,bit=0;
	bit = center >= left ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= up ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= right ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= down ? 1:0;
	census <<= 1;
	census+=bit;
	return census;
}

uchar evaluate_Census_Tris(uchar center,uchar left,uchar up,uchar right,uchar down){

	uchar census=0,bit=0;
	if(center>left){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 1;
		census <<= 1;
		census+=bit;
	}else if(center < left){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}else{
		bit = 1;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}
	if(center>up){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 1;
		census <<= 1;
		census+=bit;
	}else if(center < up){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}else{
		bit = 1;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}
	if(center>right){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 1;
		census <<= 1;
		census+=bit;
	}else if(center < right){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}else{
		bit = 1;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}
	if(center>down){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 1;
		census <<= 1;
		census+=bit;
	}else if(center < down){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}else{
		bit = 1;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}
	return census;
}

uchar evaluate_Mini_Census(uchar center,uchar left,uchar up,uchar right,uchar down,uchar firstup,uchar firstdown){

	uchar census=0,bit=0;
	bit = center >= left ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= up ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= firstup ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= right ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= down ? 1:0;
	census <<= 1;
	census+=bit;
	bit = center >= firstdown ? 1:0;
	census <<= 1;
	census+=bit;
	return census;
}

uchar compare_Value_Ternary(uchar center, uchar other){

	uchar census=0,bit=0;

	if(center>other){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 1;
		census <<= 1;
		census+=bit;
	}else if(center<other){
		bit = 0;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}else{
		bit = 1;
		census <<= 1;
		census+=bit;
		bit = 0;
		census <<= 1;
		census+=bit;
	}

	return census;

}

uchar compare_Value_Binary(uchar center, uchar other){
	
	uchar census=0,bit=0;

	if(center>=other){
		bit = 1;
		census <<= 1;
		census+=bit;
	}else{
		bit = 0;
		census <<= 1;
		census+=bit;
	}
	
	return census;

}

int evaluate_Census(IplImage *imageWindow){

	int i,j,dim,row,h,w;
	uchar center,up,left,right,down,temp;
	int census = 0;

	h = imageWindow->height;
	w = imageWindow->width;
	dim = h/2;

	center = CV_IMAGE_ELEM(imageWindow,uchar,dim,dim);
	left=CV_IMAGE_ELEM(imageWindow,uchar,dim,dim-1);
	up=CV_IMAGE_ELEM(imageWindow,uchar,dim-1,dim);
	right=CV_IMAGE_ELEM(imageWindow,uchar,dim,dim+1);
	down=CV_IMAGE_ELEM(imageWindow,uchar,dim+1,dim);

	census = evaluate_Census_Tris(center,left,right,up,down);

	left=CV_IMAGE_ELEM(imageWindow,uchar,dim,0);
	up=CV_IMAGE_ELEM(imageWindow,uchar,0,dim);
	right=CV_IMAGE_ELEM(imageWindow,uchar,dim,w-1);
	down=CV_IMAGE_ELEM(imageWindow,uchar,h-1,dim);

	census <<= 8;
	census += evaluate_Census_Tris(center,left,right,up,down);


	row=1;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if((i==j || (i+j)==(h-1)) && row%2!=0 && (i!=dim && j!=dim)){
				temp = CV_IMAGE_ELEM(imageWindow,uchar,i,j);
				census <<= 1;
				census += compare_Value_Ternary(center,temp);
			}
		}
		row++;
	}
	
	return census;
}

int hamming_Distance(int left, int right){

	int distance=0;
	int val = left ^ right;

	while (val!=0)
	{
		val = val & (val-1);
		distance++;
	}

	return distance;

}

int census_Transform_Binary(IplImage *image, IplImage *result,int window){
	
	int i,j,h,w,dim,x,y;
	long census,bit,centerCount;
	uchar center,left,right,up,down;
	IplImage *temp = cvCreateImage(cvSize(image->width+(window-1),image->height+(window-1)),8,1);
	
	dim=window/2;
	h=temp->height;
	w=temp->width;
	
	//Costruisco un'immagine nuova con un bordo grande come window/2, in modo tale da poter fare
	//la trasformata anche ai pixel che non hanno i vicini destri o sinistri
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(i<dim || j<dim || i>=h-dim || j>=w-dim){
				CV_IMAGE_ELEM(temp,uchar,i,j)=0;
			}else{
				CV_IMAGE_ELEM(temp,uchar,i,j)=CV_IMAGE_ELEM(image,uchar,i-dim,j-dim);
			}
		}
	}
	for(i=dim;i<h-dim;i++){
		for(j=dim;j<w-dim;j++){
			census=0;
			centerCount=0;
			for (x=i-dim;x<=i+dim;x++){
				for(y=j-dim;y<=j+dim;y++){
					if(centerCount!=window*window/2){//scarto il pixel centrale
						census <<= 1;
						bit = compare_Value_Binary(CV_IMAGE_ELEM(temp,uchar,i,j),CV_IMAGE_ELEM(temp,uchar,x,y));
						census+=bit;
					}
					centerCount++;
				}
			}
			CV_IMAGE_ELEM(result,uchar,i,j)=census;
			//printf("%ld - %d\n", census, CV_IMAGE_ELEM(result,uchar,i,j));
		}
	}
	return 1;

}

int census_Transform_Ternary(IplImage *image, IplImage *result,int window){
	
	int i,j,h,w,dim,x,y;
	long census,bit,centerCount;
	uchar center,left,right,up,down;
	IplImage *temp = cvCreateImage(cvSize(image->width+(window-1),image->height+(window-1)),8,1);
	
	dim=window/2;
	h=temp->height;
	w=temp->width;
	
	//Costruisco un'immagine nuova con un bordo grande come window/2, in modo tale da poter fare
	//la trasformata anche ai pixel che non hanno i vicini destri o sinistri
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(i<dim || j<dim || i>=h-dim || j>=w-dim){
				CV_IMAGE_ELEM(temp,uchar,i,j)=0;
			}else{
				CV_IMAGE_ELEM(temp,uchar,i,j)=CV_IMAGE_ELEM(image,uchar,i-dim,j-dim);
			}
		}
	}
	for(i=dim;i<h-dim;i++){
		for(j=dim;j<w-dim;j++){
			census=0;
			centerCount=0;
			for (x=i-dim;x<=i+dim;x++){
				for(y=j-dim;y<=j+dim;y++){
					if(centerCount!=window*window/2){//scarto il pixel centrale
						census <<= 1;
						bit = compare_Value_Ternary(CV_IMAGE_ELEM(temp,uchar,i,j),CV_IMAGE_ELEM(temp,uchar,x,y));
						census+=bit;
					}
					centerCount++;
				}
			}
			CV_IMAGE_ELEM(result,uchar,i,j)=census;
		}
	}
	return 1;

}

int census_Transform_Binary_Cross(IplImage *image, IplImage *result,int window){
	
	int i,j,h,w,dim;
	uchar center,left,right,up,down;
	IplImage *temp = cvCreateImage(cvSize(image->width+(window-1),image->height+(window-1)),8,1);
	
	dim=window/2;
	h=temp->height;
	w=temp->width;
	
	//Costruisco un'immagine nuova con un bordo grande come window/2, in modo tale da poter fare
	//la trasformata anche ai pixel che non hanno i vicini destri o sinistri
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(i<dim || j<dim || i>=h-dim || j>=w-dim){
				CV_IMAGE_ELEM(temp,uchar,i,j)=0;
			}else{
				CV_IMAGE_ELEM(temp,uchar,i,j)=CV_IMAGE_ELEM(image,uchar,i-dim,j-dim);
			}
		}
	}
	for(i=dim;i<h-dim;i++){
		for(j=dim;j<w-dim;j++){
			center=CV_IMAGE_ELEM(temp,uchar,i,j);
			left=CV_IMAGE_ELEM(temp,uchar,i,j-dim);
			up=CV_IMAGE_ELEM(temp,uchar,i+dim,j);
			right=CV_IMAGE_ELEM(temp,uchar,i,j+dim);
			down=CV_IMAGE_ELEM(temp,uchar,i-dim,j);
			CV_IMAGE_ELEM(result,uchar,i-dim,j-dim)=evaluate_Census_Binary(center,left,right,up,down);
		}
	}
	return 1;
}

int census_Transform_Ternary_Cross(IplImage *image, IplImage *result,int window){
	
	int i,j,h,w,dim;
	uchar center,left,right,up,down;
	IplImage *temp = cvCreateImage(cvSize(image->width+(window-1),image->height+(window-1)),8,1);
	
	dim=window/2;
	h=temp->height;
	w=temp->width;

	//Costruisco un'immagine nuova con un bordo grande come window/2, in modo tale da poter fare
	//la trasformata anche ai pixel che non hanno i vicini destri o sinistri
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(i<dim || j<dim || i>=h-dim || j>=w-dim){
				CV_IMAGE_ELEM(temp,uchar,i,j)=0;
			}else{
				CV_IMAGE_ELEM(temp,uchar,i,j)=CV_IMAGE_ELEM(image,uchar,i-dim,j-dim);
			}
		}
	}
	//Per ogni pixel 
	for(i=dim;i<h-dim;i++){
		for(j=dim;j<w-dim;j++){
			center=CV_IMAGE_ELEM(temp,uchar,i,j);
			left=CV_IMAGE_ELEM(temp,uchar,i,j-dim);
			up=CV_IMAGE_ELEM(temp,uchar,i+dim,j);
			right=CV_IMAGE_ELEM(temp,uchar,i,j+dim);
			down=CV_IMAGE_ELEM(temp,uchar,i-dim,j);
			CV_IMAGE_ELEM(result,uchar,i-dim,j-dim)=evaluate_Census_Tris(center,left,right,up,down);
		}
	}
	return 1;
}

int sparse_Census_Transform(IplImage *image, IplImage *result,int window){

	int i,j,h,w,x,y,row,col,dim;
	uchar center,left,right,up,down;
	IplImage *temp = cvCreateImage(cvSize(image->width+(window-1),image->height+(window-1)),8,1);
	IplImage *imageWindow = cvCreateImage(cvSize(window,window),8,1);
	
	dim=window/2;
	h=temp->height;
	w=temp->width;

	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(i<dim || j<dim || i>=h-dim || j>=w-dim){
				CV_IMAGE_ELEM(temp,uchar,i,j)=0;
			}else{
				CV_IMAGE_ELEM(temp,uchar,i,j)=CV_IMAGE_ELEM(image,uchar,i-dim,j-dim);
			}
		}
	}

	row=0;
	col=0;

	for(i=dim;i<h-dim;i++){
		for(j=dim;j<w-dim;j++){
			for(x=i-dim;x<i+dim;x++){
				for(y=j-dim;y<j+dim;y++){
					CV_IMAGE_ELEM(imageWindow,uchar,row,col)=CV_IMAGE_ELEM(image,uchar,x,y);
					col++;
				}
				col=0;
				row++;
			}
			col=0;
			row=0;
			CV_IMAGE_ELEM(result,uchar,i-dim,j-dim)=evaluate_Census(imageWindow);
		}
	}
	return 1;

}

int mini_Census_Transform(IplImage *image, IplImage *result){

	int i,j,h,w,dim;
	uchar center,left,right,up,down,firstup,firstdown;
	IplImage *temp = cvCreateImage(cvSize(image->width+(5-1),image->height+(5-1)),8,1);
	
	dim=5/2;
	h=temp->height;
	w=temp->width;

	//Costruisco un'immagine nuova con un bordo grande come window/2, in modo tale da poter fare
	//la trasformata anche ai pixel che non hanno i vicini destri o sinistri
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			if(i<dim || j<dim || i>=h-dim || j>=w-dim){
				CV_IMAGE_ELEM(temp,uchar,i,j)=0;
			}else{
				CV_IMAGE_ELEM(temp,uchar,i,j)=CV_IMAGE_ELEM(image,uchar,i-dim,j-dim);
			}
		}
	}
	for(i=dim;i<h-dim;i++){
		for(j=dim;j<w-dim;j++){
			center=CV_IMAGE_ELEM(temp,uchar,i,j);
			left=CV_IMAGE_ELEM(temp,uchar,i,j-dim);
			up=CV_IMAGE_ELEM(temp,uchar,i+dim,j);
			right=CV_IMAGE_ELEM(temp,uchar,i,j+dim);
			down=CV_IMAGE_ELEM(temp,uchar,i-dim,j);
			firstdown=CV_IMAGE_ELEM(temp,uchar,i-1,j);
			firstup=CV_IMAGE_ELEM(temp,uchar,i+1,j);
			CV_IMAGE_ELEM(result,uchar,i-dim,j-dim)=evaluate_Mini_Census(center,left,right,up,down,firstup,firstdown);
		}
	}
	return 1;
}



int hamming_Aggregation(IplImage *L_Trasformed, IplImage *R_Trasformed,int x_L,int y_L,int x_R, int y_R){
	
	int i,j,h,w,dim=5/2,x,y,total=0;
	
	for(i=y_L-dim;i<=y_L+dim;i++){
		for(j=x_L-dim;j<=x_L+dim;j++){
			total += hamming_Distance_bis(CV_IMAGE_ELEM(L_Trasformed,uchar,i+dim,j+dim),CV_IMAGE_ELEM(R_Trasformed,uchar,i+dim,j+dim+(x_R-x_L)));
		}
	}
	
	return total;
}

int hamming_Distance_bis(int left, int right){

	int distance=0;
	int val = left ^ right;

	while (val!=0)
	{
		val = val & (val-1);
		distance++;
	}

	return distance;

}


uchar hamming_Distance(uchar left, uchar right){

	uchar distance=0;
	uchar val = left ^ right;

	while (val!=0)
	{
		val = val & (val-1);
		distance++;
	}

	return distance;

}




