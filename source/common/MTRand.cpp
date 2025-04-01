#include "stdafx.h"

#include <math.h>

#include "../common/MTRand.h"


int				MTRand::mti		=	N_MTRAND+1; /* mti==N+1 means mt[N] is not initialized */
unsigned long	MTRand::mt []	=	{0};


/* initializes mt[N] with a seed */
void MTRand::init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N_MTRAND; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void MTRand::init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N_MTRAND>key_length ? N_MTRAND : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N_MTRAND) { mt[0] = mt[N_MTRAND-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N_MTRAND-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N_MTRAND) { mt[0] = mt[N_MTRAND-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long MTRand::genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A_MTRAND};
    /* mag01[x] = x * MATRIX_A_MTRAND  for x=0,1 */

    if (mti >= N_MTRAND) { /* generate N words at one time */
        int kk;

        if (mti == N_MTRAND+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N_MTRAND-M_MTRAND;kk++) {
            y = (mt[kk]&UPPER_MASK_MTRAND)|(mt[kk+1]&LOWER_MASK_MTRAND);
            mt[kk] = mt[kk+M_MTRAND] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N_MTRAND-1;kk++) {
            y = (mt[kk]&UPPER_MASK_MTRAND)|(mt[kk+1]&LOWER_MASK_MTRAND);
            mt[kk] = mt[kk+(M_MTRAND-N_MTRAND)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N_MTRAND-1]&UPPER_MASK_MTRAND)|(mt[0]&LOWER_MASK_MTRAND);
        mt[N_MTRAND-1] = mt[M_MTRAND-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long MTRand::genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double MTRand::genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double MTRand::genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double MTRand::genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double MTRand::genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */
