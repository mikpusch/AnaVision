#pragma once

// a class to keep a number of variables. Used in Ana for user defined functiona fit
// like a mini program
#include <vector>
#include "FunctionParser.h"

using namespace std ;

#define MaxNumberOfVariablesInPool 500

class VariablePool{
public:
	VariablePool();
	~VariablePool();
	void Clear();

	int GetNVar();

	bool AddVariable(CString c);

	bool Ready();
	int MaxIndexUsed();

	bool LoadFromFile(CFile * fp);
	bool SaveOnFile(CFile * fp);

	void MakeString(CString & c);
	bool GetFromStrings(vector<CString> & StringVector);
	bool GetFromStrings(vector<CString> & StringVector, int & WrongLine, CString & ErrorMessage);

//    void operator=(VariablePool & v );

	//	int NumberOfVariables; // used in user func fit
//	vector <Variable> Variables;
	Variable Variables[MaxNumberOfVariablesInPool];

//	double * w ; // The internal variables!

	double w[MaxNumberOfVariablesInPool];

	bool Eval(double v, double * a, vector <double> & w);
	bool Eval(double v, double * a, double * w);
	bool Eval(double v, double * a);

private:
	bool ReadVariableFromString(int indextofind, CString string, bool & erroroccured, 
				CString & LastParseModelError, CString & VariableString);

	bool ready;
	int NVar;

	int SizeW;

	void Dealloc();
	void Dealloc(VariablePool * pP);
	void Alloc();

};