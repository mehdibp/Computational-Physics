/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for Radioactive decay                                                            ***/
/***                                                                                                 ***/
/***  Radioactive decay, v1.0; Date: 1400/01/28                                                      ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by DevC++ IDE on Windows                                ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in the moodle to write this code. My code was presented in the execute() function.
 */
 

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;

// Parameters and constants ============================
const float tmax = 10;                      // Final time
const float h = 1.0/8;                      // Time step, it is better to choose it as a power of 2.

// Variables ===========================================
float N_t;
int delta;    								// Number of intervals
ofstream Nt;                                // This stream maintains the results.

// Declarations ========================================
void init();                                // This function allocates the memory and initiates the variables.
void execute();                             // This function executes the algorithm.
void done();                                // This function deallocates the memory and file(s).

// Implementations =====================================

// Main routine =================================================================
int main() { 
    cout << "Radioactive decay, v 1.0; Date: 1400/01/28" << endl;
    cout << "by Mehdi Bakhshipoor; Student ID#: 971315 \n" << endl;
    clock_t runtime = clock();
    init();
    execute();
    done();
    cout << "run time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;
}

// allocates the memory and initiates the variables =============================
void init() { 
    Nt.open("N(t).txt");
    
    N_t = 1.0;
    delta = tmax/h;
}

// deallocates the memory and file(s) ===========================================
void done() { Nt.close(); }

// executes the algorithm =======================================================
void execute() { 
	Nt << "time\t\t" << "Numerical answer\t" << "Analysis answer" << endl;			// copy tital in file
		 
	for (int i=0; i<delta ; i++)  {
		N_t = pow((1-h),i+1);							// The value of N at any particular time (at t = h*(i+1))
		Nt << h*(i+1) << "\t\t" << N_t << "\t\t" << exp(-h*(i+1)) << endl;			// copy in file	
	}
                  
}