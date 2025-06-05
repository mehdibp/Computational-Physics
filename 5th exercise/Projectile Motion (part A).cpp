/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for a projectile motion - part A                                                 ***/
/***                                                                                                 ***/
/***  Projectile motion - part A, v1.0; Date: 1400/03/07                                             ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by DevC++ IDE on Windows                                ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in Moodle to write this code. My code is presented in the rho2(), rho3(), 
 * and run() and Openfile() functions.
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
 float m, g, A, C, v0, h;					// Values in the file.
 float vx0, vy0;

const float theta0 = 45. * M_PI / 180.;     // The initial altitude angle, θ₀ [rad]
const float X0     = 0.,                    // The initial position (X₀, Y₀) [m]
            Y0     = 0.;

// Variables
float t;                                    // The current time

// Global variables for simulating the motion without a drag force
float x, y,                                 // The components of the current position and
      vx, vy;                               // velocity.

// Global variables for simulating the motion with the drag force
float X, Y,                                 // The components of the current position and
      VX, VY;                               // velocity.

      
ofstream path;                              // This file stream holds the trajectory of the particle.

void Openfile();							// This function reads variables from a file and saves them in an array.
void init();                                // This function allocates the memory and initiates the variables.
void run();                                 // This function executes the algorithm.
void done();                                // This function deallocates the memory and file(s).

// Main routine =================================================================
int main() {
    cout << "projectile motion - part A, v1.0; Date: 1400/03/07" << endl;
    cout << "by Mehdi Bakhshipoor; Student ID#: 971315" << endl;
    
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
	data = new double [6];
	for (int i=0; i<6; i++)
		fin >> data[i];			// Save numbers from the file inside the array.

    fin.close();
	
    m = data[0];				// The 1st line in the file is mass [kg].
    g = data[1];				// The 2nd line in the file is gravitational acceleration [m/s²].
    A = data[2];				// The 3rd line in the file is cross-section [m²].
    C = data[3];				// The 4th line in the file is The dimensionless constant of the drag force.
    v0 = data[4];				// The 5th line in the file is the value of the time step written.
    h = data[5];				// The 5th line in the file is the value of the time step written.
} 

// This function allocates the memory and initiates the variables ===============
void init() {
    path.open("path.txt");
    
    // Initialize the global variables
    t = 0.;
    vx0 = v0 * cos(theta0),                 // The initial velocity (vx₀, vy₀) [m/s]
    vy0 = v0 * sin(theta0);

    x  = X0;                                // Initialize the position and
    y  = Y0;
    vx = vx0;                               // the velocity of a projectile motion without an air resistance.
    vy = vy0;
    
    X  = X0;                                // Initialize the position and
    Y  = Y0;
    VX = vx0;                               // the velocity of a motion with a drag force.
    VY = vy0;
}

// Deallocation of memory and file(s) ===========================================
void done() { path.close(); }


// The air density models =======================================================
inline float rho1(float y) { return 1.2 - (8e-5*y); }              // The density of air according to the linear model
inline float rho2(float y) { return 1.2*exp(-y*1.0e-4); }          // The density of air according to the isothermal model
inline float rho3(float y) { return 1.2*pow( (1-(0.0065/300*y)) , 2.5); } // The density of air according to the adiabatic model


// Executing the algorithm ======================================================
void run() {
	
	int c = 0;                              // Temporary counter
    path << "time" << "\tx" << "\t\ty" << "\t\tx_exact" << "\t\ty_exact" << "\t\tX" << "\t\tY for rho1" << endl;
  
    do {
        if ((c & 0x7f) == 0) {              // Send the result to the path file every 128 steps using the c 
                                            // counter. This trick reduces the size of the file while improving 
                                            // the appearance of the graph.
            float x_exact = X0 + vx0*t,
                  y_exact = Y0 + vy0*t - 0.5*(g*t*t),
                  vy_exact = vy0 - g*t;
            
            path << "\n" << t << '\t' 
                 << x << "\t\t" << y << "\t\t"	 	 // A numerical simulation result without a drag force
                 << x_exact << "\t\t" << y_exact;	 // An exact solution without a drag force   
            	 
                if (Y>=0)
                    path << "\t\t" << X << "\t\t" << Y;  // A numerical simulation result with a drag force
        }
        ++c;                           			     // Increase the temporary counter

        // The Euler method - without an air resistance
        x = x + h*vx;
        y = y + h*vy;
        vy = vy - h*g;
       
        // The Euler method - with the drag force
        float rho = rho1(Y);                // Temporary variables hold the density of air.
        // float rho = rho2(Y);
        // float rho = rho3(Y);
         
        X = X + h * VX; 
        Y = Y + h * VY; 
                 
        // Temporary variables hold the velocity of the particle and psi, respectively
        float V   = sqrt( VX*VX + VY*VY );
        float Psi = 0.5*(C*A/m) *rho*V *h;

		VX = VX*(1 - Psi);
		VY = VY - h*g - Psi*VY; 

        t += h;
    } while (y > 0);

    // Final point
    float x_exact = X0 + vx0 * t,
          y_exact = Y0 + vy0 * t - 0.5 * g * t*t;
        
    path << "\n" << t << '\t' 
        << x << "\t\t" << y << "\t\t"        // A numerical simulation result without an air resistance
        << x_exact << "\t\t" << y_exact;     // An exact solution without an air resistance  		 
		        
}

