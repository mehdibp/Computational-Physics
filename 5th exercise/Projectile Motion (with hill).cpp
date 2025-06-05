/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for a projectile motion (with hill)                                              ***/
/***                                                                                                 ***/
/***  Projectile motion (with hill), v1.0; Date: 1401/03/07                                          ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by DevC++ IDE on Windows                                ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in Moodle to write this code. My code is presented in the rho2(), rho3(), 
 * and R() and Openfile() and run() and R_exact_hill and R_dragless_hill functions.
 */


#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <ctime>

using namespace std;

//â€Ž Constants and Parameters
 float m, g, A, C, v0, h;					// Values in the file.
 
const float X0     = 0.,                    // The initial position (Xâ‚ Yâ) [m]
            Y0     = 0.;

// Global variables for simulating the motion without a drag force
float x, y,                                 // The current position and
      vx, vy,
	  x1 , vx1,
	  y2 , vy1,
	  x2 , vx2;                             // velocity.

// Global variables for simulating the motion with the drag force
float X, Y,                                 // The current position and
      VX, VY;                               // velocity.

      
ofstream res;                               // This file stream holds the result as R vs. theta0

void Openfile();							// This function reads variables from a file and saves them in an array.
void init();                                // This function allocates the memory and initiates the variables.
void init(float theta0);                    // This function initialize a simulation of projectile motion with the initial altitude angle Î¸â‚€ [rad]
void run();                                 // This function executes the algorithm.
void done();                                // This function deallocates the memory and file(s).
float R_dragless_hill(float theta0);
float R_exact_hill(float theta0);

// Main routine =================================================================
int main() {
    cout << "projectile motion - part B, v1.0; Date: 1400/03/07" << endl;
    cout << "by Mehdi Bakhshipoor; Student ID#: 971315" << endl;
	
	Openfile();

    clock_t runtime = clock();
    init();
    run();
    done();
    cout << "\nrun time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;
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
    g = data[1];				// The 2nd line in the file is gravitational acceleration [m/sÂ²].
    A = data[2];				// The 3rd line in the file is cross-section [mÂ²].
    C = data[3];				// The 4th line in the file is The dimensionless constant of the drag force.
    v0 = data[4];				// The 5th line in the file is the value of the time step written.
    h = data[5];				// The 5th line in the file is the value of the time step written.
} 

// This function allocates the memory and initiates the variables ===============
void init() { res.open("result(hill).txt"); }

// This function initialize a simulation of projectile motion with the initial altitude angle Î¸â‚€ [rad] ==
void init(float theta0) {
    float vx0   = v0 * cos(theta0);       // The initial velocity
    float vy0   = v0 * sin(theta0);

    // Initialize the global variables
    x  = X0;                                // Initialize the position and
    y  = Y0;
    vx = vx0;                               // the velocity of a projectile motion without an air resistance
    vy = vy0;
    
    X  = X0;                                // Initialize the position and
    Y  = Y0;
    VX = vx0;                               // the velocity of a motion with a drag force.
    VY = vy0;
    
}

// Deallocation of memory and file(s) ===========================================
void done() { res.close(); }


// The air density models =======================================================
inline float rho1(float y) { return 1.2 - (8e-5*y); }              // The density of air according to the linear model
inline float rho2(float y) { return 1.2*exp(-y*1.0e-4); }          // The density of air according to the isothermal model
inline float rho3(float y) { return 1.2*pow( (1-(0.0065/300*y)) , 2.5); } // The density of air according to the adiabatic model

//==============================================================================
// The range R of a projectile on the ground level is given by
// R(Î¸â‚€) = vâ‚€Â² sin(2Î¸â‚€) / g, 
// when the air resistance is negligible. The following function returns 
// this exact solution.
float R_exact(float theta0) { return v0*v0 * sin(2*theta0)/g; }

// The computed R(Î¸â‚€) of a projectile motion without an air resistance =======
float R_dragless(float theta0) {
    do {
        // The Euler method - without an air resistance
        x = x + h*vx; 
        y = y + h*vy; 
        
        vy = vy - h*g;

    } while (y > 0);
    return x;
}

// The computed R(Î¸â‚€) of realistic projectile motion with drag force =========
float R_drag(float theta0) {
	init(theta0);
    do {
        
        // Temporary variables hold the density of air and the velocity of the particle, respectively
        float rho = rho2(Y);                
        // float rho = rho2(Y);
        // float rho = rho3(Y);

		// The Euler method - with the drag force
        X = X + h * VX; 
        Y = Y + h * VY; 
      
        // Temporary variables hold the velocity of the particle and psi, respectively
        float V   = sqrt( VX*VX + VY*VY );
        float Psi = 0.5*(C*A/m) *rho*V *h;

		VX = VX*(1 - Psi);
		VY = VY - h*g - Psi*VY;
		

        if (Y > 10000.) Y = 0;     
	
    } while (Y > 0);
    return X;
}

// Executing the algorithm ======================================================
void run() {

	res << "theta\t\t" << "R_drog\t\t" << "R_drogless\t" << "R_exact" << endl;	// copy tital in file
	
    for (float theta0 = 0; theta0 < 0.51*M_PI; theta0 += 0.01) {
        init(theta0);
        res << theta0 * 180 / M_PI << "\t\t" << R_drag(theta0) 
                                   << "\t\t" << R_dragless(theta0) 
                                   << "\t\t" << R_exact(theta0)
								   << "\t\t" << R_dragless_hill(theta0)
								   << "\t\t" << R_exact_hill(theta0) << endl;		// copy in file
    }
    
    float RMax = 0.;						// Maximum range
	float theta_Max = 0.;					// Angle with the Maximum range
    
    for (float theta0 = 0; theta0 < 0.51*M_PI; theta0 += 0.01) {
 		init(theta0);
			
    	if(R_drag(theta0) > RMax){
    		RMax = R_drag(theta0);
			theta_Max = theta0;
		}
	}
   
	cout << "\nAngle with the maximum range = " << theta_Max * 180 / M_PI << endl;
    
}



// ==============================================================================
float R_exact_hill(float theta0) {
	float y_ex , ww;
	init(theta0);
	 do {
        x  += h * vx;
        y_ex = (x*tan(theta0)) - (x*x*g)/(2*vx*vx);
        ww = (x*tan(M_PI/6));
        
    } while ( y_ex > ww);
 	return x/(cos(M_PI/6));
 	
}

// ==============================================================================
float R_dragless_hill(float theta0) {
 	float w;
 	init(theta0);
    do {
        // The Euler method - without an air resistance - and we have hill
        x = x + h*vx; 
        y = y + h*vy; 
        
        vy = vy - h*g;
		
		w = x*tan(M_PI/6);
		
    } while (y > w);
    return x/(cos(M_PI/6));
}
