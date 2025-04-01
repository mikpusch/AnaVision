#include "stdafx.h"

#include "../common/CapacitanceAnalysis.h"
#include "../common/utils.h"

#define PI_CAPACITANCE	    3.14159265358979323846
#define TWOPI_CAPACITANCE	    (2.0 * PI_CAPACITANCE)

CapacitanceAnalysis::CapacitanceAnalysis(
						double SampleTimeInSeconds,
						double CosineFrequencyInHz,
						double Gain , // To convert the input data from __int16 to Amperes
						double VHoldInVolt, // To calculate the dc conductance
						double CosineAmplitudeInVolt, // The stimulation amplitude
						double Phi, // The pase angle in radians to correct for phase shifts
						int NCyclesOmitFromStart, // see above
						bool RsCmUSed, // as explained in Gillis, 2000, 
									 // the Rs-Cm compensation will be added artificially usin gthe values below
						double RsCompensatedInMOhm,
						double CmCompensatedInpF)
{
	COSINEFREQ = CosineFrequencyInHz;
	SAMPLET = SampleTimeInSeconds;
	GAIN = Gain;
	VHOLD = VHoldInVolt;
	COSAMPLITUDE = CosineAmplitudeInVolt;
	PHI = Phi + PI_CAPACITANCE/2.0;
	NCYCLESOMIT = NCyclesOmitFromStart;

	RSCMUSED = RsCmUSed; // as explained in Gillis, 2000, 
									 // the Rs-Cm compensation will be added artificially usin gthe values below
	RSCOMPENSATED = RsCompensatedInMOhm;
	CMCOMPENSATED = CmCompensatedInpF;
	PeriodOfCosine = 1/COSINEFREQ;
	NumberOfPointsPerCycle = int (1.0/(SAMPLET*COSINEFREQ));
	NStart = int ( double(NCyclesOmitFromStart)/(SAMPLET*COSINEFREQ)) ;

	RealAdmittanceCompensated = ImagAdmittanceCompensated = 0.0;

	if (RSCMUSED){
		double tau = RSCOMPENSATED*CMCOMPENSATED*1e-6; // in seconds
		double omegac = COSINEFREQ*2.0*PI_CAPACITANCE;
		double tp1 = omegac*tau;
		ImagAdmittanceCompensated = omegac*CMCOMPENSATED*1e-12/(1.0 + tp1*tp1);
		RealAdmittanceCompensated = ImagAdmittanceCompensated*tp1;
	}

}

CapacitanceAnalysis::~CapacitanceAnalysis(){
}


void CapacitanceAnalysis::Analyze(
		__int32 * data,
		int NPoints,  // number of points in array. The first NCyclesOmitFromStart cycles will be omitted. Only a whole number of cycles
					  // will be analyzed
		double & DCConductanceInSiemens,
		double & A, // Real part of admittance, in Siemens
		double & B, // Imaginary part of admittance
		double & RsInOhm, // Series Resistance
		double & CMinFarad,
		double & RmInOhm){

		double DoubleNumberOfCyclesToAnalyze = double(NPoints - NStart)*COSINEFREQ*SAMPLET;

		NumberOfCyclesToAnalyze = int (DoubleNumberOfCyclesToAnalyze);

		NumberOfPointsToAnalyze = int ( double(NumberOfCyclesToAnalyze)/ (SAMPLET*COSINEFREQ) );

		if (NumberOfPointsToAnalyze<=0){
			Alert0("ERROR IN CapacitanceAnalysis: NumberOfPointsToAnalyze<=0");
			return;
		}

		MeanCurrent = 0;
		A = 0;
		B = 0;

		double UnscaledMean = 0;

		__int32 * ShiftedData =  data + NStart;

		double scalearg = COSINEFREQ*SAMPLET*TWOPI_CAPACITANCE;

		for (int i=0; i<NumberOfPointsToAnalyze; i++){
			double d = double(ShiftedData[i]);
			UnscaledMean += d;
		}

		UnscaledMean /= double(NumberOfPointsToAnalyze);

		for (int i=0; i<NumberOfPointsToAnalyze; i++){
			double d = double(ShiftedData[i]) - UnscaledMean;
			double arg = double(i)*scalearg + PHI;
			A -= d*cos(arg);
			B += d*sin(arg);
		}

		MeanCurrent = UnscaledMean*GAIN;

		double VEffective = VHOLD;
		if (fabs(VHOLD)<1e-6){
			VEffective = 1e-6;
		}
		DCConductanceInSiemens = MeanCurrent / VEffective;
		if (DCConductanceInSiemens<0){
//			DCConductanceInSiemens = 0;
//			Alert0("ATTENTION IN CapacitanceAnalysis: DCConductanceInSiemens<0 : set to 0");
		}
		double b= DCConductanceInSiemens;

		double ScaleAdmittance = 2.0*SAMPLET*GAIN*COSINEFREQ/COSAMPLITUDE/double(NumberOfCyclesToAnalyze);

		A *= ScaleAdmittance;
		B *= ScaleAdmittance;

		A += RealAdmittanceCompensated;
		B += ImagAdmittanceCompensated;


		double tp1 = A*A + B*B - A*b;
		RsInOhm = (A-b)/tp1;

		double tp2 = (A-b)*(A-b) + B*B;

		CMinFarad = tp1*tp1/(TWOPI_CAPACITANCE * COSINEFREQ*B*tp2);

		RmInOhm = tp2/(b*tp1);

}


