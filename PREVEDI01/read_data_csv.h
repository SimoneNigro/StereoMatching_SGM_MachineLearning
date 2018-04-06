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


using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std; // OpenCV API is in the C++ "cv" namespace

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

std::vector<std::string> split(const std::string &s, char delim);

int number_of_lines(char* filename);

int* find_parameters_from_csv(char* filename_train);

int read_data_from_csv(char* filename, Mat data, Mat classes, int n_samples, int ATTRIBUTES_PER_SAMPLE);
