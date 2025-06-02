/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for Instability                                                                  ***/
/***                                                                                                 ***/
/***  ODE dependent instability, v1.0; Date: 1400/02/08                                              ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by DevC++ IDE on Windows                                ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use my last code in the moodle to write this code.
 */
 

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;

// Parameters and constants ============================
const float tmax = 15;                      // Final time
const float h = 1.0/8;                      // Time step, it is better to choose it as a power of 2.

// Variables ===========================================
float N1;									// for ODE with g(x,y) = -y^2
float N2;									// for ODE with g(x,y) = -y^(1/2)
int delta;    								// Number of intervals
ofstream Nt;                                // This stream maintains the results.

// Declarations ========================================
void init();                                // This function allocates the memory and initiates the variables.
void execute();                             // This function executes the algorithm.
void done();                                // This function deallocates the memory and file(s).

// Implementations =====================================

// Main routine =================================================================
int main() { 
    cout << "ODE dependent instability, v 1.0; Date: 1400/02/08" << endl;
    cout << "by Mehdi Bakhshipoor; Student ID#: 971315 \n" << endl;
    clock_t runtime = clock();
    init();
    execute();
    done();
    cout << "run time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;
}

// allocates the memory and initiates the variables =============================
void init() { 
    Nt.open("y(x).txt");
    
    N1 = N2 = 1.0;
    delta = tmax/h;
}

// deallocates the memory and file(s) ===========================================
void done() { Nt.close(); }

// executes the algorithm =======================================================
void execute() { 
	Nt << "time\t\t" << "for g(x,y) = -y^2\t" << "for g(x,y) = -y^(1/2)" << endl;		// copy tital in file

	for (int i=0; i<delta ; i++)  {
		
		N1 = N1*(1 - N1*h);										// The value of N1 at any particular time (at t = h*(i+1))
		N2 = N2 - h*sqrt(fabs(N2));								// The value of N2 at any particular time (at t = h*(i+1))
		Nt << h*(i+1) << "\t\t" << N1 << "\t\t" << N2 << endl;  // copy values in file
			
	}
}
