//#define SPARSE_MARKOV_MAX_STATES 500000
#pragma once

//#ifndef MIK_SPARSE_MARKOV
//#define MIK_SPARSE_MARKOV

class SparseMarkov
{
public:
	SparseMarkov();
	~SparseMarkov();

	static int DEFAULT_MAX_ITER_SPARSE_MARKOV;
	static double DEFAULT_MIN_NORM_SPARSE_MARKOV;

	// METHOD 1
	// call first:
	void SetNStatesAndNRatesMarkov(int NStates, int NRates);
	// this only allocates the space and does not asssign any values or
	// does calculationa	

//	Then assign values for the sparse matrix, externally

	// then call either of the following:
	void CalcPinf(); // uses the above default values
	void CalcPinf(double MinNorm);
	void CalcPinf(int MaxIter);
	void CalcPinf(double MinNorm, int MaxIter);



// METHOD 2:

	void SetRates(int NStates, double ** R);
	// this assigns space  and determines also NRATES and assigns the values
	// in the sparse matrix, and calls "calcpinf"

	double * rsa;
	int	   * isa;
	double * pinf;


private:
	int NS, NRATES;

	double * rsforcheck;

	void DeleteSparseMarkov();

};

//#endif
