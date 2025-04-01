// NewExportBinary.cpp : implementation file
//

#include "stdafx.h"
#include "../common/NewExportBinary.h"
#include "../common/nrutil.h"
#include "../common/utils.h"

#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// NewExportBinary


double newdoublebuff[33000];

NewExportBinary::NewExportBinary()
{
	Compressed = false;
	CompressionFactor = 1;
	Version = 0;
	NTraces=0;
	NPoints=0;
	SampleTime=0.001; // in secs
	VHold=0; // in mV
	StimInterval=0; // in secs
	TimeOffset=0; // in sampletimes
	fitted=false;
	data = NULL;
	fit=NULL;
	xdata = 0;
}

NewExportBinary::~NewExportBinary()
{
	if (fit){
		free_dmatrix(fit, 0, NTraces-1, 0, NPoints-1);
		fit = NULL;
	}
	if (data){
		free_dmatrix(data, 0, NTraces-1, 0, NPoints-1);
		data = NULL;
	}
	if (xdata){
		delete [] xdata;
		xdata = NULL;
	}
}


/////////////////////////////////////////////////////////////////////////////
// NewExportBinary message handlers

BOOL NewExportBinary::WriteHeader(CFile & f){
	SaveInt(Version,f);

	SaveInt(NTraces, f);
	SaveInt(NPoints, f);
	SaveDouble(SampleTime, f);
	SaveDouble(VHold, f); // in mV

	SaveDouble(StimInterval, f); // in secs
	SaveInt(TimeOffset, f); // in sampletimes

	SaveDouble(conc, f);
	int j;
	for (j=0; j<10; j++){
		SaveDouble(spare[j], f);
	}

	SaveInt(NSegments, f);

	for (j=0; j<MAXSEGMENTSNEWEXPORTBINARY ; j++){
		SaveDouble(V[j], f);
		SaveDouble(DV[j], f);
		SaveDouble(T[j], f);
		SaveDouble(DT[j], f);
	}


	return true;
}

BOOL NewExportBinary::LoadHeader(CFile & f){
	if (!LoadInt(Version, f)) return false;
	if (Version !=0 ) return false;

	if (!LoadInt(NTraces, f)) return false;
	if (!LoadInt(NPoints, f)) return false;
	if (!LoadDouble(SampleTime, f)) return false;
	if (!LoadDouble(VHold, f)) return false; // in mV
	if (!LoadDouble(StimInterval, f)) return false; // in secs
	if (!LoadInt(TimeOffset, f)) return false; // in sampletimes

	if (!LoadDouble(conc, f)) return false;
	int j;
	for (j=0; j<10; j++){
		if (!LoadDouble(spare[j], f)) return false;
	}

	if (!LoadInt(NSegments, f)) return false;

	for (j=0; j<MAXSEGMENTSNEWEXPORTBINARY ; j++){
		if (!LoadDouble(V[j], f)) return false;
		if (!LoadDouble(DV[j], f)) return false;
		if (!LoadDouble(T[j], f)) return false;
		if (!LoadDouble(DT[j], f)) return false;
	}


	return true;
}

BOOL NewExportBinary::LoadData(CFile & file){
	return DoLoadData(file, false, 1.0);
}

BOOL NewExportBinary::LoadCompressed(CFile & f, double CompressionFactor){
	return DoLoadData(f, true, CompressionFactor);
}



BOOL NewExportBinary::DoLoadData(CFile & file, BOOL Compress, double CompressionFactor){
	if (Compress){
		if (CompressionFactor<=1.0){
			Compress = false;
		}
	}
	int i, t;
	if (!Compress){
		if (!data){
			data = dmatrix(0, NTraces-1, 0, NPoints-1);

		}
		if (!fit){
			fit = dmatrix(0, NTraces-1, 0, NPoints-1);
		}
		
		xdata = new double[NPoints];

		for (t=0; t<NTraces; t++){

			UINT s=sizeof(double)*NPoints;
			UINT read;
			read=file.Read(data[t], s);
			if (read!=s){
				return false;
			}
		}
		for (int i=0; i<NPoints; i++){
			xdata[i] = double(i+TimeOffset)*SampleTime;
		}
		return true;
	}
	
	double prod = 1.0;
	int NCompressedPoints = 0;
	for (i=0; i<NPoints; i++){
		NCompressedPoints ++;
		prod *= CompressionFactor;
		if ((prod+NCompressedPoints)>=NPoints) break;
	}
//	ShowFloat(CompressionFactor, "CF");
//	ShowFloat(NPoints, "nP");
//	ShowFloat(NCompressedPoints, "nC");
	data = dmatrix(0, NTraces-1, 0, NCompressedPoints-1);

	fit = dmatrix(0, NTraces-1, 0, NCompressedPoints-1);
	
	xdata = new double[NCompressedPoints+10];

	int j, i1, i2;
	double sum;
	int Nav;
	CWnd * f = AfxGetMainWnd();
	CDC * pDC = f->GetDC();

	TEXTMETRIC textMetric;
	pDC->GetTextMetrics(&textMetric);

	for (t=0; t<NTraces; t++){

		UINT si=sizeof(double)*NPoints;
		UINT read;
		read=file.Read(newdoublebuff, si);
		if (read!=si){
			f->ReleaseDC(pDC);
			return false;
		}
		int yy=100;

		int xx=50;

		char s[20];
			pDC->TextOut(xx, yy, CString("loading binary data"));


		yy += textMetric.tmHeight;


		_itoa(NPoints, s, 10);
		pDC->TextOut(xx, yy, CString("NPoints ")+CString(s)+CString("          "));
		yy += textMetric.tmHeight;

		_itoa(NCompressedPoints, s, 10);
		pDC->TextOut(xx, yy, CString("NCompressedPoints ")+CString(s)+CString("          "));
		yy += 2*textMetric.tmHeight;

		_itoa(t, s, 10);
		pDC->TextOut(xx, yy, CString("loading trace ")+CString(s)+CString("          "));
		yy += textMetric.tmHeight;
	
//		ShowFloat(t, "t");

		i1=0;
		prod = 1.0;
		for (j=0; j<NCompressedPoints; j++){
			i2 = Calci2(i1, prod);
			if (i2>=NPoints){
				i2=NPoints-1;
			}
			
			sum=0;
			Nav=0;
			xdata[j] = double(i1+TimeOffset) + double(i2-i1)/2.0;
			xdata[j] *= SampleTime;
			for (i=i1; i<=i2; i++){
				sum += newdoublebuff[i];
				Nav++;
			}
			data[t][j] = sum/double(Nav);
			prod *= CompressionFactor;
			i1=i2+1;
//			if (Nav<1)	ShowFloat(Nav, "Nav");
		}
	}
	NPoints = NCompressedPoints;
	Compressed = true;
	f->ReleaseDC(pDC);
	return true;
}

double NewExportBinary::GetMax(){
	double max = 0.0;
	int i, t;
	for (t=0; t<NTraces; t++){
		for (i=0; i<NPoints; i++){
			if (fabs(data[t][i])>max){
				max = fabs(data[t][i]);
			}
		}
	}

	return max;
}

BOOL NewExportBinary::WriteData(CFile & file){
	if (!data){
		return false;
	}
	int t, i;

	for (t=0; t<NTraces; t++){
		for (i=0; i<NPoints; i++){
			SaveDouble(data[t][i], file);
		}
	}
	return true;
}


void NewExportBinary::ShowHeader(){
//	ShowInt(ExportType, "ExportType"); // 0 = IV, 1 = tails, 2 = envelope, 3 = direct IV
	ShowInt(NTraces, "NTraces");
	ShowInt(NPoints, "NPoints");
	ShowDouble(SampleTime, "SampleTime"); // in secs
	ShowDouble(VHold, "VHold"); // in mV
	ShowDouble(StimInterval, "StimInterval"); // in secs
	ShowInt(TimeOffset, "TimeOffset"); // in sampletimes
	ShowDouble(conc, "conc");
	ShowInt(this->NSegments, "N Segs");
	for (int i=0; i<NSegments; i++){
		ShowDouble(this->V[i], "V");
		ShowDouble(this->T[i], "T");
		ShowDouble(this->DV[i], "DV");
		ShowDouble(this->DT[i], "DT");
	}
	for (int t=(NTraces-1); t<NTraces; t++){
		for (int i=(NPoints-1); i<NPoints; i++){
			ShowFloat(data[t][i], "D");
		}
	}
//	ShowDouble(VTail, "VTail"); // in mV
//	ShowDouble(DeltaVTail, "DeltaVTail"); // != 0 only for tails
//	ShowDouble(TCond, "TCond"); // in secs
//	ShowDouble(DeltaTCond, "DeltaTCond"); // != 0 only for envelope
//	ShowDouble(VCond, "VCond"); // in mV
//	ShowDouble(DeltaVCond, "DeltaVCond"); // != 0 only for IV!!
//	if (ExportTye==4){
//		ShowDouble(TCond2, "TCond2"); // in secs
//		ShowDouble(VCond2, "VCond2"); // in mV
//	}

}

