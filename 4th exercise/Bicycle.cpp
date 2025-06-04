/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for Bicycle                                                                      ***/
/***                                                                                                 ***/
/***  Bicycle, v1.0; Date: 1400/02/08                                                                ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by DevC++ IDE on Windows                                ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in Moodle to write this code. My code is presented in the run() & Openfile() function.
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <bits/stdc++.h> 

using namespace std;

// Note: The following algorithm was developed in the reduced units system.

// Constants and Parameters
const float tmax = 60;                       // The maximum time
const float x0   = 0.;
const float v0   = 0.;

// Variables
float delta;                                // Number of intervals
float x,                                    // For simulating the motion of bicycle without a drag force
      v,
      v2;                                   // v2 = v ^ 2
float X,                                    // For simulating the motion of bicycle with the drag force
      V,
      V2;                                   // v2 = v ^ 2
float Vt;									// final speed in page 6-2
 
float P, A, ro, m, h;				     	// Values in the file.

ofstream Res;                               // This file stream holds the results.

void Openfile();							// This function reads variables from a file and saves them in an array.
void init();                                // This function allocates the memory and initiates the variables.
void run();                                 // This function executes the algorithm.
void done();                                // This function deallocates the memory and file(s).

// Main routine =================================================================
int main() {
    cout << "Bicycle, v 1.0; Date: 1400/02/08" << endl;
    cout << "by Mehdi Bakhshipoor; Student ID#: 971315 \n" << endl;
    
    Openfile();
    
    clock_t runtime = clock();
    init();
    run();
    
    done();
    cout << "run time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;
}


// Read files and insert numbers ================================================
void Openfile () {
	
	ifstream fin ("data" , ios :: in );
	
	double *data;
	data = new double [5];
	for (int i=0; i<5; i++)
		fin >> data[i];			// Save numbers from the file inside the array.

    fin.close();
	
    P = data[0];				// The 1st line in the file is P(W).
    A = data[1];				// The 2nd line in the file is A(m^2).
    ro = data[2];				// The 3rd line in the file is ro(kg/m^3).
    m = data[3];				// The 4th line in the file is m(kg).
    h = data[4];				// The 5th line in the file is the value of the time step written.
} 	


// This function allocates the memory and initiates the variables ===============
void init() {
    Res.open("Result.txt");
    delta = tmax/h;							  // Number of steps.

    // For simulating the motion of bicycle without a drag force.
    x = x0;
    v = v0;
    v2 = v0 * v0;
    
    // For simulating the motion of bicycle with the drag force.
    X = x0;
    V = v0;
    V2 = v0 * v0;  
	 
}

// Deallocation of memory and file(s) ===========================================
void done() { Res.close(); }

// Executing the algorithm ======================================================
void run() {
	
    float U0_3 = 2*P/(ro*A);				    // final speed V_c^3
    float U0 = pow(U0_3 , 1.0/3.0);		        // final speed V_c in page 6-6
    float T0 = 2*m/(ro*A*U0);				    // t = T0 * t_tilde
    float L0 = 2*m/(ro*A);					    // x = L0 * x_tilde
 
    float h_tilde = h/T0;
	
	
	Res << "time\t\t" << "x-without air\t" << "v-without air\t" 
   		<< "V(t)\t\t"   << "X-With air\t"    << "V-With air"
   		<<  endl;				// copy tital in file
   
    for (int i=0; i<=delta ; i++)  {
   	    Vt = sqrt(v0*v0 + 2*(h_tilde*i));				    // An exact solution without a drag force

        Res << h*i <<  "\t\t" 
            << L0*x << "\t\t" << U0*v << "\t\t"     		// A numerical calculation without a drag force
            << U0*Vt << "\t\t"  					 		// An exact solution without a drag force 
            << L0*X << "\t\t" << U0*V << endl;     		    // A numerical calculation with the drag force
      	

        // The Euler method for no air resistance ----------------------
        x = x + h_tilde*v; 
        v2 = v2 + 2*h_tilde;
        v  = sqrt(v2);

        // The Euler method for when we also have air resistance -------
        X = X + h_tilde*V;
        V2 = V2 + 2*h_tilde*(1-(V*V*V));
        V = sqrt(V2);
	}

}
