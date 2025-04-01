
/**

This class implements the sliding window sequence comparsion of two sequences

 */
#pragma once;

#include <vector>

using namespace std ;

class SingleScoreClassTwoSeqs{
public:
	SingleScoreClassTwoSeqs();
	~SingleScoreClassTwoSeqs();

	double GetScore();

	int IndexStartA, LengthA;
	int IndexStartB, LengthB;
	CString A, B, M;
	bool Evaluated;
	double Score;

	bool WriteOnFile(CFile & file);
	bool ReadFromFile(CFile & file);
};

class TwoSeqs {
public:
	TwoSeqs();
	~TwoSeqs();

	bool WriteOnFile(CFile & file);
	bool ReadFromFile(CFile & file);


	void Reset(); // sets the number of results to 0;
	void Restart(CDC * pDC = NULL);

	// Members

	CString SeqA, SeqB;

	int WindowLengthA, WindowLengthB;

	int GapOpenPenalty, GapExpandPenalty; 
	int NResultsToShow;

	vector <SingleScoreClassTwoSeqs> Aligns;
	void SortLastElement();
	void Order();

	
};

