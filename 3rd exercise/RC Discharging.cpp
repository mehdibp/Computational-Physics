/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template code for RC discharging                                                               ***/
/***                                                                                                 ***/
/***  Radioactive decay, v1.0; Date: 1400/02/21                                                      ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in C++ by DevC++ IDE on Windows                                ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use my first code in the moodle to write this code.
 */
 

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <bits/stdc++.h> 

using namespace std;

// Parameters and constants ==============================================
const float tmax = 10;                      // Final time for example method
const float h = 1./8.;                      // Time step, it is better to choose it as a power of 2.

// Variables =============================================================
float q_t ,q_SI ;
float V,R,C ,Tmax_SI,h_SI ,Q;				// Variables that is read from file
	
int delta , delta_SI;    					// Number of intervals & Number of intervals in the SI system.
	string data_file;						// get the name of the input file.
ofstream qt;                                // This stream maintains the results.

// Declarations ==========================================================
void init();                                // This function allocates the memory and initiates the variables.
void Openfile();							// This function reads variables from a file and saves them in an array.
void execute();                             // This function executes the algorithm.
void execute_SI();                          // This function executes the algorithm in the SI system.
void done();                                // This function deallocates the memory and file(s).



// Main routine =================================================================
int main() { 
    cout << "RC discharging, v 1.0; Date: 1400/02/21" << endl;
    cout << "by Mehdi Bakhshipoor; Student ID#: 971315 \n\n" << endl;

    Openfile();
    
    clock_t runtime = clock();
    init();
    execute();
    execute_SI();
    done();
    cout << "run time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;
}

// Read files and insert numbers ================================================
void Openfile () {
	
	cout << "Enter the name of your data file" <<endl;
	cin >> data_file;									// Receives the name of the input file.
	
	char* in ;											// These lines prepare the file name to read the numbers in Dev.
	int n = data_file.length(); 
	char char_array[n+1]; 
	strcpy(char_array, data_file.c_str());		
	in = char_array;
	
	ifstream fin (in , ios :: in ) ;
	
	double *data;
	data = new double [5];
	for (int i=0; i<5; i++)
		fin >> data[i];			// Save numbers from the file inside the array.

	fin.close();
	
	V = data[0];				// The 1st line in the file is the amount of voltage written.
	R = data[1];				// The 2nd line in the file is the value of the resistance written.
	C = data[2];				// The 3rd line in the file is the amount of capacity written.
	Tmax_SI = data[3];			// The 4th line in the file is the value of the final time written.
	h_SI = data[4];				// The 5th line in the file is the value of the time step written.
} 


// allocates the memory and initiates the variables =============================
void init() { 

    qt.open("q(t).txt");
    
    q_t = 1.0;					// The initial amount of electric charge in reduced units system.
    delta = tmax/h;				// Number of steps in reduced units system.
     
    Q = V*C;					// The initial amount of electric charge from voltage and capacitance
	q_SI = Q;
	delta_SI = Tmax_SI/h_SI;   	// Number of steps in the SI system
}

// deallocates the memory and file(s) ===========================================
void done() { qt.close(); }


// executes the algorithm in reduced units system ===============================
void execute() { 

	qt << "In reduced units system:" <<endl;															// copy tital in file
	qt << "time\t\t" << "Euler method\t" << "The example method\t" << "Analysis answer" << endl;		// copy tital in file


// The method mentioned in the example
/*	float *q;
	q = new float [delta];
	
	q[0]= q_t;
	q[1]= 1-h;
	
	for (int i=0; i<=delta; i++) 
		q[i+2] = -(2*h)*q[i+1] + q[i];					// The value of 'q' at any particular time (at t = h*i)

	for (int i=0; i<=delta ; i++)  {
		q_t = pow(1-h,i);								// The value of 'q' at any particular time (at t = h*i)
		qt << h*i << "\t\t" <<  q_t << "\t" << q[i] << "\t\t" << exp(-(h*i)) << endl;			// copy in file	
	}
	
delete[] q;*/

	float q0 , q1 , q2;
	
	q0= q_t;
	q1= 1-h;
		 
	for (int i=0; i<=delta ; i++)  {
		
		q_t = pow(1-h,i);						// The value of 'q' at any particular time (at t = h*i)
		
		qt << h*i << "\t\t" <<  q_t << "\t" << q0 << "\t\t" << exp(-(h*i)) << endl;		// copy in file	
		
		q2 = -(2*h)*q1 + q0;					// The value of 'q' at any particular time (at t = h*i)	
		q0 = q1;
		q1 = q2;
	}

}


// executes the algorithm in the SI system ======================================
void execute_SI() { 
	
	qt << "\n\n\nIn SI system:" <<endl;																// copy tital in file
	qt << "time\t\t" << "Euler method\t" << "The example method\t" << "Analysis answer" << endl;	// copy tital in file

	// The method mentioned in the example --------------------------------------
	float *q_si;
	q_si = new float [delta_SI];
	
	q_si[0]= Q;
	q_si[1]= Q*(1-h_SI/(R*C));
	
	for (int i=0; i<=delta_SI; i++) 
		q_si[i+2] = -(2*h_SI/(R*C))*q_si[i+1] + q_si[i];				// The value of 'q' at any particular time (at t = h*i)


	// Euler methods ------------------------------------------------------------
	for (int i=0; i<=delta_SI; i++)  {
		q_SI = Q* pow(1-h_SI/(R*C),i);									// The value of 'q' at any particular time (at t = h*i)
		qt << h_SI*i << "µs\t\t" <<  q_SI << "\t\t" << q_si[i] << "\t\t" << Q*exp(-(h_SI*i)/(R*C)) << endl;		// copy in file	
	}
	
	delete[] q_si;
}
