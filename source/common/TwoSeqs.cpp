
/**

This class implements the sliding window sequence comparsion of two sequences

 */
#include "stdafx.h"


#include "TwoSeqs.h"
#include "NeedleManWunsch.h"
#include "../common/nrutil.h"
#include "../common/utils.h"

#include <math.h>

using namespace std ;

SingleScoreClassTwoSeqs::SingleScoreClassTwoSeqs(){
	IndexStartA = LengthA = 0;
	IndexStartB = LengthB = 0;
	A="";
	B="";
	M="";

	Evaluated = false;
	Score = 0;
}
SingleScoreClassTwoSeqs::~SingleScoreClassTwoSeqs(){
}

double SingleScoreClassTwoSeqs::GetScore(){
	return Score;
}

bool SingleScoreClassTwoSeqs::WriteOnFile(CFile & file){
	CFile * fp = &(file);
	WriByNS(IndexStartA);
	WriByNS(LengthA);
	WriByNS(IndexStartB);
	WriByNS(LengthB);
	WriByNS(Evaluated);
	WriByNS(Score);
	SaveString(A, file);
	SaveString(B, file);
	SaveString(M, file);

	return true;
}
bool SingleScoreClassTwoSeqs::ReadFromFile(CFile & file){
	CFile * fp = &(file);
	ReByNS(IndexStartA);
	ReByNS(LengthA);
	ReByNS(IndexStartB);
	ReByNS(LengthB);
	ReByNS(Evaluated);
	ReByNS(Score);
	if (!LoadString(A, file)){
		return false;
	}
	if (!LoadString(B, file)){
		return false;
	}
	if (!LoadString(M, file)){
		return false;
	}
	return true;
}

TwoSeqs::TwoSeqs(){
	GapOpenPenalty = NeedlemanWunsch::GapOpenDefault();
	GapExpandPenalty = NeedlemanWunsch::GapExtendDefault();

	WindowLengthA = WindowLengthB = 10;

	NResultsToShow = 100;

}
TwoSeqs::~TwoSeqs(){
}
	
bool TwoSeqs::WriteOnFile(CFile & file){
	CFile * fp = &(file);
	WriByNS(GapOpenPenalty);
	WriByNS(GapExpandPenalty);
	WriByNS(WindowLengthA);
	WriByNS(WindowLengthB);
	WriByNS(NResultsToShow);
//	WriteStringOnFile(SeqA, file);
//	WriteStringOnFile(SeqB, file);
	SaveString(SeqA, file);
	SaveString(SeqB, file);
	int n = Aligns.size();
	WriByNS(n);
	for (int i=0; i<n; i++){
		Aligns[i].WriteOnFile(file);
	}
	return true;

}
bool TwoSeqs::ReadFromFile(CFile & file){
	CFile * fp = &(file);
	ReByNS(GapOpenPenalty);
	ReByNS(GapExpandPenalty);
	ReByNS(WindowLengthA);
	ReByNS(WindowLengthB);
	ReByNS(NResultsToShow);
//	ShowFloat(WindowLengthB, "LB");
	if (!LoadString(SeqA, file)){
		return false;
	}
//	Alert0(SeqA);
	if (!LoadString(SeqB, file)){
		return false;
	}
//	Alert0(SeqB);
	int n;
	ReByNS(n);
//	ShowFloat(n, "n");
	if (n<1){
		Aligns.resize(0);
		return true;
	}
	Aligns.resize(n);
	for (int i=0; i<n; i++){
		if (!Aligns[i].ReadFromFile(file)){
			return false;
		}
	}
	return true;
}

void TwoSeqs::Reset(){ // sets the number of results to 0;
	Aligns.resize(0);
}

void TwoSeqs::Restart(CDC * pDC){
	int n=Aligns.size();
	int na=SeqA.GetLength();
	int nb=SeqB.GetLength();

//	na=nb=100;

	if (this->WindowLengthA>na){
		Alert0("WindowLength a longer than sequence a");
		return;
	}
	if (this->WindowLengthB>nb){
		Alert0("WindowLength B longer than sequence B");
		return;
	}

	int count=0;

//	ShowFloat(nb, "nb");
	for (int ia=0; ia<(na-WindowLengthA-1); ia++){
		CString ca=SeqA.Mid(ia, WindowLengthA);
		ca.MakeUpper();
		for (int ib=0; ib<(nb-WindowLengthB-1); ib++){
			char s = InKey();
			if ((s=='s') || (s=='S')){
				return;
			}
			if (count>=n){
				count++;
				CString cb=SeqB.Mid(ib, WindowLengthB);
				cb.MakeUpper();
				SingleScoreClassTwoSeqs single;

				if (false){
//				if (ib>(nb-2*WindowLengthB)){
					Alert0(ca+" "+cb);
				}
//				Alert0(cb);
				NeedlemanWunsch NW = NeedlemanWunsch(true, ca, cb);

				CString AlignString;
				single.Score = NW.DoAlign(ca, cb,
										this->GapOpenPenalty,
										this->GapExpandPenalty,
										AlignString, true,
										single.A, single.B, single.M); 
				single.IndexStartA = ia;
				single.LengthA = na;
				single.IndexStartB = ib;
				single.LengthB = nb;

				if (pDC){
					CString c;
					char s[100];
					_gcvt(single.Score, 6, s);
					c = CString(s);
					_itoa(count, s,10);
					c += " " + CString(s);
					pDC->TextOut(50,50, CString("                                "));
					pDC->TextOut(50,50, c);
				}

//				Alert0("alignstring");
//				Alert0(AlignString);
//				Alert0(A);
//				Alert0(B);
//				Alert0(M);

				Aligns.push_back(single);
				this->SortLastElement();
			}
		}
	}

//	Alert0("finished now order");

	CWaitCursor dummy;

//	Order();

}

void TwoSeqs::Order(){
	int NData = Aligns.size();
	if (NData<2) return;

	bool ordered;

	do{
		ordered = true;
		for (int i=0; i<(NData-1); i++) {
			if (Aligns[i].Score < Aligns[i + 1].Score){
//			if (data[i] > data[i + 1]){
				ordered = false;
				SingleScoreClassTwoSeqs save = Aligns[i];
				Aligns[i] = Aligns[i + 1];
				Aligns[i + 1] = save;
			}
		}
	}
	while (!ordered);
}

void TwoSeqs::SortLastElement(){
	int n = Aligns.size();
	if (n<2) return;

	double s = Aligns[n-1].Score;

	SingleScoreClassTwoSeqs single = Aligns[n-1];

	for (int i=0; i<(n-1); i++){
		if (Aligns[i].Score<s){ // replace here
			for (int j=(n-1); j>i; j--){
				Aligns[j] = Aligns[j-1];
			}
			Aligns[i]=single;
			return;
		}
	}
}
