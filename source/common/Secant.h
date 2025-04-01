#pragma once

// class to find a root between x1 and x2
class Secant{
public:
	Secant();
	~Secant();
	double InitSecant(double x1, double x2, double MaxError = 0.0, int MaxIter = 100); // 
		// This function returns the first x-value to evaluate. The evaluation (i.e. function value)
		// has to be suplied as fx in the following function

	bool NextSecant(double fx, double &NewX);
		// User supplies function value of previous evaluation and recieves the next x-value
	// If the procedure has converged or if the max no. of iterations have been done the functuon  retuns false;

/* Example of use:

	Secant s;
	double x = s.InitSecant(0.0, 1.0, 1e-10, 20);
	do{
		double func = MyFunc(x);
	while (s.NextSecant(func, x));

	print x, "Root found"
*/





private:
	double X;
	int MAXITER;
	double MAXERROR;
	double X1, X2;
	double Y1, Y2;
	int j;

	bool Initialized;
};