// Utility functions
// =================
inline float sqr(float x) {                 // The inline square function return x²
    return x * x; 
}
//------------------------------------------

// Random number generator
// =======================
/* The following function is based on the random number generator 

   double ran2(long *idum);

which is implemented in the  Numerical Recipes in C, chapter 7 (ran2)

Long period (> 2 × 10^{18}) random number generator of L. Ecuyer with Bays-Durham shuffle
and added safeguards. Returns a uniform random deviate between 0.0 and 1.0 (exclusive of
the endpoint values). 

***!!! Call with idum a negative integer to initialize; !!!*** thereafter, do not alter
idum between successive deviates in a sequence. RNMX should approximate the largest floating
value that is less than 1. For example, define*/

extern long iseed;                          // seed of random number generator

/*
 * "ran2(&iseed)" returns a random number in range [0..1) with an uniform distribution, and
 * reset the iseed for next call.*/

double ran2(long *idum);

/*
 * "iseed = -abs(time(NULL))" initiates the seed of random number generator. Remove it or 
 * add the comment sign (\\) at the beginning of its line of code when debugging the code.

/*Visit www.nr.com for the licence.*/
