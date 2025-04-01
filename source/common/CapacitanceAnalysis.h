#pragma once

/*
This module aims to determine the real and complex admittance, and the dc conductance from a stretch of
data obtained by sinusoidal stimulation.
The basic method is as follows:

Assume the acquired signal is I(t)with t=0, ..T, acquired at sample times dt
N is the number of sample points
thus sample points Ii are at t=i*dt (i=0, ..., N-1)

the stimulation with the sine wave is at frequency f (in Hz).
the assumption is that at t=0 the stimulation is V0*cos(2*pi*f*t+PHI)
the phase angle phi is supplied as input parameter to adjust for delays etc. 

each cycle of the cosine wave lasts T=1/f. 
each cycle of the cosine wave has NCycle = T/dt= = 1/f/dt sample points.

in order to allow for settlement, a certain number of cycles is omitted from
the start of the analysis: NCyclesOmittFromStart

thus analysis starts at sample point NStart = NCyclesOmittFromStart/f/dt.

An integer number of cycles will be analyzed.

This is exactly: NumberOfCycles = int ( (N-NStart)/ NCycle ) = int ((N - NStart)*f*dt)

Input parameters:	__int 16 * Current;
					double gain; 

*/

class CapacitanceAnalysis{
public:
	CapacitanceAnalysis(double SampleTimeInSeconds,
						double CosineFrequencyInHz,
						double Gain , // To convert the input data from __int16 to Amperes
						double VHoldInVolt, // To calculate the dc conductance
						double CosineAmplitideInVolt, // The stimulation amplitude
						double Phi, // The pase angle in radians to correct for phase shifts
						int NCyclesOmitFromStart, // see above

						bool RsCmUSed, // as explained in Gillis, 2000, 
									 // the Rs-Cm compensation will be added artificially usin gthe values below
						double RsCompensatedInMOHm,
						double CmCompensatedInpF
					);
	void Analyze(
			__int32 * data,
			int NPoints,  // number of points in array. The first NCyclesOmitFromStart cycles will be omitted. Only a whole number of cycles
						  // will be analyzed
			double & DCConductanceInSiemens,
			double & A, // Real part of admittance, in Siemens
			double & B, // Imaginary part of admittance
			double & RsInOhm, // Series Resistance
			double & CMinFarad,
			double & RmInOhm);

	~CapacitanceAnalysis();
private:
	double COSINEFREQ;
	double SAMPLET;
	double GAIN;
	double VHOLD;
	double COSAMPLITUDE;
	double PHI;
	double NCYCLESOMIT;

	bool RSCMUSED; // as explained in Gillis, 2000, 
									 // the Rs-Cm compensation will be added artificially usin gthe values below
	double RSCOMPENSATED;
	double CMCOMPENSATED;

	double RealAdmittanceCompensated;
	double ImagAdmittanceCompensated;

	double PeriodOfCosine;
	int NumberOfPointsPerCycle;
	int NStart;

	int NumberOfCyclesToAnalyze;
	int NumberOfPointsToAnalyze;

	double MeanCurrent;
};
