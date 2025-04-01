#include "stdafx.h"

#include "..\common\VariablePool.h"
#include "..\common\utils.h"
#include "..\common\nrutil.h"
#include "..\common\FunctionParser.h"

VariablePool::VariablePool(){
//	Variables.reserve(10000);
	ready = true;
	for (int i=0; i<MaxNumberOfVariablesInPool; i++){
		w[i]=0;
		Variables[i].f = NULL;
		Variables[i].FunctionString = CString("");
	}
	NVar = 0;
//	w = NULL;
//	SizeW = 0;
}
VariablePool::~VariablePool(){
//	Clear();
/*
int NVar = Variables.size();
	for (int i=0; i<NVar; i++){
		Variables[i].f = NULL;
	}
	Dealloc();
*/
}
int VariablePool::GetNVar(){
	return NVar;
}
void VariablePool::Clear(){
	Dealloc(this);
	NVar = 0;
}

void VariablePool::Dealloc(VariablePool * pP){
	if (!pP) return;
	for (int i=0; i<MaxNumberOfVariablesInPool; i++){
		Variables[i].FunctionString = CString("");
		if (Variables[i].f){
			delete Variables[i].f;
			Variables[i].f = NULL;
		}
	}
}
void VariablePool::Dealloc(){
/*
if (w){
		delete [] w;
		w = NULL;
	}
*/
}
void VariablePool::Alloc(){
/*
int NVar = Variables.size();
	if (NVar<SizeW){
		return;
	}
	double * NewW = new double[NVar];
	for (int i=0; i<SizeW; i++){
		NewW[i]=w[i];
	}
	for (int i=SizeW; i<NVar; i++){
		NewW[i] = 0;
	}
	Dealloc();
	w = NewW;
	SizeW = NVar;
*/
}

/*
void VariablePool::operator = (VariablePool & v )   { 
	int NVar = v.Variables.size();
//	ShowFloat(NVar, "NVar");
	Variables.resize(NVar);
	FunctionParser fp;
	for (int i=0; i<NVar; i++){
		Variables[i].FunctionString = v.Variables[i].FunctionString;
		Variables[i].f = fp.MakeFunction(Variables[i].FunctionString );
	}
	Dealloc();
	if (v.w){
		SizeW = v.SizeW;
		w = new double[SizeW];
		for (int i=0; i<SizeW; i++){
			w[i] = v.w[i];
		}
	}

}
*/

bool VariablePool::AddVariable(CString c){

	if (NVar>=MaxNumberOfVariablesInPool){
		Alert0("Error: too many variables in Load file");
		return false;
	}

	Variables[NVar].FunctionString = CString("");
	if (Variables[NVar].f){
		delete Variables[NVar].f;
		Variables[NVar].f = NULL;
	}
	Variables[NVar].FunctionString = c;

	FunctionParser fp;
	Variables[NVar].f=fp.MakeFunction(c);
	if (Variables[NVar].f != NULL){
		NVar++;
		return true;
	}
	else{
		return false;
	}

}

bool VariablePool::LoadFromFile(CFile* fp){
	ready = true;
	int NumberOfVariables;
	ReByNS(NumberOfVariables);
	ShowFloat(NumberOfVariables, "NumberOfVariables");
	if (NumberOfVariables<0){
		Dealloc(this);
		NVar = 0;
		return false;
	}
	if (NumberOfVariables>MaxNumberOfVariablesInPool){
		Alert0("Error: too many variables in Load file");
		Dealloc(this);
		NVar = 0;
		return false;
	}

//	Variables.resize(NumberOfVariables);
	for (int i=0; i<NumberOfVariables; i++){
		if (!Variables[i].LoadFromFile(*fp)){
			NVar = i;
//			Variables.resize(i);
			Alloc();
			return false;
		}
	}
	NVar = NumberOfVariables;
	Alloc();
	return true;
}

bool VariablePool::SaveOnFile(CFile* fp){
//	int NumberOfVariables = Variables.size();
	int NumberOfVariables = this->GetNVar();
	ShowFloat(NumberOfVariables, "NumberOfVariables");
	WriByNS(NumberOfVariables);
	for (int i=0; i<NumberOfVariables; i++){
		if (!Variables[i].SaveOnFile(*fp)){
			return false;
		}
	}
	return true;
}

void VariablePool::MakeString(CString & c){
	int NumberOfVariables = this->GetNVar();
//	int NumberOfVariables = Variables.size();
	for (int i=0; i<NumberOfVariables; i++){
		c += CString("w[");
		char s[20];
		_itoa(i, s, 10);
		c += CString(s) + CString("]=")+Variables[i].FunctionString;
		if (i<(NumberOfVariables-1)){
			c += "\r\n";
		}
	}
}

bool VariablePool::ReadVariableFromString(int indextofind, CString string, 
										  bool & erroroccured, CString & LastParseModelError, 
										  CString & VariableString){
	//w[0]= a[0]
	erroroccured = true;

	string.TrimLeft();
	string.TrimRight();

	CString comp1 = _T("w[");
	int cl = comp1.GetLength();
	int l = string.GetLength();

	if (l<=(cl+3)){ // must be at least w[.]=
		LastParseModelError = _T("String too short: must be at least w[.]=");
		return false;
	}

	CString l1=string.Left(cl);
//	Alert0(l1);

	if (comp1.CompareNoCase(l1)!=0){ // i.e. not equal "w["
//		Alert0("is not w[");
		LastParseModelError =_T("Variable must begin with 'w['");
		return false;
	}

	string = string.Right(l-cl); // e.g. "w[0]= a[0]" -> "0]= a[0]"

	string.TrimLeft();

	// now find "]"

	CString find0=_T("]");

	int r0 = string.Find(find0, 0);
	if (r0<=0){
		LastParseModelError =_T("not found ]");
		return false;
	}

	CString indexstring = string.Left(r0);

	int index = atoi(CStringA(indexstring));
	if (index != indextofind){
//	ShowFloat(index, "index");
//	if (index != this->NVariables){
		char ss[20];
		_itoa(indextofind, ss, 10);
		LastParseModelError = CString(_T("bad index invariable: expected: "))+CString(ss);
		return false;
//		return false;
	}

	string = string.Right(string.GetLength()-r0-1);

	CString find1= _T("=");

	int r1=string.Find(find1, 0);

	if (r1<0){
		LastParseModelError = _T("Could not find '='");
		return false;
	}

	string = string.Right(string.GetLength()-r1-1);

//	Alert0(string);


//	BOOL result = this->AddVariableAtEnd(string);
	VariableString = string;

/*
if (!result){
		LastParseModelError = _T("Error adding variable to model");
		return false;
	}

*/
	erroroccured = false;

	return true;

}

bool VariablePool::GetFromStrings(vector<CString> & StringVector){
	int WrongLine;
	CString ErrorMessage;
	return GetFromStrings(StringVector, WrongLine, ErrorMessage);
}


bool VariablePool::GetFromStrings(vector<CString> & StringVector, int & WrongLine, CString & ErrorMessage){

	ready = true;
	bool NoErrorOccured=true;

	FunctionParser fp;

	int index = 0;
	int indexvar = 0;
	bool erroroccured=false;
	Dealloc(this);
	NVar = 0;
//	Variables.resize(0);
	while (true){ // Read variables
		if (!GetNextNonEmptyString(index, StringVector)){
//				SuppressMessages = FALSE;
			Alloc();
			return NoErrorOccured;
		}
//			Alert0(StringVector[index]);
		CString newvariablestring;
		bool result = ReadVariableFromString(indexvar, StringVector[index],
			erroroccured, ErrorMessage, newvariablestring);
		if (!result){
			NoErrorOccured = false;
			WrongLine = index;
			return false;
		}

		if (erroroccured) NoErrorOccured = false;
		if (result){
			if (erroroccured){
//					FirstBadIndex = index;
//					SuppressMessages = FALSE;
				ready = false;
				NoErrorOccured = false;
				return false;
			}
//ShowFloat(indexvar, "indexvar BEF");
			NVar++;
//			Variables.resize(indexvar+1);
//ShowFloat(indexvar, "indexvar AFT");
			Variable & var = Variables[indexvar];
			var.FunctionString = newvariablestring;
//		Alert0(var.FunctionString );
			var.f = fp.MakeFunction(newvariablestring);
			if (!var.f){
				ready = false;
				NoErrorOccured = false;
				ErrorMessage = "Could not parse";
				WrongLine = index;
				return false;
//			NVar = indexvar;
//			Variables.resize(index);
//			return false;
			}
	
			indexvar++;
		}
		index++;
		
	}

	Alloc();
	return NoErrorOccured;

//	return ready;
}


bool VariablePool::Eval(double v, double * a, vector <double> & vectorw){
	if (!ready){
		return false;
	}
//	int NVar = Variables.size();
	vectorw.resize(NVar);
	for (int i=0; i<NVar; i++){
		Variable & var = Variables[i];
		w[i] = var.f->eval(v, a, NULL, w, NULL, NULL);
		vectorw[i] = w[i];
	}
	return true;
}
bool VariablePool::Eval(double v, double * a, double * w){
	if (!ready){
		return false;
	}
//	int NVar = Variables.size();
	for (int i=0; i<NVar; i++){
		Variable & var = Variables[i];
		w[i] = var.f->eval(v, a, NULL, w, NULL, NULL);
	}
	return true;
}

bool VariablePool::Eval(double v, double * a){
	if (!ready){
		return false;
	}
//	int NVar = Variables.size();
	for (int i=0; i<NVar; i++){
		Variable & var = Variables[i];
		w[i] = var.f->eval(v, a, NULL, w, NULL, NULL);
	}
	return true;
}

bool VariablePool::Ready(){
	return ready;
}
int VariablePool::MaxIndexUsed(){
	int MaxIndex = -1;
	if (!ready) return -1;

//	int NVar = Variables.size();
	for (int i=0; i<NVar; i++){
		Variable & var = Variables[i];
		int index = var.f->MaxIndexUsed();
		if (index>MaxIndex){
			MaxIndex = index;
		}
	}
	return MaxIndex;
}
