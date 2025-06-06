/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for the solar system                                                             ***/
/***                                                                                                 ***/
/***  Solar system, v1.0; Date: 1400/03/09                                                           ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by DevC++ IDE on Windows                                ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in Moodle to write this code. My code is presented in the Openfile() and run() functions.
 */


#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;

// Constants and Parameters
const float GMS   = 4.*M_PI*M_PI;           // G multiple the mass of Sun [AU³/yr²]
const float GMS_SI= 3.38e18;			    // ~= 1AU^3/1yr^2 = (1.5*10^11)^3/(365.25*24*3600)^2

float MJOMS, R, T, h;					    // in file...

float xi_no, yi_no, vxi_no, vyi_no;		    // initial Earth's position and velocity without considering the rest of the planets
float xi, yi, vxi, vyi;					    // initial position and velocity of Earth
float Xi, Yi, Vxi, Vyi;					    // initial position and velocity of Jupiter
 

// Variables
float t;                                    // The current time [yr]

// Global variables
float x_no, y_no,							// The components of the current position [AU] and
	  vx_no, vy_no,							//  velocity of Earth without considering the rest of the planets [AU/yr].
	  x, y,                                 // The components of the current position [AU] and
      vx, vy,                               // velocity of Earth [AU/yr].
      X, Y,                                 // The components of the current position {AU] and
      Vx, Vy;                               // velocity of Jupiter {AU/yr].
      
ofstream path;                              // This file stream holds the trajectory of Earth and Jupiter.

void Openfile();							// This function reads variables from a file and saves them in an array.
void init();                                // This function allocates the memory and initiates the variables.
void run();                                 // This function executes the algorithm.
void done();                                // This function deallocates the memory and file(s).

// Main routine =================================================================
int main() {
    cout << "Solar system, v1.0; Date: 1400/03/09\n"
         << "by Mehdi Bakhshipoor; Student ID#: 971315\n";
         
    Openfile();
                         
    clock_t runtime = clock();
    init();
    run();
    done();
    cout << "run time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;
}


// Read files and insert numbers ================================================
void Openfile () {
	
	ifstream fin ("data" , ios :: in ) ;
	
	double *data;
	data = new double [5];
	for (int i=0; i<5; i++)
		fin >> data[i];			// Save numbers from the file inside the array.

    fin.close();
	
    MJOMS = data[0];			// The 1st line in file is M_j/M_S.
    R = data[1];				// The 2nd line in file is the orbital radius of Jupiter [AU].
    T = data[2];				// The 3rd line in file is the period of orbital motion of Jupiter [yr].
    h = data[3];				// The 5th line in file is the value of the time step written.
    
} 	

// This function allocates the memory and initiates the variables ===============
void init() {
    path.open("path.txt");
    
    // initial position and velocity of jupiter    
    Xi  = R,                         // The components of the initial position of Jupiter (X₀, Y₀) [AU]
    Yi  = 0.;
    Vxi = 0.,                        // The initial components of the velocity of Jupiter (Vx₀, Vy₀) [Au/yr]
    Vyi = 2*M_PI*R/T;
        
    // initial position and velocity of earth
    xi  = 1,                        // The components of the initial position of Earth (x₀, y₀) [AU]
    yi  = 0;
    vxi = 0,                        // The initial components of the velocity of Earth (vx₀, vy₀) [Au/yr]
    vyi = 2*M_PI;
    
    xi_no  = 1,                     // The components of the initial position of Earth...
    yi_no  = 0;						// ...without considering the rest of the planets (x₀, y₀) [AU]
    vxi_no = 0,                     // The initial components of the velocity of Earth...
    vyi_no = 2*M_PI;				// ...without considering the rest of the planets (vx₀, vy₀) [Au/yr]

    // Initialize the global variables
    t = 0.;
    
    x_no  = xi_no;                          // Initialize the position and velocity of Earth without other the planets
    y_no  = yi_no;
    vx_no = vxi_no;
    vy_no = vyi_no;

    x  = xi;                                // Initialize the position and velocity of Earth
    y  = yi;
    vx = vxi;
    vy = vyi;
    
    X  = Xi;                                // Initialize the position and and velocity of Jupiter
    Y  = Yi;
    Vx = Vxi;
    Vy = Vyi;
}

// Deallocation of memory and file(s) ===========================================
void done() { path.close(); }


// Executing the algorithm ======================================================
void run() {
	
	path << "x_no\t\t" << "y_no\t\t" << "x\t\t" << "y\t\t" << "X\t\t" << "Y" << endl;
	
    do {
        path << GMS_SI*x_no << "\t" << GMS_SI*y_no << "\t"			// The position and velocity of Earth without other the planets
			 << GMS_SI*x << "\t" << GMS_SI*y << "\t"  				// The position of Earth and 
             << GMS_SI*X << "\t" << GMS_SI*Y << endl;    		    // Jupiter in the orbital motion in the reference framework of Sun
             	

        // The Euler-Cromer method
         float r_no = sqrt(x_no*x_no + y_no*y_no);
         float r = sqrt(x*x + y*y);
         float rEJ = sqrt((x-X)*(x-X) + (y-Y)*(y-Y));
         float R = sqrt(X*X + Y*Y);
         
         
		vx_no -= GMS*h*x_no/pow(r_no,3);
		vy_no -= GMS*h*y_no/pow(r_no,3);
		
		vx -= GMS*h*((x/pow(r,3)) + MJOMS*(x - X)/pow(rEJ,3));
		vy -= GMS*h*((y/pow(r,3)) + MJOMS*(y - Y)/pow(rEJ,3));
		Vx -= GMS*h*X/pow(R,3);
		Vy -= GMS*h*Y/pow(R,3);

       
    	x_no += h * vx_no;
    	y_no += h * vy_no;

        x  += h * vx; 
        y  += h * vy; 

        X  += h * Vx; 
        Y  += h * Vy; 
        
        t += h;
    } while (t <= T);
}
