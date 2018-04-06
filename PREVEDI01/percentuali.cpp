#include <opencv/cv.h>       
#include <opencv/ml.h>	
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>

#include "matrix.h"
#include <math.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>
#include <omp.h>

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

int main( int argc, char** argv )
{
    vector<string> attributes;
    std::ifstream input("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/differenzeSOMMA4PES");
    string line;
    float quattro = 0;
    float tre = 0;
    float due = 0;
    float uno = 0;
    float zero = 0;
    int counter = 0;

    float quattroPOS = 0;
    float trePOS = 0;
    float duePOS = 0;
    float unoPOS = 0;
    float zeroPOS = 0;
    int counterPOS = 0;

	float max4 = 0;
	float min4 = 0;
	float max3 = 0;
	float min3 = 0;
	float max2 = 0;
	float min2 = 0;
	float max1 = 0;
	float min1 = 0;
	float max0 = 0;
	float min0 = 0;
	
	int indice_max4;
	int indice_min4;
	int indice_max3;
	int indice_min3;
	int indice_max2;
	int indice_min2;
	int indice_max1;
	int indice_min1;
	int indice_max0;
	int indice_min0;

while(getline( input, line ))
{
    line.erase(line.find_last_not_of(" \n\r\t")+1);
    attributes = split(line,' ');

    quattro += ::atof(attributes[1].c_str());
    tre     += ::atof(attributes[2].c_str());
    due     += ::atof(attributes[3].c_str());
    uno     += ::atof(attributes[4].c_str());
    zero    += ::atof(attributes[5].c_str());

    counter++;	

    if(::atof(attributes[1].c_str())>0 && ::atof(attributes[2].c_str())>0 && atof(attributes[3].c_str())>0 &&
	 ::atof(attributes[4].c_str())>0 && ::atof(attributes[5].c_str())>0)
    {
	    quattroPOS += ::atof(attributes[1].c_str());
	    trePOS     += ::atof(attributes[2].c_str());
	    duePOS     += ::atof(attributes[3].c_str());
	    unoPOS     += ::atof(attributes[4].c_str());
	    zeroPOS    += ::atof(attributes[5].c_str());

	    counterPOS++;

    }

    if(::atof(attributes[1].c_str()) > max4){  max4=::atof(attributes[1].c_str()); indice_max4=counter;   }    
    if(::atof(attributes[1].c_str()) < min4){  min4=::atof(attributes[1].c_str()); indice_min4=counter;   }  

    if(::atof(attributes[2].c_str()) > max3){  max3=::atof(attributes[2].c_str()); indice_max3=counter;   }    
    if(::atof(attributes[2].c_str()) < min3){  min3=::atof(attributes[2].c_str()); indice_min3=counter;   }    

    if(::atof(attributes[3].c_str()) > max2){  max2=::atof(attributes[3].c_str()); indice_max2=counter;   }    
    if(::atof(attributes[3].c_str()) < min2){  min2=::atof(attributes[3].c_str()); indice_min2=counter;   }    

    if(::atof(attributes[4].c_str()) > max1){  max1=::atof(attributes[4].c_str()); indice_max1=counter;   }    
    if(::atof(attributes[4].c_str()) < min1){  min1=::atof(attributes[4].c_str()); indice_min1=counter;   }    

    if(::atof(attributes[5].c_str()) > max0){  max0=::atof(attributes[5].c_str()); indice_max0=counter;   }    
    if(::atof(attributes[5].c_str()) < min0){  min0=::atof(attributes[5].c_str()); indice_min0=counter;   }      

}

float media4 = quattro / counter;
float media3 = tre     / counter;
float media2 = due     / counter;
float media1 = uno     / counter;
float media0 = zero    / counter;
    
float media4POS = quattroPOS / counterPOS;
float media3POS = trePOS     / counterPOS;
float media2POS = duePOS     / counterPOS;
float media1POS = unoPOS     / counterPOS;
float media0POS = zeroPOS    / counterPOS;

printf("N° campioni totali: %d\n", counter);
printf("Media 4: %f Media 3: %f Media 2: %f Media 1: %f Media 0: %f\n", media4, media3, media2, media1, media0);

printf("N° campioni in cui il machine learning ha migliorato il risultato: %d\n", counterPOS);
printf("Media 4: %f Media 3: %f Media 2: %f Media 1: %f Media 0: %f\n", media4POS, media3POS, media2POS, media1POS, media0POS); 

printf("Soglia 4 ----> MAX: %f\tMIN: %f\tImmagineMax: %d ImmagineMin: %d\n", max4, min4, indice_max4, indice_min4);
printf("Soglia 3 ----> MAX: %f\tMIN: %f\tImmagineMax: %d ImmagineMin: %d\n", max3, min3, indice_max3, indice_min3);
printf("Soglia 2 ----> MAX: %f\tMIN: %f\tImmagineMax: %d ImmagineMin: %d\n", max2, min2, indice_max2, indice_min2);
printf("Soglia 1 ----> MAX: %f\tMIN: %f\tImmagineMax: %d ImmagineMin: %d\n", max1, min1, indice_max1, indice_min1);
printf("Soglia 0 ----> MAX: %f\tMIN: %f\tImmagineMax: %d ImmagineMin: %d\n", max0, min0, indice_max0, indice_min0);


}
