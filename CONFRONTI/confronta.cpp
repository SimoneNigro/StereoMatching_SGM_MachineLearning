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

void confronta(IplImage* DispMap, IplImage* Normale, IplImage* Scelti, IplImage* Somma4, IplImage* Somma4Pesata, IplImage* GroundTruth, char *q)
{
    
    int w = DispMap->width;
    int h = DispMap->height;

    int corretti4 = 0;    int corretti4norm = 0;
    int corretti3 = 0;    int corretti3norm = 0;
    int corretti2 = 0;    int corretti2norm = 0;
    int corretti1 = 0;    int corretti1norm = 0;
    int corretti0 = 0;    int corretti0norm = 0;

    int corretti4somma4 = 0;    int corretti4somma4pes = 0;
    int corretti3somma4 = 0;    int corretti3somma4pes = 0;
    int corretti2somma4 = 0;    int corretti2somma4pes = 0;
    int corretti1somma4 = 0;    int corretti1somma4pes = 0;
    int corretti0somma4 = 0;    int corretti0somma4pes = 0;

    int corretti4scelti = 0;    
    int corretti3scelti = 0;    
    int corretti2scelti = 0;    
    int corretti1scelti = 0;   
    int corretti0scelti = 0;    

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
		//scelti
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Scelti, uchar, i, j)) <= 4 )
				{	
					corretti4scelti++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Scelti, uchar, i, j)) <= 3 )
				{	
					corretti3scelti++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Scelti, uchar, i, j)) <= 2 )
				{	
					corretti2scelti++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Scelti, uchar, i, j)) <= 1 )
				{	
					corretti1scelti++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Scelti, uchar, i, j)) == 0 )
				{	
					corretti0scelti++;				
				}
		//somma4
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4, uchar, i, j)) <= 4 )
				{	
					corretti4somma4++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4, uchar, i, j)) <= 3 )
				{	
					corretti3somma4++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4, uchar, i, j)) <= 2 )
				{	
					corretti2somma4++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4, uchar, i, j)) <= 1 )
				{	
					corretti1somma4++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4, uchar, i, j)) == 0 )
				{	
					corretti0somma4++;				
				}
		//somma4pesata
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4Pesata, uchar, i, j)) <= 4 )
				{	
					corretti4somma4pes++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4Pesata, uchar, i, j)) <= 3 )
				{	
					corretti3somma4pes++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4Pesata, uchar, i, j)) <= 2 )
				{	
					corretti2somma4pes++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4Pesata, uchar, i, j)) <= 1 )
				{	
					corretti1somma4pes++;				
				}
				if ( abs( CV_IMAGE_ELEM(GroundTruth, uchar, i, j) - CV_IMAGE_ELEM(Somma4Pesata, uchar, i, j)) == 0 )
				{	
					corretti0somma4pes++;				
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

    float percentuale4scelti =  ((float) (validi - corretti4scelti)) / ((float)validi) * 100;
    float percentuale3scelti =  ((float) (validi - corretti3scelti)) / ((float)validi) * 100;
    float percentuale2scelti =  ((float) (validi - corretti2scelti)) / ((float)validi) * 100;
    float percentuale1scelti =  ((float) (validi - corretti1scelti)) / ((float)validi) * 100;
    float percentuale0scelti =  ((float) (validi - corretti0scelti)) / ((float)validi) * 100;

    float percentuale4somma4 =  ((float) (validi - corretti4somma4)) / ((float)validi) * 100;
    float percentuale3somma4 =  ((float) (validi - corretti3somma4)) / ((float)validi) * 100;
    float percentuale2somma4 =  ((float) (validi - corretti2somma4)) / ((float)validi) * 100;
    float percentuale1somma4 =  ((float) (validi - corretti1somma4)) / ((float)validi) * 100;
    float percentuale0somma4 =  ((float) (validi - corretti0somma4)) / ((float)validi) * 100;

    float percentuale4somma4pes =  ((float) (validi - corretti4somma4pes)) / ((float)validi) * 100;
    float percentuale3somma4pes =  ((float) (validi - corretti3somma4pes)) / ((float)validi) * 100;
    float percentuale2somma4pes =  ((float) (validi - corretti2somma4pes)) / ((float)validi) * 100;
    float percentuale1somma4pes =  ((float) (validi - corretti1somma4pes)) / ((float)validi) * 100;
    float percentuale0somma4pes =  ((float) (validi - corretti0somma4pes)) / ((float)validi) * 100;
/*
  ofstream myfile ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/confrontiSOMMA4PES", ios::out | ios::app );
  ofstream myfile2 ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/differenzeSOMMA4PES", ios::out | ios::app );
  ofstream myfile3 ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/mediaErroreML", ios::out | ios::app );
  ofstream myfile4 ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/mediaErroreSGM", ios::out | ios::app );
  ofstream myfile5 ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/mediaDifferenze", ios::out | ios::app );
  if (myfile.is_open())
  {
    myfile << "ERRORE SGM_ML Id:" << q << 
		"\tSoglia 4:\t" << percentuale4 << 
		"%\tSoglia 3:\t" << percentuale3 << 
		"%\tSoglia 2:\t" << percentuale2 << 
		"%\tSoglia 1:\t" << percentuale1 << 
		"%\tSoglia 0:\t" << percentuale0 << "%\n";

    myfile << "ERRORE SGM    Id:" << q << 
		"\tSoglia 4:\t" << percentuale4norm << 
		"%\tSoglia 3:\t" << percentuale3norm << 
		"%\tSoglia 2:\t" << percentuale2norm << 
		"%\tSoglia 1:\t" << percentuale1norm << 
		"%\tSoglia 0:\t" << percentuale0norm << "%\n";

    myfile << "DIFFERENZA    Id:" << q << 
		"\tDiff 4:   \t" << percentuale4norm-percentuale4 << 
		"%\tDiff 3:  \t" << percentuale3norm-percentuale3 << 
		"%\tDiff 2:  \t" << percentuale2norm-percentuale2 << 
		"%\tDiff 1:  \t" << percentuale1norm-percentuale1 << 
		"%\tDiff 0:  \t" << percentuale0norm-percentuale0 << "%\n";

    myfile << "MIGLIORAMENTO Id:" << q <<
		 "\tSoglia 4:\t" << (percentuale4norm-percentuale4)/percentuale4norm   * 100   << 
		"%\tSoglia 3:\t" << (percentuale3norm-percentuale3)/percentuale3norm   * 100   << 
		"%\tSoglia 2:\t" << (percentuale2norm-percentuale2)/percentuale2norm   * 100   << 
		"%\tSoglia 1:\t" << (percentuale1norm-percentuale1)/percentuale1norm   * 100   << 
		"%\tSoglia 0:\t" << (percentuale0norm-percentuale0)/percentuale0norm   * 100   << "%\n\n";
  
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
   if (myfile3.is_open())
  {
    myfile3  << q <<
		" " << percentuale4    << 
		" " << percentuale3    << 
		" " << percentuale2    << 
		" " << percentuale1    << 
		" " << percentuale0    <<"\n";
    myfile3.close();
  }
  if (myfile4.is_open())
  {
    myfile4  << q <<
		" " << percentuale4norm    << 
		" " << percentuale3norm    << 
		" " << percentuale2norm    << 
		" " << percentuale1norm    << 
		" " << percentuale0norm    <<"\n";
    myfile4.close();
  }
  if (myfile5.is_open())
  {
    myfile5  << q <<
		" " << percentuale4norm-percentuale4    << 
		" " << percentuale3norm-percentuale3    << 
		" " << percentuale2norm-percentuale2    << 
		" " << percentuale1norm-percentuale1    << 
		" " << percentuale0norm-percentuale0    <<"\n";
    myfile5.close();
  }*/

//faccio un csv con id, errore somma8pes nelle 5 soglie, errore scelti 5 soglie, errore somma4 5 soglie, errore somma4pes 5 soglie, errore sgm 5 soglie
  ofstream risultati ("/home/simo/Dropbox/TESI/PREVEDI 01/risultanti/risultati", ios::out | ios::app );
   if (risultati.is_open())
  {
    risultati  << q << //id
		";" << percentuale4    << 
		";" << percentuale3    << 
		";" << percentuale2    << 
		";" << percentuale1    << 
		";" << percentuale0    << //fine somma8pes
		";" << percentuale4scelti    << 
		";" << percentuale3scelti    << 
		";" << percentuale2scelti    << 
		";" << percentuale1scelti    << 
		";" << percentuale0scelti    << //fine sscelti
		";" << percentuale4somma4    << 
		";" << percentuale3somma4    << 
		";" << percentuale2somma4    << 
		";" << percentuale1somma4    << 
		";" << percentuale0somma4    << //fine somma4
		";" << percentuale4somma4pes    << 
		";" << percentuale3somma4pes    << 
		";" << percentuale2somma4pes    << 
		";" << percentuale1somma4pes    << 
		";" << percentuale0somma4pes    << //fine somma4pes
		";" << percentuale4norm    << 
		";" << percentuale3norm    << 
		";" << percentuale2norm    << 
		";" << percentuale1norm    << //fine sgm
		";" << percentuale0norm    << "\n";
    risultati.close();
  }

}


