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
using namespace std; // OpenCV API is in the C++ "cv" namespace

//tokenizer
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
//returns the number of lines given a file
int number_of_lines(char* filename)
{
	FILE* myfile = fopen(filename, "r");
	int ch, number_of_lines = 0;

	do 
	{
	    ch = fgetc(myfile);
	    if(ch == '\n')
	    	number_of_lines++;
	} while (ch != EOF);

	// last line doesn't end with a new line!
	// but there has to be a line at least before the last line
	if(ch != '\n' && number_of_lines != 0) 
	    number_of_lines++;

	fclose(myfile);
	
	return number_of_lines;
}



//returns the number of training samples, the number of testing samples and the attributes per sample given the 2 files
int* find_parameters_from_csv(char* filename_train)
{
    int number_of_training_samples = 0;
    int number_of_attributes;
    int* results;  
    vector<string> attributes;

    FILE* f1 = fopen( filename_train, "r" );
    
    if( !f1 )
    {
        printf("ERROR: cannot read file %s\n",  filename_train);
        return 0; // all not OK
    }

    number_of_training_samples = number_of_lines(filename_train);

    std::ifstream input(filename_train);
    string line;

    getline( input, line );
    //fa il trim
    line.erase(line.find_last_not_of(" \n\r\t")+1);

    attributes = split(line, ',');
 
    number_of_attributes = attributes.size() - 1;

    results[0] = number_of_training_samples;
    results[1] = number_of_attributes;

    return results;
}

// loads the sample database from file (which is a CSV text file)

int read_data_from_csv(char* filename, Mat data, Mat classes,
                       int n_samples, int ATTRIBUTES_PER_SAMPLE)
{
    float tmp;

    // if we can't read the input file then return 0
    FILE* f = fopen( filename, "r" );
    if( !f )
    {
        printf("ERROR: cannot read file %s\n",  filename);
        return 0; // all not OK
    }

    // for each sample in the file
    #pragma omp parallel for 
    for(int line = 0; line < n_samples; line++)
    {

        // for each attribute on the line in the file

        for(int attribute = 0; attribute < (ATTRIBUTES_PER_SAMPLE + 1); attribute++)
        {

            if (attribute < ATTRIBUTES_PER_SAMPLE)
            {

                // first 4 elements (0-3) in each line are the attributes

                fscanf(f, "%f,", &tmp);
                data.at<float>(line, attribute) = tmp;
                // printf("%f,", data.at<float>(line, attribute));

            }
            else if (attribute == ATTRIBUTES_PER_SAMPLE)
            {

                // attribute 5 is the class label {0 ... 1}

                fscanf(f, "%f,", &tmp);
                classes.at<float>(line, 0) = tmp;
                // printf("%f\n", classes.at<float>(line, 0));

            }
        }
    }

    fclose(f);

    return 1; // all OK
}
