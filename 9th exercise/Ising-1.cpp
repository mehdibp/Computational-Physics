/*******************************************************************************************************/
/***                                                                                                 ***/
/***  Template for a Ising model code - part 1                                                       ***/
/***                                                                                                 ***/
/***  Ising model - part 1, v1.0; Date: 1399/04/29                                                   ***/
/***  by Mehdi Bakhshipoor; Student ID#: 971315                                                      ***/
/***                                                                                                 ***/
/***  The following code is developed in the C++ by DevC++ IDE on Windows.                           ***/
/***                                                                                                 ***/
/*******************************************************************************************************/

/*
 * Note: I use a template in the moodle to write this code. My code is presented in the 
 * Openfile(), Init(), Execute(), magnetization() functions.
 */
 
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <ctime>

using namespace std;


#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

double ran2(long *idum) {
    int j;
    long k;
    static long idum2=123456789;
    static long iy=0;
    static long iv[NTAB];
    double temp;

    if (*idum <= 0) {
        if (-(*idum) < 1) *idum=1;
        else *idum = -(*idum);
        idum2=(*idum);
        for (j=NTAB+7;j>=0;j--) {
            k=(*idum)/IQ1;
            *idum=IA1*(*idum-k*IQ1)-k*IR1;
            if (*idum < 0) *idum += IM1;
            if (j < NTAB) iv[j] = *idum;
        }
        iy=iv[0];
    }
    k=(*idum)/IQ1;
    *idum=IA1*(*idum-k*IQ1)-k*IR1;
    if (*idum < 0) *idum += IM1;
    k=idum2/IQ2;
    idum2=IA2*(idum2-k*IQ2)-k*IR2;
    if (idum2 < 0) idum2 += IM2;
    j=iy/NDIV;
    iy=iv[j]-idum2;
    iv[j] = *idum;
    if (iy < 1) iy += IMM1;
    if ((temp=AM*iy) > RNMX) return RNMX;
    else return temp;
}
#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

// -----------------------------------------------------------------------------
// My interface 

// Random number generator seed
long iseed = -36;

// You can initialize random number generator seed by using the following routine.
void Randomize() { iseed = -time(NULL); }

// Return random number with uniform distribution in the range [0..1).
inline double Random() { return ran2(&iseed); }

// Return integer random number with uniform distribution in range 0 to N which includes 0 but excludes N.
inline int Random(int N) { return int(ran2(&iseed)*N); }
//END_FOLD

int L, step_N1;	             		// in file.
const int LMask = L-1;          	// Mask used to perform periodic boundary condition in a Perid() function
int **s;                        	// Spins lattice; s[i][j] shows a spin of the cell (i,j)

// All the following parameters are in the reduced unit system
float T, h, J, N;                   // in file.

ofstream snapshot("snapshot.txt", ios::out | ios::trunc);	 // Keeps model snapshot
ofstream magnet  ("magnet.txt"  , ios::out | ios::trunc);	 // save magnetization per step
ofstream energy ("Energy.txt", ios::out | ios::trunc);  
                   

void Openfile();				  // This function reads variables from a file and saves them in an array.
void Init();                      // Initialize model, allocate dynamic memory, ...
void Execute();                   // Simulate Ising model
void PostProcess();               // Perform post statistical analysis
float Energy();
void Done();                      // Deallocate dynamic memory, ...


// Main routine =================================================================
int main () {
	cout << "Ising code, v1.0; Date: 1400/04/29" << endl;
    cout << "by Mehdi Bakhshipoor; Student ID#: 971315\n" << endl;
	
	Openfile();
	
    clock_t runtime = clock();
    Init();   
    Execute();
    //PostProcess();
    Done();

	cout << "\nrun time = " << (double) (clock() - runtime) / CLOCKS_PER_SEC << " sec" << endl;
    cout << "Finish" << endl;

}

// Read files and insert numbers ================================================
// Read files and insert numbers
void Openfile () {
	
	ifstream fin ("data1.txt" , ios :: in ) ;
	
	double *data;
	data = new double [5];
	for (int i=0; i<5; i++)
		fin >> data[i];			// Save numbers from the file inside the array.

    fin.close();
	
    L = data[0];				// The 1st line in the file is the lattice size; L=2^n.
    T = data[1];				// The 2nd line in the file is temperature [T*].
    h = data[2];				// The 3rd line in the file is the external field.
    J = data[3];				// The 4th line in the file is the spin-spin coupling constant. 
    step_N1 = data[4];			// The 5th line in the file is the number of Monte Carlo steps.

    N = L*L;						// Namber of Spin 

}

// Initialize model, allocate dynamic memory, ... ===============================
void Init() {
    cout << "Init ..." << endl;
    
    Randomize();
    s = new int*[L];
    
    for (int i = 0; i<L; i++) {
        s[i] = new int[L];
        for (int j = 0; j<L; j++){
            s[i][j] = 1;				// at first all spin is +1

    // for random spin +1,-1
		// if (Random() > 0.5)
		//     s[i][j] = 1;
	    // else
		//     s[i][j] = -1;
	    
    	}
	}
    snapshot << L << endl;
}

// Deallocate dynamic memory, ... ===============================================
void Done() {
    cout << "Done ..." << endl;

    snapshot.close();
    magnet.close();
    energy.close();
    
    for (int i = 0; i<L; i++)
        delete[] s[i];
    delete[] s;
}

//==============================================================================
// Rotate i if cross the boundary with respect to the periodic boundary condition
inline int Period(int i) {
//    return i & LMask;
    return (i+L) % L;
}

// Calculate Delta E if s(i,j) flipped ==========================================
float Delta_E_Flip(int i, int j) {
    float sum = 0.0;
    sum = s[i][Period(j+1)] + s[i][Period(j-1)] + s[Period(i-1)][j] + s[Period(i+1)][j];
    return 2*s[i][j]* (h + J * sum);
}

// Perform a single Monte Carlo step (step b in algorithm in page 7-12) =========
void Single_Monte_Carlo_Step() {
    for (int c=0; c<L*L; c++) { 			// L^2 infinitesimate (Markov) step
        int i = Random(L);        			// Select a random row between [0.,L-1]
        int j = Random(L);       			// Select a random column between [0.,L-1]
        
		float dE = Delta_E_Flip(i,j);
        if ( dE < 0 ) {          			// If the total energy decreases after spin-flip, accept the infinitesimal step
            s[i][j] = -s[i][j];
        } else if ( Random() < exp(-dE/T) ) {
            s[i][j] = -s[i][j];
        }
    }
    
}

// Export a single snapshot to snapshot.txt =====================================
void Export_Single_Snapshot() {
    for (int i = 0; i<L; i++)
        for (int j = 0; j<L; j++)
            if ( s[i][j] > 0 )  
                snapshot << "1" << " ";
            else
                snapshot << "0" << " ";
    snapshot << endl;        	
}

//this function calculate the magnetization of the spins ========================
float magnetization() {
  	float M = 0.0;
  	for (int i=0; i<L; i++) {
  		for (int j=0; j<L; j++) {
  			M += s[i][j];           // total of spins
  		}
 	}
    return ( M/N );    			    // magnetization
}

// Simulate Ising model =========================================================
void Execute() {
    cout << "Execute the Metropolis algorithm ..." << endl;
    //The Metropolis algorithm
    for (int i=0; i<step_N1; i++) {
    	Single_Monte_Carlo_Step();
		
		magnet << i+1 << "\t" << magnetization() << endl;		
		energy << i+1 << "\t" << Energy()/N << endl;
	}
	
    Export_Single_Snapshot();
}

// ==============================================================================
float Energy() {
	float E = 0.0;
	
	for (int i=0; i<L; i++) {
  		for (int j=0; j<L; j++) {
  			E += s[i][j]*(s[i][Period(j+1)] + s[i][Period(j-1)] + s[Period(i-1)][j] + s[Period(i+1)][j]);
  		}
  	}
	return -E/2.0;
}
