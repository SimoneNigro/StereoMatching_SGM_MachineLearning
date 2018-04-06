//Rispetto al codice che trovi qui--> http://breckon.eu/toby/teaching/ml/examples/c++/opticaldigits_ex/randomforest.cpp
//non devi definire in maniera statica il numero di training samples e di attributi, ho aggiungo qualche funzioncina per automatizzarlo
//in più, qui la random forest viene usata per la regressione
#include <opencv/cv.h>       
#include <opencv/ml.h>		  

using namespace cv; 
using namespace std; 
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>

//tokenizer
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

//funzione split
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

//restituisce il numero di linee di un fle
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



//restituisce il numero di training samples e gli attributi per sample dato il file di train
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

//apre il csv di train e carica i vari dati nelle Mat per poterli usare all'interno del programma
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

    for(int line = 0; line < n_samples; line++)
    {

        // for each attribute on the line in the file

        for(int attribute = 0; attribute < (ATTRIBUTES_PER_SAMPLE + 1); attribute++)
        {
            if (attribute < ATTRIBUTES_PER_SAMPLE)
            {

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

/******************************************************************************/

int main( int argc, char** argv )
{
    // lets just check the version first

    printf ("OpenCV version %s (%d.%d.%d)\n",
            CV_VERSION,
            CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION);
    
    if(argc != 2)
    {
     	printf("Usage: %s file_training\n", argv[0]);
        exit(0);
    }

//leggo dal csv il numero di training samples e di attributi per sample
    int* results = find_parameters_from_csv(argv[1]);
    
    int NUMBER_OF_TRAINING_SAMPLES = results[0] - 1;
    int ATTRIBUTES_PER_SAMPLE = results[1];

    printf("N° of training samples: %d \nN° of attributes: %d \n", NUMBER_OF_TRAINING_SAMPLES,ATTRIBUTES_PER_SAMPLE);

    // define training data storage matrices (one for attribute examples, one
    // for classifications)

    Mat training_data = Mat(NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE, CV_32FC1);
    Mat training_classifications = Mat(NUMBER_OF_TRAINING_SAMPLES, 1, CV_32FC1);

    // define all the attributes as numerical
    // alternatives are CV_VAR_CATEGORICAL or CV_VAR_ORDERED(=CV_VAR_NUMERICAL)
    // that can be assigned on a per attribute basis

    Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
    var_type.setTo(Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

//Dal momento che ci interessa la regressione, devo assegnare questo tipo 
    var_type.at<uchar>(ATTRIBUTES_PER_SAMPLE, 0) = CV_VAR_ORDERED;
    
    double result; // value returned from a prediction

    // load training and testing data sets

    if (read_data_from_csv(argv[1], training_data, training_classifications, NUMBER_OF_TRAINING_SAMPLES, ATTRIBUTES_PER_SAMPLE))
    {
        // define the parameters for training the random forest (trees)
//i parametri ottimali sono stati trovati facendo prove su prove, in generale, non esagerare con il numero di alberi o i tempi diventano infiniti

        CvRTParams params = CvRTParams(25, // max depth era 15
                                       1100, // min sample count era 5 poi 10 poi 100
                                       0.0001f, // regression accuracy: N/A here
                                       false, // compute surrogate split, no missing data
                                       2, // max number of categories (use sub-optimal algorithm for larger numbers) la regressione non lo usa
                                       NULL, // the array of priors //era "priors", vettore di float con tutti 1 ho messo io NULL
                                       true,  // calculate variable importance 
                                       12,       // number of variables randomly selected at node and used to find the best split(s). era 4 di base
                                       10,	 // max number of trees in the forest
                                       0.0001f,				// forrest accuracy
                                       CV_TERMCRIT_ITER |	CV_TERMCRIT_EPS // termination cirteria
                                      );

        // train random forest classifier (using training data)

        printf( "\nUsing training database: %s\n\n", argv[1]);
        CvRTrees* rtree = new CvRTrees;

        rtree->train(training_data, CV_ROW_SAMPLE, training_classifications,
                     Mat(), Mat(), var_type, Mat(), params);

// salva la foresta
	rtree->save("foresta.xml");

	//legge l'importanza data a ciascun attributo
        Mat var_importance = rtree->get_var_importance();
	cout << "Importanza = " << endl << " " << var_importance << endl << endl;


        // all OK : main returns 0

        return 0;
    }

    // not OK : main returns -1

    return -1;
}
/******************************************************************************/

