/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for a Random Walk (RW) code                                                      ***/
/***                                                                                                 ***/
/***  Random Walk, v1.0; Date: 1400/03/27                                                            ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by by DevC++ IDE on Windows                             ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in the moodle to write this code. My code is presented in the Openfile(), run and main() functions.
 */

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <ctime>

using namespace std;
int m,n,								// Numbers of RWs & steps.
	a,b;								// The value of the step length to the right, left.
	
double  *S,								// An array which is used to compute the ensemble average of 
										//  the position of the random walk in each step.
		*S2,                            // An array which is used to compute the ensemble average of the square of
                                        //  the position of the random walk in each step.
    	*S3,
    	*S4;
    	                 
void Openfile();						// This function reads variables (m,n,a,b) from a file.
void run();								// function for Calculating.

ofstream temp; 							// file to save <s> & <s^2> for each functions & position of one particle.
                                            
// Main routine ===================================================================
int main () {
  	cout << "Random Walk (RW), v1.00; Date: 1400/03/27\n"
       	 << "by Mehdi Bakhshipoor; Student ID#: 971315" << endl;
       
    clock_t runtime = clock();
    
    Openfile();
	run();
	
	temp.close();	
   
	cout << "\nrun time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;  
	cout << "Finish!" << endl;
} 

// Read files and insert numbers ==================================================
void Openfile () {
	
	ifstream fin ("data" , ios :: in ) ;
	
	float *data;
	data = new float [4];
	for (int i=0; i<4; i++) fin >> data[i];			// Save numbers from the file inside the array.

	fin.close();
	
	m = data[0];				// The 1st line in the file is Number of RWs.
	n = data[1];				// The 2nd line in the file is Number of steps.
	a = data[2];				// The 3rd line in the file is the step length to the forward.
	b = data[3];				// The 4th line in the file is the step length to the backward.
	 
    S  = new double [n];
    S2 = new double [n]; 
    S3 = new double [n];
    S4 = new double [n];
  
    cout << "\nParameters:\n"
         << "    m = "  << m  << "\tn = " << n
		 << "\n    a = " << a << "\tb = " << -b << endl;
} 

// Calculate average, average of the square and position of one random particle ===
void run(){
	srand(time(NULL));                      // Randomize the seed of C++ random number generator by the system time. It 
                                            // should be noted here that the time(NULL) returns the number of seconds 
                                            // elapsed since 00:00:00 (GMT), January 1, 1970. 
	int R = rand() %m;
	temp.open("temp.txt");
    temp << "Position of the " << R << "th particle" << endl;
    
    
	// Init ...
	for (int i=0; i<n; i++) 
		S[i] = S2[i] = S3[i] = S4[i] = 0;       // Zeros S[], S2[], S3[], S4[].
    
	// Execute ...
  	for (int j=0; j<m; j++) {                 	// Summation loop over ensembles
    	float x = 0;                            // The current position of the random walk
        
		for (int i=0; i<n; i++) {               // Dynamic of the j'th random walk
			float r = rand()/(1. + RAND_MAX);   // Floating-point random number in the range of [0.,1)
		
			if (r < 0.5) { x+=a;				// Move forward and backward
			} else x-=b;
		
	
			S[i]  += x; 						// Calculate the average value.
			S2[i] += x*x;						// Calculate the square of average value.
			S3[i] += x*x*x;
			S4[i] += x*x*x*x;
			
			if (j == R) 
				temp << "x (" << i+1 << ")=\t" << x << endl;	// Position of one random particle.
		} 
  	}
    
	// Done ...
  	// The following lines of the code output the 〈x²〉 to `temp.txt' file.
	temp << "\n\n\nValues for the calculate resulte (x+=a)\n\n";
	temp << "step\t"<< "<s>\t" << "<s^2>\t" << "<s^3>\t" << "<s^4>" << endl;   
	for(int i=1; i<n; i++)
		temp << i << "\t" << S[i]/m << "\t" << S2[i]/m << "\t" << S3[i]/m << "\t" << S4[i]/m << endl;	
}
