/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template for a molecular dynamics code - part 1                                                ***/
/***                                                                                                 ***/
/***  Molecular dynamics - part 1, v1.0; Date: 1399/03/27                                            ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in the C++ by DevC++ IDE on Windows.                           ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in the moodle to write this code. My code is presented in the 
 * Openfile(), init(), scale(), update(), PB(), RD(), Acc() and LJForce() functions.
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include "utils.h"                          // Our private utility library

using namespace std;

const float kB = 1.38E-23;                  // Boltzmann constant [J/K]
const float eV = 1.6E-19;                   // electronvolt [J]
const float Da = 1.6E-27;                   // The mass of an atom (atomic mass) is often expressed in the Dalton unit. 
                                            // According to the definition, 1 Dalton is defined as ¹/₁₂ of the mass of 
                                            // a single ¹²C atom at rest [kg].
// Constants and Parameters ========================
const float mAr     = 40 * Da;              // Mass of Argon atom [kg]
const float sigma   = 3.40e-10;             // σ of the Lennard-Jones potential; σ = 3.40 Å [m].
const float epsilon = 117 * kB;             // ɛ of the Lennard-Jones potential [J].


// <===  Units in the reduced units system  ===>
const float ul = sigma;                        // unit of length; l*
const float um = mAr;                          // unit of length; m*
const float ut = sqrt(mAr*sqr(sigma)/epsilon); // unit of time; t*
const float uE = epsilon;                      // unit of energy; ɛ
const float uv = sqrt(epsilon/mAr);            // unit of velocity; v*
const float uT = epsilon/kB;                   // unit of temperature; T*

// <===  MD parameters  ===>
int N , L;  
float T0, h, t_max;
int A;


// Variables =======================================
float t;                                    // The current time

float  *x_0, *y_0,							// The first position  
	   *x,  *y,            	      		    // The position
       *vx, *vy,                   	    	// velocity and
       *ax, *ay;                  	        // acceleration of particles.
      
ofstream path;                              // This file stream holds the trajectory of the particle in the simulation box.
ofstream res;                               // This file stream holds the result of the MD simulation.
ofstream ft;								// This file stream holds the particle displacement from the starting point.


// Functions and subroutine ========================
void Openfile();							// This function reads variables from a file and saves them in an array.
void init();                                // allocates the memory and initiates the variables.
void scale(float T);                        // scales the velocities of the particles relative to the temperature.
void done();                                // deallocates the memory and file(s).

void run();                                 // executes the algorithm.
void Update();                              // updates particles according to the velocity Verlet algorithm.
float PB(float x);                          // returns the position of the particle relative to the simulation 
                                            // box based on the periodic boundary conditions.

void Acc(float* x,  float* y,               // Acc() calculates and returns the accelerator of particles according to 
         float* ax, float* ay);             // the position of particles. Here, the star symbols (*) in the definition 
                                            // of arguments mean that those are pointers to the floating-point values.
float LJForce(float  dx, float  dy,         // Find the force from Lennard-Jones potential. The symbol (&) in the definition 
              float &fx, float &fy);        // of arguments indicates 'fx' and 'fy' are the references to the floating-point 
                                            // variables. So changing them in the function is applied to the variables which 
                                            // are used when calling the function.
float RD(float delta);                      // The relative distance of particles in PBC

void ExportPath();                          // Export the current position of particles to the path stream.
float EK();                                 // EK() returns the kinetic energy of the system per atom [ɛ].
inline float Temp();                        // Temp() returns the temperature [T*].


//===============================================================================
int main() {
    cout << "MD - part 1, v1.0; Date: 1400/03/20" << endl;
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
	
	ifstream fin ("data.txt" , ios :: in ) ;
	
	double *data;
	data = new double [5];
	for (int i=0; i<5; i++)
		fin >> data[i];			// Save numbers from the file inside the array.

    fin.close();
	
    N  = data[0];				// The 1st line in the file is number of particles (Argon atoms).
    L  = data[1];				// The 2nd line in the file is the length of the simulation box [l*].
    T0 = data[2];				// The 3rd line in the file is initial temperature [T*].
    h  = data[3];				// The 4th line in the file is the value of the time step written.
    t_max = data[4];			// The 5th line in the file is the maximum time of simulation.
	 
    x_0 = new float [N];
    y_0 = new float [N];	 
    x   = new float [N];
    y   = new float [N];
    vx  = new float [N];
    vy  = new float [N];
    ax  = new float [N];
    ay  = new float [N]; 	 
	 
} 


// This function allocates the memory and initiates the variables ===============
void init() {
    iseed = -abs(time(NULL));                 // That initiates the seed of the random number generator. Add the comment
                                              // sign (\\) at the beginning of the line when debugging the code.
	
	ft.open("f(t).txt");
    path.open("path.txt");
    path << L << '\t' << N << endl;         // The path stream header
    res.open("result.txt");
    
    // Initialize the global variables
    t = 0.;

    int sqrtN   = round(sqrt(N));
    float delta = 0.5 * L / sqrt(N);
    
    for (int i=0; i<N; i++) {
        int j = i / sqrtN;
        int k = i % sqrtN;
        x_0[i] = k * delta;                   // Initialize xᵢ
        y_0[i] = j * delta;                   // and yᵢ
        
        x[i] = x_0[i];
        y[i] = y_0[i];
    }
    
    // Initialize the velocities by the uniform random value in the range of [0..1).
    for (int i=0; i<N; i++) {
        vx[i] = ran2(&iseed);
        vy[i] = ran2(&iseed);
    }
    
    // Find the average of velocities.
    float Sx = 0,
          Sy = 0;
    
    for (int i=0; i<N; i++) { 
        Sx += vx[i];
        Sy += vy[i];
    }
    
    Sx /= N;
    Sy /= N;
    
    // Shift the average of velocities to zero.
    for (int i=0; i<N; i++) { 
        vx[i] -= Sx;
        vy[i] -= Sy; 
    }
    
    scale(T0);                              // Scale the velocities to tune the temperature to T.       
    
    Acc(x, y, ax, ay);                      // Initialize the accelerations.

}

// scales the velocities of the particles relative to the temperature ===========
void scale(float T) {
    
    float S=0;  
    for(int i=0; i<N; i++)
    	S += sqr(vx[i]) + sqr(vy[i]);		// < vx[i]^2 + vy[i]^2 > * N-1
	
	float r = sqrt(2*T*(N-1)/S);			// scale factor
	
	for (int i=0; i<N; i++){
		vx[i] *= r;
		vy[i] *= r;
	}		
}

// Deallocation of memory and file(s) ===========================================
void done() { path.close();  res.close();  ft.close(); }

// Executing the algorithm ======================================================
void run() {
	int c = 0;
		A = 0;
    do {
    	Update();
    	if ((c & 0x3ff) == 0)					// every 1024 step, save position in path file.
        	ExportPath();
        	
        if ((c & 0x1ff) == 0)					// every 512 step, save tempeteture and energy in result file.
        	res << t << "\t" << T0 << "\t" << EK() << "\t" << log(EK()) << endl;

        ++c;
    } while (t <= t_max);
}

// Updates particles according to the velocity Verlet algorithm =================
void Update() {
    t += h;
    A += 1;
    
    float f_t = 0.;
    
    for (int i=0; i<N; i++){ 	
    	x[i] = PB( x[i] + h*vx[i] + 0.5*h*h*ax[i] );		// update position for x Direction
    	y[i] = PB( y[i] + h*vy[i] + 0.5*h*h*ay[i] );		// update position for y Direction
    	 	
    	f_t += sqr(x[i]-x_0[i]) + sqr(y[i]-y_0[i]);
	}
	if ((A & 0x3ff) == 0)							// every 1024 step, save displacement.
    ft << t << "\t" << f_t/N << endl;	
	
    float bx[N];
    float by[N];
    Acc(x, y, bx, by);								// update Acceleration in time n+1
 
	if (((A & 0x1ff) == 0) && (t>2.)){ 				// Lower the temperature  
   		if(T0 > 0.1) T0 -= 1./16*T0;
   		scale (T0);
	}   
    
    
    for (int i=0; i<N; i++){
    	
    	vx[i] += 0.5*h* (ax[i] + bx[i]);			// update velocity for x Direction
    	vy[i] += 0.5*h* (ay[i] + by[i]);			// update velocity for y Direction
				
		ax[i] = bx[i];								// update Acceleration in time n for x Direction
		ay[i] = by[i];								// update Acceleration in time n for y Direction
	}
}

//==============================================================================
// PB() returns the position of the particle relative to the simulation box based on the periodic boundary conditions
float PB(float x) {
    if (x>L){ 		return x-L;	
	} else if(x<0){ return x+L;	
	} else 			return x;
}

//==============================================================================
// Acc() calculates and returns the accelerator of particles according to the position of particles
void Acc(float* x,  float* y, float* ax, float* ay) {
    
    for (int i=0; i<N; i++){
    	ax[i] = 0;
    	ay[i] = 0;
    }
    
    float fx;			// Force in the direction x
    float fy;			// Force in the direction y
    
    for (int i=0; i<N-1; i++){
    	for (int j=i+1; j<N; j++){

    		float dx = RD(x[j] - x[i]);
    		float dy = RD(y[j] - y[i]);
    			
    		LJForce(dx, dy, fx, fy);
    		
    		ax[i] += fx;				// Calculate the particle acceleration by 
    		ay[i] += fy;				// the force of all other particles
    		ax[j] -= fx;
    		ay[j] -= fy;
		}
	}
}

// Find the force from Lennard-Jones potential ==================================
float LJForce(float  dx, float  dy, float &fx, float &fy) {
    
    float r2 = sqr(dx) + sqr(dy);					// The distance of two particles from each other
    float g  = 24/r2 * ( 2*pow(r2,-6) - pow(r2,-3) );
    
    fx = -g*dx;				// Calculate the force through the distance between the particles
    fy = -g*dy;				
}

// The relative distance of particles in PBC ====================================
float RD(float delta) {
    if (delta > L/2){			return delta-L;	
	} else if (delta < -L/2) {	return delta+L;
	} else 						return delta;
}

// Export the current position of particles to path stream ======================
void ExportPath() {
    path << t << '\t' << N << endl;	
    for (int i=0; i<N; i++)
        path << x[i] << '\t' << y[i] << endl;  
}

// EK() returns the kinetic energy of the system per atom [ɛ] ===================
float EK() {
    float S = 0;
    for (int i = 0; i<N; i++)
        S += sqr(vx[i]) + sqr(vy[i]);
    
    return 0.5 * S / (N-1);                 // Here, we assume the 〈v_iy〉 = 〈v_iy〉 = 0
}

