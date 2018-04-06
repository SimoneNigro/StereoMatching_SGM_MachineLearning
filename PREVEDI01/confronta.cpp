#include <opencv/cv.h>       // opencv general include file
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std; // OpenCV API is in the C++ "cv" namespace
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
/******************************************************************************/

void confronta(IplImage* DispMap, IplImage* Normale, IplImage* GroundTruth, char *q)
{
    
    int w = DispMap->width;
    int h = DispMap->height;

    int corretti4 = 0;    int corretti4norm = 0;
    int corretti3 = 0;    int corretti3norm = 0;
    int corretti2 = 0;    int corretti2norm = 0;
    int corretti1 = 0;    int corretti1norm = 0;
    int corretti0 = 0;    int corretti0norm = 0;
	int validi = 0;

    	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			//se il pixel della ground truth non ha intensita 0 lo considero
			if (CV_IMAGE_ELEM(GroundTruth, uchar, i, j) != 0) 
			{	
				validi++;
		
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(DispMap, uchar, i, j)) <= 4 )
				{	
					corretti4++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(DispMap, uchar, i, j)) <= 3 )
				{	
					corretti3++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(DispMap, uchar, i, j)) <= 2 )
				{	
					corretti2++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(DispMap, uchar, i, j)) <= 1 )
				{	
					corretti1++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(DispMap, uchar, i, j)) == 0 )
				{	
					corretti0++;				
				}
		//normale
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Normale, uchar, i, j)) <= 4 )
				{	
					corretti4norm++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Normale, uchar, i, j)) <= 3 )
				{	
					corretti3norm++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Normale, uchar, i, j)) <= 2 )
				{	
					corretti2norm++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Normale, uchar, i, j)) <= 1 )
				{	
					corretti1norm++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Normale, uchar, i, j)) == 0 )
				{	
					corretti0norm++;				
				}
			}	
					
		}
	}

    float percentuale4 =  ((float) (validi - corretti4)) / ((float)validi) * 100;
    float percentuale3 =  ((float) (validi - corretti3)) / ((float)validi) * 100;
    float percentuale2 =  ((float) (validi - corretti2)) / ((float)validi) * 100;
    float percentuale1 =  ((float) (validi - corretti1)) / ((float)validi) * 100;
    float percentuale0 =  ((float) (validi - corretti0)) / ((float)validi) * 100;

    float percentuale4norm =  ((float) (validi - corretti4norm)) / ((float)validi) * 100;
    float percentuale3norm =  ((float) (validi - corretti3norm)) / ((float)validi) * 100;
    float percentuale2norm =  ((float) (validi - corretti2norm)) / ((float)validi) * 100;
    float percentuale1norm =  ((float) (validi - corretti1norm)) / ((float)validi) * 100;
    float percentuale0norm =  ((float) (validi - corretti0norm)) / ((float)validi) * 100;

  ofstream myfile ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/confronti", ios::out | ios::app );
  ofstream myfile2 ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/differenze", ios::out | ios::app );
  if (myfile.is_open())
  {
    myfile << "ERRORE SGM_ML Id: " << q << " Soglia 4: " << percentuale4 << "% Soglia 3: " << percentuale3 << "% Soglia 2: " << percentuale2 
		<< "% Soglia 1: " << percentuale1 << "% Soglia 0: " << percentuale0 << "%\n";
    myfile << "ERRORE SGM Id: " << q << " Soglia 4: " << percentuale4norm << "% Soglia 3: " << percentuale3norm << "% Soglia 2: " << percentuale2norm 
		<< "% Soglia 1: " << percentuale1norm << "% Soglia 0: " << percentuale0norm << "%\n";
    myfile << "DIFFERENZA Id: " << q << " Diff 4: " << percentuale4norm-percentuale4 << "% Diff 3: " << percentuale3norm-percentuale3 << 
	"% Diff 2: " << percentuale2norm-percentuale2 << "% Diff 1: " << percentuale1norm-percentuale1 << "% Diff 0: " 
		<< percentuale0norm-percentuale0 << "%\n";
    myfile << "MIGLIORAMENTO Id: " << q <<
		 " Soglia 4: " << (percentuale4norm-percentuale4)/percentuale4norm   * 100   << 
		"% Soglia 3: " << (percentuale3norm-percentuale3)/percentuale3norm   * 100   << 
		"% Soglia 2: " << (percentuale2norm-percentuale2)/percentuale2norm   * 100   << 
		"% Soglia 1: " << (percentuale1norm-percentuale1)/percentuale1norm   * 100   << 
		"% Soglia 0: " << (percentuale0norm-percentuale0)/percentuale0norm   * 100   << "%\n\n";
    myfile.close();
  }
  if (myfile2.is_open())
  {
   // myfile2 << q << " " << percentuale4-percentuale4norm << " " << percentuale3-percentuale3norm << 
//	" " << percentuale2-percentuale2norm << " " << percentuale1-percentuale1norm << " " 
//		<< percentuale0-percentuale0norm << "\n";
    myfile2  << q <<
		" " << (percentuale4norm-percentuale4)/percentuale4norm  * 100    << 
		" " << (percentuale3norm-percentuale3)/percentuale3norm  * 100    << 
		" " << (percentuale2norm-percentuale2)/percentuale2norm  * 100    << 
		" " << (percentuale1norm-percentuale1)/percentuale1norm  * 100    << 
		" " << (percentuale0norm-percentuale0)/percentuale0norm  * 100    <<"\n";
    myfile2.close();
  }		
 
}


