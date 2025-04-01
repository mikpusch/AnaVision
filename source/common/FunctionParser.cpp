// FunctionParser.cpp : implementation file
//

#include "stdafx.h"
//#include "../StateEditor/StateEditor.h"
#include <Math.h>
#include "FunctionParser.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


Variable::Variable(){
	FunctionString=CString("");
	f=NULL;
}


Variable::~Variable(){
	if (f){
//		delete f;
	}
	f=NULL;
}

bool Variable::SaveOnFile(CFile & file){
	SaveString(FunctionString, file);
	return true;
}

bool Variable::LoadFromFile(CFile & file){
	if (!LoadString(FunctionString, file)){
		return false;
	}
	FunctionParser fp;
	if (f) delete f;
	f=NULL;
	f=fp.MakeFunction(FunctionString);
	return (f != NULL);
}

void Variable::Copy(Variable & g){
	g.FunctionString = FunctionString;
	FunctionParser fp;
	Function::Clear(g.f);
	g.f = fp.MakeFunction(g.FunctionString);
}


/*
void Variable::operator = (const Variable & v )   { 
	FunctionString = v.FunctionString;
	FunctionParser fp;
	f = fp.MakeFunction(FunctionString);
}
*/

int Variable::CheckVariable(bool * valid, int MaxAllowed){	// 0: invalid use of variables
										// 1: not defined (NIL)
										// 2: defined and ok
	if (f==NULL) return 1;
	return f->CheckVariable(valid, MaxAllowed);
}



double My_step(double arg1){
	if (arg1<0.0) return 0.0;
	return 1.0;
}

double My_plus(double arg1, double arg2){
	return arg1 + arg2;
}
double My_minus(double arg1, double arg2){
	return arg1 - arg2;
}
double My_times(double arg1, double arg2){
	return arg1*arg2;
}
double My_divide(double arg1, double arg2){
	return arg1 / arg2;
}

double My_inv(double arg1){
	return 1.0/arg1;
}
double My_fabs(double arg1){
	return fabs(arg1);
}
double My_log(double arg1){
	return log(arg1);
}

double My_negative(double arg1){
	return -arg1;
}

double My_exp10(double arg1){
	return pow(10.0, arg1);
}
double My_sqrt(double arg1){
	return sqrt(arg1);
}
double My_pow(double arg1, double arg2){
	return pow(arg1, arg2);
}

double My_exp(double arg1){
	return exp(arg1);
}

double My_log10(double arg1){
	return log10(arg1);
}

double My_sinus(double arg){
	return sin(arg);
}
double My_cosinus(double arg){
	return sin(1.570796326794897-arg); // pi/2 - x
}

bool IsNumber(CString c, bool first){
	if (c==CString("0")) return true;
	if (c==CString("1")) return true;
	if (c==CString("2")) return true;
	if (c==CString("3")) return true;
	if (c==CString("4")) return true;
	if (c==CString("5")) return true;
	if (c==CString("6")) return true;
	if (c==CString("7")) return true;
	if (c==CString("8")) return true;
	if (c==CString("9")) return true;
	if (c==CString(".")) return true;
	if ((c==CString("-")) & first) return true;
	return false;
}

double GetNumber(CString InString, int & length){
	CString c=InString.Left(1);
	int l=InString.GetLength();
	int i=1;
	bool expfound = false;
	do{
		if (i>l) break;
		CString c2 = InString.Mid(i,1);
		if (IsNumber(c2, 0)){
			c += c2;
		}
		else{
			if (expfound | (c2 != CString("E"))){
				break;
			}
			i++;
			if (i>l){
				break;
			}
			CString c3=InString.Mid(i,1);
			if (IsNumber(c3, 0)){
				c += c2 + c3;
				expfound=1;
			}
			else{
				if (c3==CString("-")){
					i++;
					if (i>l){
						break;
					}
					CString c4=InString.Mid(i,1);
					if (!IsNumber(c4, 0)){
						break;
					}
					c += c2 + c3 + c4;
					expfound=1;
				}
				else{
					break;
				}
			}
		}
		i++;
	}
	while (1);
    length = c.GetLength();
	return atof(CStringA(c));
}



/////////////////////////////////////////////////////////////////////////////
// FunctionParser

IMPLEMENT_DYNCREATE(FunctionParser, CCmdTarget)

FunctionParser::FunctionParser(){
	LastError=CString("");
//	Funcs=NULL;
}

FunctionParser::~FunctionParser()
{
/*
	if (Funcs){
		//DeleteFuncs();
		delete [] Funcs;
		Funcs=NULL;
	}
*/
}

bool FunctionParser::CheckUserFunc(CString c, int & UserIndex){
	// Minimum is: FUNC[0] i.e. 7 characters
	int l=c.GetLength();
	if (l<7) return false;
	CString test1 = c.Left(5);
	if (test1!=CString("FUNC[")) return false;
	int findresult = c.Find(']');
	if (findresult==(-1)){
		return false;
	}

//	ShowFloat(findresult, "find ]");

	CString s2 = c.Mid(5,findresult-5);
//	Alert0(s2);
	UserIndex = atoi(CStringA(s2));
//	ShowFloat(UserIndex, "User index");
	return true;
}

int FunctionParser::GetUnaryFunc(CString & c,	
				  unaryfuncptr & lunaryfunction){

//	Alert0(CString("enter getunaryfunc :") + c);

	if (c == CString("STEP")){
		lunaryfunction = My_step;
		return 1;
	}
	if (c == CString("EXP")){
//		Alert0("exp");
		lunaryfunction = My_exp;
		return 1;
	}
	if (c == CString("EXP10")){
//		Alert0("expten");
		lunaryfunction = My_exp10;
		return 1;
	}
	if (c == CString("SIN")){
//		Alert0("sin");
		lunaryfunction = My_sinus;
		return 1;
	}
	if (c == CString("COS")){
//		Alert0("sin");
		lunaryfunction = My_cosinus;
		return 1;
	}
	if (c == CString("INV")){
		lunaryfunction = My_inv;
		return 1;
	}
	if (c == CString("SQRT")){
		lunaryfunction = My_sqrt;
		return 1;
	}
	if (c == CString("LOG")){
		lunaryfunction = My_log;
		return 1;
	}
	if (c == CString("LOG10")){
		lunaryfunction = My_log10;
		return 1;
	}
	if (c == CString("FABS")){
		lunaryfunction = My_fabs;
		return 1;
	}
	return 0;
}


Function * FunctionParser::NewFunc(){
	Function * result = new Function;
//	Funcs[NFunc]=result;
//	NFunc++;
	return result;
}

void FunctionParser::RemoveFunc(Function * f){
	if (!f) return;
	if (f->firstarg){
		RemoveFunc(f->firstarg);
		f->firstarg = NULL;
		if (f->secondarg){
			RemoveFunc(f->secondarg);
			f->secondarg = NULL;
		}
	}
	f->firstarg = NULL;
	f->secondarg = NULL;
	delete f;
}

void FunctionParser::DeleteFuncs(){
	for (int i=0; i<NFunc; i++){
//		RemoveFunc(Funcs[i]);
	}
}


BEGIN_MESSAGE_MAP(FunctionParser, CCmdTarget)
	//{{AFX_MSG_MAP(FunctionParser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FunctionParser message handlers

double Function::eval(double v, double * darray, double * p, double * variables,
//					  Function * UserFuncs){
					  Variable * UserFuncs, double ** rates){

  switch (Type) {
	case 0 : return value; break;
	case 1 : return v;		break;
	case 2 : return darray[arrayindex]; break;
	case 3 : return (*unaryfunction)(firstarg->eval(v, darray, p, variables, UserFuncs, rates));
			  break;
	case 4 : return (*binaryfunction)
				     ( firstarg->eval (v, darray, p, variables, UserFuncs, rates),
					   secondarg->eval(v, darray, p, variables, UserFuncs, rates));
			break;
	case 5: return variables[arrayindex]; break;
	case 6: {
				double arg = firstarg->eval(v, darray, p, variables, UserFuncs, rates);
//				return UserFuncs[arrayindex].eval(arg, array, variables, UserFuncs);
				return UserFuncs[arrayindex].f->eval(arg, darray, p, variables, UserFuncs, rates);
			}
		break;
	case 7: return p[arrayindex]; break;
	case 8: return rates[arrayindex][secondarrayindex]; break;
	default : return 0; //
	}
}

bool Function::NeedParenthesisForMultiply(){
  switch (Type) {
	case 0 : return false; break;
	case 1 : return false;		break;
	case 2 : return false; break;
	case 3 : return false; break;
	case 4 : if (binaryfunction==My_times){
					return false;
			 }
			 else{
				 return true;
			 }
			break;
	case 5: return false; break;
	case 6: return false; break;
	case 7: return false; break;
	case 8: return false; break;
	default : return false; //
	}
}

double Function::evalwithcheck(double v, double * darray, double * p, double * variables,
//					  Function * UserFuncs){
					  Variable * UserFuncs, double ** rates){

  switch (Type) {
	case 0 : return value; break;
	case 1 : return v;		break;
	case 2 : {
				if (darray){
					return darray[arrayindex];
				}
				else{
					return 0;
				}
				break;
			}
	case 3 : return (*unaryfunction)(firstarg->evalwithcheck(v, darray, p, variables, UserFuncs, rates));
			  break;
	case 4 : return (*binaryfunction)
				     ( firstarg->evalwithcheck (v, darray, p, variables, UserFuncs, rates),
					   secondarg->evalwithcheck (v, darray, p, variables, UserFuncs, rates));
			break;
	case 5: {
				if (variables){
					return variables[arrayindex];
				}
				else{
					return 0;
				}
				break;
			}
	case 6: {
				if (UserFuncs){
					double arg = firstarg->evalwithcheck(v, darray, p, variables, UserFuncs, rates);
					return UserFuncs[arrayindex].f->evalwithcheck(arg, darray, p, variables, UserFuncs, rates);
				}
				else{
					return 0;
				}
			}
		break;
	case 7: {
				if (p){
					return p[arrayindex];
				}
				else{
					return 0;
				}
			}
			break;
	case 8: {
				if (rates){
					return rates[arrayindex][secondarrayindex];
				}
				else{
					return 0;
				}
			}
			break;
	default : return 0; //
	}

}


int Function::MaxIndexUsed(){
	return HelpMaxIndexUsed(-1);
}


int Function::HelpMaxIndexUsed(int result){
	if (Type==0) return result;
	if (Type==1) return result;
	if (Type==5) return result;
	if ((Type==2) | (Type ==8)){
		if (result<=arrayindex){
			result=arrayindex;
		}
		if (Type==8){
			if (result<=secondarrayindex){
				result=secondarrayindex;
			}
		}
		return result;
	}
	if (Type==3){
	  return (firstarg->HelpMaxIndexUsed(result));
    }
	if (Type==6){
	  return (firstarg->HelpMaxIndexUsed(result));
    }
	if (Type==4){
		result = firstarg->HelpMaxIndexUsed(result);
		return secondarg->HelpMaxIndexUsed(result);
	}
	return result;
}

int Function::CheckVariable(bool * valid, int MAXVARALLOWED){
	if (Type == 0) return 2; // value
	if (Type == 1) return 2; //x	
	if (Type == 2) return 2; // array
	if (Type == 8) return 2; // matrix 
	if (Type == 7){			// array of probablities
		if (arrayindex<0){
			return 0;
		}
		return 2; 
	}
	if (Type == 5){			// w[]
		if (arrayindex<0){
			return 0;
		}
		if (arrayindex>=MAXVARALLOWED){
			return 0;
		}
		if (!valid[arrayindex]){
			return 0;
		}
		return 2;
	}
	if (Type==3){ // unary function 
	  return firstarg->CheckVariable(valid, MAXVARALLOWED);
    }
	if (Type==6){ // user function  func[]()
		if (arrayindex<0){
			return 0;
		}
		if (arrayindex>=MAXVARALLOWED){
			return 0;
		}
		return firstarg->CheckVariable(valid, MAXVARALLOWED);
    }
	if (Type==4){	// binary func
		int res1=firstarg->CheckVariable(valid, MAXVARALLOWED);
//		if (res1==1){
//			return 1;
//		}
		if (res1==0) return 0;
		int res2=secondarg->CheckVariable(valid, MAXVARALLOWED);
//		if (res2==1) return 1; 
		if (res2==0) return 0;
		return 2;
	}
	return 0;
}

int Function::MaxVariableUsed(){
	return HelpMaxVariableUsed(-1);
}

int Function::HelpMaxVariableUsed(int result){
	if (Type==0) return result;
	if (Type==1) return result;
	if (Type==2) return result;
	if (Type==5){
		if (result>arrayindex){
			return result;
		}
		else{
			return arrayindex;
		}
	}
	if ((Type==3) | (Type==6)){
	  return (firstarg->HelpMaxVariableUsed(result));
    }
	if (Type==4){
		int res1=(firstarg->HelpMaxVariableUsed(result));
		int res2=(secondarg->HelpMaxVariableUsed(result));
		if (res1>res2) return res1;
		return res2;
	}
	return result;
}


void Function::Remove(){
	//Alert0("enter remove ");


	if (firstarg){
		firstarg->Remove();
		firstarg = NULL;
	}
	if (secondarg){
		secondarg->Remove();
		secondarg = NULL;
	}

	//delete this;

}

Function::Function()
{
//	unaryfunction = NULL;
//	binaryfunction = NULL;
	firstarg = NULL;
	secondarg = NULL;
}

Function::~Function()
{

//	return;
//	Remove();
//	return;

	if (firstarg){
//		firstarg->~Function();
		delete firstarg;
		firstarg = NULL;
		if (secondarg){
//		secondarg->~Function();
			delete secondarg;
			secondarg = NULL;
		}
	}
//	unaryfunction = NULL;
//	binaryfunction = NULL;
//	delete this;
}

Function * FunctionParser::MakeFunction(CString instring, bool * UsesVoltageVariable){
	CString s=instring;
	s.MakeUpper();
	CString out=CString("");
	int l=s.GetLength();
	int i;
	for (i=0; i<l; i++){
		CString c=s.Mid(i,1);
		if (!c.Compare(_T("'"))){
			break;
		}
		if (c.Compare(_T(" "))){
			out += c;
		}
	}
	ParseError=0;
	NFunc=0;

//	Funcs = new Function *[s.GetLength()*10];

	bool ErrorOccured=false;
//	Alert0("Hi call aux");
	Function * result = ParseAux(out, ErrorOccured, UsesVoltageVariable);
	if (ErrorOccured | (!result)){
		if (ErrorOccured){
			if (result){
//				Alert0("hi, delte result");
				delete result;
			}
		}
		result = NULL;
		//DeleteFuncs();
	}
	return result;
}

int GetCorrespondingRightParentIndex(int index, int count, CString & is){
	int l=is.GetLength();
	if (index>=l) return -1;
	CString f=is.Mid(index, 1);
	if (f.Compare(_T("("))==0){
	    return GetCorrespondingRightParentIndex(index+1, count+1, is);
	}
	if (f.Compare(_T(")"))==0){
		if (count==1){
			return index;
		}
		else{
		    return GetCorrespondingRightParentIndex(index+1, count-1, is);
		}
	}
    return GetCorrespondingRightParentIndex(index+1, count, is);
}

binaryfuncptr GetBinaryOp(CString c, int & priority, bool & ErrorOccured){
	if (c==CString("^")){
		priority=0;
		return My_pow;
	}
	if (c==CString("/")){
		priority=1;
		return My_divide;
	}
	if (c==CString("*")){
		priority=2;
		return My_times;
	}
	if (c==CString("+")){
		priority=4;
		return My_plus;
	}
	if (c==CString("-")){
		priority=3;
		return My_minus;
	}
	ErrorOccured=true;
	return NULL;
}

Function * FunctionParser::ParseAuxOld(CString is, bool & ErrorOccured){
	//Alert(CString("Enter new parse with: ")+is);
	ErrorOccured=false;
	int l= is.GetLength();
	if (l==0) return NULL;

	Function * exprs[MAXEXPRESSIONALLOWEDINPARSE];

	exprs[0] = NULL;

	binaryfuncptr bf[MAXEXPRESSIONALLOWEDINPARSE];
	int priority[MAXEXPRESSIONALLOWEDINPARSE];

	int nexprs=0;
	bool GoOn=true;
	while (GoOn){
		//Alert0(is);
		Function * e = GetFirstExpr(is, ErrorOccured);
		if (ErrorOccured){
			//return NULL;
			return exprs[0];
		}
		if (e){
			exprs[nexprs]=e;
			int newl=is.GetLength();
			if (!newl){
				GoOn=false;
			}
			else{
				CString op=is.Mid(0,1); // operator string, i.e. *, ^, /, +, -
				is=is.Right(newl-1);
				//Alert(CString("op = ")+op+CString(" new is = ")+is);
				bf[nexprs] = GetBinaryOp(op, priority[nexprs], ErrorOccured);
				if (ErrorOccured){
					//return NULL;
					return exprs[0];
				}
			}
			nexprs++;
			if ((nexprs+1)>=MAXEXPRESSIONALLOWEDINPARSE){
				Alert0(_T("EXPRESSION TOO COMPLICATED"));
				ErrorOccured=true;
				//return NULL;
				return exprs[0];
			}
		}
		else{ // i.e. no expression found, but no error
			GoOn=false;
		}
	} // end while(GoOn)

	if (nexprs==0){
		ErrorOccured=true;
		//return NULL;
		return exprs[0];
	}

	while (1){
		if (nexprs==1){
			return exprs[0];
		}
		int mini=0;
		int minp=priority[0];
		int i;
		for (i=1; i<(nexprs-1); i++){
			if (priority[i]<minp){
				minp=priority[i];
				mini=i;
			}
		}
		Function * nf = NewFunc();
		nf->Type=4;
		nf->firstarg=exprs[mini];
		nf->secondarg=exprs[mini+1];
		nf->binaryfunction=bf[mini];
		for (i=(mini+2); i<nexprs; i++){
			exprs[i-1]=exprs[i];
		}
		for (i=(mini+1); i<(nexprs-1); i++){
			bf[i-1]=bf[i];
			priority[i-1]=priority[i];
		}
		exprs[mini]=nf;
		nexprs--;
	}

}
void Function::Clear(Function * f){
	if (f) delete f;
}

void Function::operator=(Function &f )
      { 
		  Copy(f);
      }
/*void Function::operator=(Function * & f )
      { 
		  Copy(f);
      }
	  */

Function * Function::Copy(){
	if (!this) return NULL;
	Function * g = new Function;
	Copy(*g);
	return g;
}


void Function::Copy(Function & g){
//	Alert0("enter copy");
//	return;
	g.Remove();

	g.arrayindex = this->arrayindex;
	g.binaryfunction = this->binaryfunction;
	if (this->firstarg){
		g.firstarg=new Function;
		this->firstarg->Copy(*g.firstarg);
		if (this->secondarg){
			g.secondarg = new Function;
			this->secondarg->Copy(*g.secondarg);
		}
	}
	g.secondarrayindex = this->secondarrayindex;
	g.Type=this->Type;
	g.unaryfunction = this->unaryfunction;
	g.value=this->value;
}
/*
void Function::Copy(Function * & g){
	Alert0("enter copy pointer");
	if (!g){
		g= new Function;
	}
	Copy(*g);
}
*/

Function * FunctionParser::ParseAux(CString is, bool & ErrorOccured, bool * UsesVoltageVariable){
	//Alert0(CString("Enter new parse with: ")+is);
	ErrorOccured=false;
	int l= is.GetLength();
	if (l==0) return NULL;

	Function ** exprs = new Function * [MAXEXPRESSIONALLOWEDINPARSE];
	for (int i=0; i<MAXEXPRESSIONALLOWEDINPARSE; i++){
		exprs[i] = NULL;
	}
//	exprs[0] = NULL;

	binaryfuncptr bf[MAXEXPRESSIONALLOWEDINPARSE];
	int priority[MAXEXPRESSIONALLOWEDINPARSE];

	int nexprs=0;
	bool GoOn=true;
	while (GoOn){
		//Alert0("Before enter get first"+ is);
		Function * e = GetFirstExpr(is, ErrorOccured, UsesVoltageVariable);
		//Alert0("after enter get first"+ is);
		if (ErrorOccured){
			//return NULL;
			/////XXXXFunction * result = new Function;
//			result = exprs[0];
			/////XXXXexprs[0]->Copy(*result);
			for (int i=0; i<MAXEXPRESSIONALLOWEDINPARSE; i++){
				if (exprs[i]){
					delete exprs[i];
					exprs[i]=NULL;
				}
			}

			delete [] exprs;
			return NULL;
//			return result;
		}
		if (e){
			exprs[nexprs]=e;
			int newl=is.GetLength();
			if (!newl){
				GoOn=false;
			}
			else{
				CString op=is.Mid(0,1); // operator string, i.e. *, ^, /, +, -
				is=is.Right(newl-1);
				//Alert(CString("op = ")+op+CString(" new is = ")+is);
				//Alert0("Eneter get bin op with "+op);
				bf[nexprs] = GetBinaryOp(op, priority[nexprs], ErrorOccured);
				if (ErrorOccured){
					//Alert0("ERROR OCCURED in GetBinaryOp");
					//return NULL;
					Function * result = new Function;
					exprs[0]->Copy(*result);
					for (int i=0; i<MAXEXPRESSIONALLOWEDINPARSE; i++){
						if (exprs[i]){
							delete exprs[i];
							exprs[i]=NULL;
						}
					}
					delete [] exprs;
					return result;
					//return exprs[0];
				}
			}
			nexprs++;
			if ((nexprs+1)>=MAXEXPRESSIONALLOWEDINPARSE){
				Alert0(_T("EXPRESSION TOO COMPLICATED"));
				ErrorOccured=true;
				//return NULL;
				Function * result = new Function;
				exprs[0]->Copy(*result);
				for (int i=0; i<MAXEXPRESSIONALLOWEDINPARSE; i++){
					if (exprs[i]){
						delete exprs[i];
						exprs[i]=NULL;
					}
				}
				delete [] exprs;
				return result;
//				return exprs[0];
			}
		}
		else{ // i.e. no expression found, but no error
			GoOn=false;
		}
	} // end while(GoOn)

	if (nexprs==0){
		ErrorOccured=true;
		Function * result = exprs[0]->Copy();
//		Function * result = new Function;
//		exprs[0]->Copy(*result);
		for (int i=0; i<MAXEXPRESSIONALLOWEDINPARSE; i++){
			if (exprs[i]){
				delete exprs[i];
				exprs[i]=NULL;
			}
		}
		delete [] exprs;
		return result;
		//return NULL;
//		return exprs[0];
	}

	while (1){
//		ShowFloat(nexprs, "nexps");
		if (nexprs==1){
//			Function * result = new Function;
			Function * result = exprs[0]->Copy();
//			exprs[0]->Copy(*result);
			for (int i=0; i<MAXEXPRESSIONALLOWEDINPARSE; i++){
				if (exprs[i]){
					delete exprs[i];
					exprs[i]=NULL;
				}
			}
			delete [] exprs;
			return result;

//			return exprs[0];
		}
		int mini=0;
		int minp=priority[0];
		int i;
		for (i=1; i<(nexprs-1); i++){
			if (priority[i]<minp){
				minp=priority[i];
				mini=i;
			}
		}
		Function * nf = NewFunc();
		nf->Type=4;
//		nf->firstarg = NewFunc();
//		exprs[mini]->Copy(*nf->firstarg);
		nf->firstarg = exprs[mini]->Copy();
//		nf->firstarg=exprs[mini];
//		nf->secondarg = NewFunc();
//		exprs[mini+1]->Copy(*nf->secondarg);
		nf->secondarg = exprs[mini+1]->Copy();
//		nf->secondarg=exprs[mini+1];
		nf->binaryfunction=bf[mini];
		for (i=(mini+2); i<nexprs; i++){
			delete exprs[i-1];
			exprs[i-1]=exprs[i];
			exprs[i]=NULL;
		}
		for (i=(mini+1); i<(nexprs-1); i++){
			bf[i-1]=bf[i];
			priority[i-1]=priority[i];
		}
		delete exprs[mini];
		exprs[mini]=nf;
		nexprs--;
	}

}

Function * FunctionParser::GetFirstExpr(CString & is, bool & ErrorOccured, bool * UsesVoltageVariable){
	//Alert0(CString("Enter GetFirstEpr with: ")+is);
	ErrorOccured=false;
	int l=is.GetLength();
	if (l==0) return NULL;
	CString f=is.Mid(0,1);
	if (f.Compare(_T("]"))==0){
		ErrorOccured = true;
		return NULL;
	}
//	if ((f.Compare(_T("V"))==0)|(f.Compare(_T("X"))==0)|(f.Compare(_T("C"))==0)|(f.Compare(_T("T"))==0)){
	if ((f.Compare(_T("V"))==0)|(f.Compare(_T("X"))==0)|(f.Compare(_T("T"))==0)){
		//Alert("V in GetFirstExpr");
		if (UsesVoltageVariable){
			*UsesVoltageVariable = (f.Compare(_T("V"))==0);
		}

		is = is.Right(l-1);
		Function * nf = NewFunc();
		nf->Type=1;
		return nf;
	}
	if (f.Compare(_T("C"))==0){ // might be c as consencatrtaion or cos
//		Alert0("c");
		if (l==1){ // a single c!
//			Alert0("single c");
			if (UsesVoltageVariable){
				*UsesVoltageVariable = false;
			}
			is = is.Right(l-1);
			Function * nf = NewFunc();
			nf->Type=1;
			return nf;
		}
		CString NextChar = is.Mid(1,1);
		if (NextChar.Compare(_T("O"))==0){
//			Alert0("Must be cosine");
		}
		else{
//			Alert0("should be conc");
			if (UsesVoltageVariable){
				*UsesVoltageVariable = false;
			}
			is = is.Right(l-1);
			Function * nf = NewFunc();
			nf->Type=1;
			return nf;
		}
	}


	if (f.Compare(_T("("))==0){
		//Alert(" ( in GetFirstExpr");
		int rpindex=GetCorrespondingRightParentIndex(1, 1, is);
		if (rpindex<0){
			ErrorOccured=true;
			return NULL;
		}
		CString fs=is.Mid(1, rpindex-1);
		if (rpindex==(l-1)){ // i.e. nothing left
			is = CString("");
		}
		else{
			is = is.Mid(rpindex+1, l-rpindex-1);
		}
		return ParseAux(fs, ErrorOccured, UsesVoltageVariable);
	}
	if (f.Compare(_T("-"))==0){
		//Alert(" - in GetFirstExpr");
		if (l==1){
			ErrorOccured=true;
			return NULL;
		}
		CString s = is.Mid(1,1);
		if (IsNumber(s, false)){
			int ln;
			double r=GetNumber(is, ln);
			Function * f = NewFunc();
			f->Type=0;
			f->value=r;
			is = is.Right(l-ln);
			return f;
		}
		is = is.Right(l-1);
		Function * f2 = GetFirstExpr(is, ErrorOccured);
		if (ErrorOccured){
			return NULL;
		}
		if (f2==NULL){
			ErrorOccured=true;
			return NULL;
		}
		Function * f = NewFunc();
		f->Type=3; // UNARY FUNC;
		f->firstarg=f2;
		f->unaryfunction=My_negative;
		return f;
	}
	if (IsNumber(f, false)){
		//Alert(" is # in GetFirstExpr");
		int ln;
		double r=GetNumber(is, ln);
		Function * f = NewFunc();
		f->Type=0;
		f->value=r;
		is = is.Right(l-ln);
		return f;
	}
	if (f.Compare(_T("A"))==0){
		//Alert(" A in GetFirstExpr");
		if (l<2){
			ErrorOccured=true;
			return NULL;
		}
		CString c2;
		c2=is.Mid(1,1);
		if (c2 != CString("[")){
			LastError=CString("not [ after A");
			ErrorOccured=1;
			return NULL;
		}
		int findresult = is.Find(']');
		if (findresult==(-1)){
			ErrorOccured=1;
			LastError=CString("not found ]");
			return NULL;
		}
		CString s2 = is.Mid(2,findresult-2);
		int larrayindex = atoi(CStringA(s2));
		is = is.Right(l-findresult-1);
		//Alert(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=2;
		f->arrayindex=larrayindex;
		return f;
	}
	if (f.Compare(_T("R"))==0){
		//Alert(" R in GetFirstExpr"); // Min: R[0][0] = 7
		if (l<7){
			ErrorOccured=true;
			return NULL;
		}
		CString c2;
		c2=is.Mid(1,1);
		if (c2 != CString("[")){
			LastError=CString("not [ after R");
			ErrorOccured=1;
			return NULL;
		}
		int findresult = is.Find(']');
		if (findresult==(-1)){
			ErrorOccured=1;
			LastError=CString("not found ]");
			return NULL;
		}
		CString s2 = is.Mid(2,findresult-2);
		int larrayindex = atoi(CStringA(s2));
		is = is.Right(l-findresult-1);

		c2=is.Mid(0,1);
		if (c2 != CString("[")){
			LastError=CString("not [ after R[.]");
			ErrorOccured=1;
			return NULL;
		}
		findresult = is.Find(']');
		if (findresult==(-1)){
			ErrorOccured=1;
			LastError=CString("not found ]");
			return NULL;
		}
		s2 = is.Mid(1,findresult-2);
		int secondarrayindex = atoi(CStringA(s2));
		is = is.Right(is.GetLength()-findresult-1);


//		Alert0(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=8;
		f->arrayindex=larrayindex;
		f->secondarrayindex=secondarrayindex;
		return f;
	}
	if (f.Compare(_T("P"))==0){
		//Alert(" P in GetFirstExpr");
		if (l<2){
			ErrorOccured=true;
			return NULL;
		}
		CString c2;
		c2=is.Mid(1,1);
		if (c2 != CString("[")){
			LastError=CString("not [ after P");
			ErrorOccured=1;
			return NULL;
		}
		int findresult = is.Find(']');
		if (findresult==(-1)){
			ErrorOccured=1;
			LastError=CString("not found ]");
			return NULL;
		}
		CString s2 = is.Mid(2,findresult-2);
		int larrayindex = atoi(CStringA(s2));
		is = is.Right(l-findresult-1);
		//Alert(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=7;
		f->arrayindex=larrayindex;
		return f;
	}
	if (f.Compare(_T("W"))==0){
		//Alert(" W in GetFirstExpr");
		if (l<2){
			ErrorOccured=true;
			return NULL;
		}
		CString c2;
		c2=is.Mid(1,1);
		if (c2 != CString("[")){
			LastError=CString("not [ after W");
			ErrorOccured=1;
			return NULL;
		}
		int findresult = is.Find(']');
		if (findresult==(-1)){
			ErrorOccured=1;
			LastError=CString("not found ]");
			return NULL;
		}
		CString s2 = is.Mid(2,findresult-2);
		//Alert0(s2);
		int larrayindex = atoi(CStringA(s2));
		is = is.Right(l-findresult-1);
		//Alert0(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=5;
		f->arrayindex=larrayindex;
		return f;
	}

	// Now can only be unary func
//	Alert(" Can only be unary in GetFirstExpr");
	int lp=is.Find(_T("("));
	if (lp==(-1)){
		ErrorOccured=1;
		return false;
	}
	int rp=GetCorrespondingRightParentIndex(lp+1, 1, is);

	if ((rp<0)|(rp==(lp+1))){
		ErrorOccured=true;
		return NULL;
	}
//	ShowInt(lp, "lp");
//	ShowInt(rp, "rp");

	CString fs=is.Mid(lp+1, rp-lp-1);
	unaryfuncptr lunaryfunction;
	CString UF=is.Left(lp);
	//Alert0(UF);
	//Check first if user func:
	int UserIndex=0;
	bool IsUserFunc = CheckUserFunc(UF, UserIndex);

	if (!IsUserFunc){
		int result=GetUnaryFunc(UF, lunaryfunction);
//	Alert(CString("Get unary func with: ")+UF);
		if (!result){
			ErrorOccured=1;
			LastError=CString("bad unary func");
			return NULL;
		}
	}
//	Alert("unary func found");
	Function * arg = ParseAux(fs, ErrorOccured, UsesVoltageVariable);
	if (ErrorOccured){
		return NULL;
	}
	if (rp==(l-1)){ // i.e. nothing left
		is = CString("");
	}
	else{
		is = is.Mid(rp+1, l-rp-1);
	}
	Function * rf = NewFunc();
	rf->firstarg=arg;
	if (IsUserFunc){
		rf->Type = 6;
		rf->arrayindex = UserIndex;
	}
	else{
		rf->Type=3;
		rf->unaryfunction = lunaryfunction;
	}
	return rf;
}



bool FunctionParser::DecreaseVarIndex(CString & s, int var){
// starting from var all indexes are decrease by one
//ShowString(s, "s");
	if (s.GetLength()<3){
		return true;
	}

	CString in = s;
	CString out("");

	CString fl( "w[" );
	CString fr( "]"  );

//	int Find( LPCTSTR lpszSub ) const;
  //Return Value
//The zero-based index of the first character in this CString object
//that matches the requested substring or characters;
//-1 if the substring or character is not found.

	while(true){
		int length = s.GetLength();
		if (length<4) break;
		int res = s.Find(fl); // find w[
		if (res<0){
			break;
		}

		res += 2;

		out += s.Left(res);
//		ShowString(out, "out");
		s = s.Right(length-res);
		length = s.GetLength();

//		ShowString(s, "s");

		res = s.Find(fr);	// find ]
		if (res<=0){		// if not found or if the situation was w[]
			break;
		}
		CString number = s.Left(res);
		double d = strtod(CStringA(number), NULL);
		int index = int(d);
//		ShowString(number, "number");
//		ShowFloat(index, "index");
		if (index==var){
			s = in; // the variable is used!!!
			return false;
		}

		if (index>var){
			index --;
			if (index<0){
				s = in;
				return false;
			}
			char ss[20];
			_itoa(index, ss, 10);
			number = CString(ss);
		}
		out += number + fr;
		s = s.Right(length-res-1);
	}
	out += s;
	s = out;
	return true;
}




void FunctionParser::IncreaseVarIndex(CString & s, int var){
// starting from var+1 all indexes are decrease by one
//ShowString(s, "s");

	CString in = s;
	CString out("");

	CString fl( "w[" );
	CString fr( "]"  );

//	int Find( LPCTSTR lpszSub ) const;
  //Return Value
//The zero-based index of the first character in this CString object
//that matches the requested substring or characters;
//-1 if the substring or character is not found.

	while(true){
		int length = s.GetLength();
		if (length<4) break;
		int res = s.Find(fl); // find w[
		if (res<0){
			break;
		}

		res += 2;

		out += s.Left(res);
//		ShowString(out, "out");
		s = s.Right(length-res);
		length = s.GetLength();

//		ShowString(s, "s");

		res = s.Find(fr);	// find ]
		if (res<=0){		// if not found or if the situation was w[]
			break;
		}
		CString number = s.Left(res);
		double d = strtod(CStringA(number), NULL);
		int index = int(d);
//		ShowString(number, "number");
//		ShowFloat(index, "index");

		if (index>=var){
			index ++;
			char ss[20];
			_itoa(index, ss, 10);
			number = CString(ss);
		}
		out += number + fr;
		s = s.Right(length-res-1);
	}
	out += s;
	s = out;
}

bool FunctionParser::ChangePIndex(CString & s, int OldIndex, int NewIndex){
// starting from var all indexes are decrease by one
//ShowString(s, "s");

	CString in = s;
	CString out("");

	CString fl( "p[" );
	CString fr( "]"  );

//	int Find( LPCTSTR lpszSub ) const;
  //Return Value
//The zero-based index of the first character in this CString object
//that matches the requested substring or characters;
//-1 if the substring or character is not found.

	while(true){
		int length = s.GetLength();
		if (length<4) break;
		int res = s.Find(fl); // find p[
		if (res<0){
			break;
		}

		res += 2;

		out += s.Left(res);
//		ShowString(out, "out");
		s = s.Right(length-res);
		length = s.GetLength();

//		ShowString(s, "s");

		res = s.Find(fr);	// find ]
		if (res<=0){		// if not found or if the situation was w[]
			break;
		}
		CString number = s.Left(res);
		double d = strtod(CStringA(number), NULL);
		int index = int(d);
//		ShowString(number, "number");
//		ShowFloat(index, "index");

		if (index==OldIndex){
			index=NewIndex;
			char ss[20];
			_itoa(index, ss, 10);
			number = CString(ss);
		}
		out += number + fr;
		s = s.Right(length-res-1);
	}
	out += s;
	s = out;
	return true;
}




void Function::ShowFunction(){
	ShowFunction(0);
}

void Function::ShowFunction(int level){
	char s[20];
	CString c2;

	_itoa(level, s, 10);
	CString c = CString("level ")+CString(s)+CString("; ");

	switch (Type) {
	case 0 : _gcvt(value,6,s); c += CString("value = ")+ CString(s);
			Alert0(c); return ; break;
	case 1 : c += CString("x "); Alert0(c); return; break;
	case 2 : c += CString("a[");_itoa(arrayindex, s, 10); c += CString(s)+CString("]");
		     Alert0(c); return; break;
	case 3 : c += CString("unary func: ");
			GetUnaryFuncName(unaryfunction, c2);
			c += c2 + CString(". Follows argument");
			Alert0(c);
			firstarg->ShowFunction(level+1);
			return; break;
	case 4 : c += CString("binary func: ");
			GetBinaryFuncName(binaryfunction, c2);
			c += c2 + CString(". Follows argument1, then 2");
			Alert0(c);
			firstarg->ShowFunction(level+1);
			secondarg->ShowFunction(level+1);
			return;
			break;
	case 5: c += CString("w[");_itoa(arrayindex, s, 10); c += CString(s)+CString("]");
		     Alert0(c); return; break;
	case 6: {
			 c += CString("func["); _itoa(arrayindex, s, 10);
			 c += CString(s)+CString("]; follows arg");
			 Alert0(c);
			firstarg->ShowFunction(level+1);
			return; break;

			}
		break;
	case 7: c += CString("p[");_itoa(arrayindex, s, 10); c += CString(s)+CString("]");
		     Alert0(c); return; break;
	case 8: c += CString("r[");_itoa(arrayindex, s, 10); c += CString(s)+CString("][");
			_itoa(secondarrayindex, s, 10); c += CString(s)+CString("]");
		     Alert0(c); return; break;
	default : c += "ERROR"; Alert0(c); return ; //
	}


}


void Function::GetUnaryFuncName(unaryfuncptr f, CString & c){
	if (f==My_step) c="step";
	if (f==My_exp)  c="exp";
	if (f==My_exp10)  c="exp10";
	if (f==My_inv)  c="inv";
	if (f==My_sqrt) c="sqrt";
	if (f==My_log)  c="log";
	if (f==My_log10)  c="log10";
	if (f==My_fabs) c="fabs";
}

void Function::GetBinaryFuncName(binaryfuncptr f, CString & c){
	if (f==My_pow)    c="pow";
	if (f==My_divide) c="/";
	if (f==My_times)  c="*";
	if (f==My_plus)   c="+";
	if (f==My_minus)  c="-";
}


bool FunctionParser::DecreaseStateIndex(CString & s, int state){
	// starting from state all indexes in p[] expressions are decreases by one
//ShowString(s, "s");

	CString in = s;
	CString out("");

	CString fl( "p[" );
	CString fr( "]"  );

//	int Find( LPCTSTR lpszSub ) const;
  //Return Value
//The zero-based index of the first character in this CString object
//that matches the requested substring or characters;
//-1 if the substring or character is not found.

	while(true){
		int length = s.GetLength();
		if (length<4) break;
		int res = s.Find(fl); // find w[
		if (res<0){
			break;
		}

		res += 2;

		out += s.Left(res);
//		ShowString(out, "out");
		s = s.Right(length-res);
		length = s.GetLength();

//		ShowString(s, "s");

		res = s.Find(fr);	// find ]
		if (res<=0){		// if not found or if the situation was w[]
			break;
		}
		CString number = s.Left(res);
		double d = strtod(CStringA(number), NULL);
		int index = int(d);
//		ShowString(number, "number");
//		ShowFloat(index, "index");
/*
		if (index==state){
			s = in; // the variable is used!!!
			return false;
		}

*/
		if (index>=state){
			index --;
			if (index<0){
				s = in;
				return false;
			}
			char ss[20];
			_itoa(index, ss, 10);
			number = CString(ss);
		}
		out += number + fr;
		s = s.Right(length-res-1);
	}
	out += s;
	s = out;
	return true;
}

bool FunctionParser::CheckDecreaseStateIndex(CString s, int state){
	// starting from state all indexes in p[] expressions are decreases by one
//ShowString(s, "s");

//	CString in = s;
//	CString out("");

	CString fl( "p[" );
	CString fr( "]"  );

//	int Find( LPCTSTR lpszSub ) const;
  //Return Value
//The zero-based index of the first character in this CString object
//that matches the requested substring or characters;
//-1 if the substring or character is not found.

	while(true){
		int length = s.GetLength();
		if (length<4) break;
		int res = s.Find(fl); // find w[
		if (res<0){
			break;
		}

		res += 2;

//		out += s.Left(res);
//		ShowString(out, "out");
		s = s.Right(length-res);
		length = s.GetLength();

//		ShowString(s, "s");

		res = s.Find(fr);	// find ]
		if (res<=0){		// if not found or if the situation was w[]
			break;
		}
		CString number = s.Left(res);
		double d = strtod(CStringA(number), NULL);
		int index = int(d);
//		ShowString(number, "number");
//		ShowFloat(index, "index");

		if (index==state){
//			s = in; // the variable is used!!!
			return false;
		}


		if (index>state){
			index --;
			if (index<0){
//				s = in;
				return false;
			}
			char ss[20];
			_itoa(index, ss, 10);
			number = CString(ss);
		}
//		out += number + fr;
		s = s.Right(length-res-1);
	}
//	out += s;
//	s = out;
	return true;
}


bool FunctionParser::IsVariable(CString c, int & index){  // returns true and puts the correct index if 
	c.Remove(' ');
	c.TrimLeft();
	c.MakeUpper();
	if (c.GetAt(0)!='W'){
		return false;
	}
	int brack=c.Find(']');
	CString num=c.Mid(2, brack-2);
//		Alert0(num);
	index = atoi(num);
	return true;
}

/*
bool FunctionParser::GetExpArg(CString c, CString & arg){
	c.MakeUpper();

	return true;
}
*/
Function * FunctionParser::GetExpArgument(Function * g){
	if (!g) {
		return NULL;
	}
	switch (g->Type) {
		case 0 : return NULL; break;
		case 1 : return NULL; break;
		case 2 : return NULL; break;
		case 3 : if (g->unaryfunction == exp){
					return g->firstarg;
				 }
				 else{
					 GetExpArgument(g->firstarg);
				 }
				 break;
		case 4 : {
					Function * f1 = GetExpArgument(g->firstarg);
					if (f1) return f1;
					return GetExpArgument(g->secondarg);
				 }
				 break;
		case 5: return NULL; break;
		case 6:		return GetExpArgument(g->firstarg);
					break;
		case 7: return NULL; break;
		case 8: return NULL; break;
		default : return NULL; //
	}

	return NULL;
}

Function * FunctionParser::ReplaceAllVariables(Function * g, Variable * Vars, Variable * UserFuncs){
	if (!g) return NULL;

	if (g->Type == 5) { // Var
		return ReplaceAllVariables(Vars[g->arrayindex].f, Vars, UserFuncs);
	}

	Function * r = new Function;
	r->Type = g->Type;
	r->arrayindex = g->arrayindex;
	r->secondarrayindex = g->secondarrayindex;
	r->value = g->value;

	switch (g->Type) {
		case 0 : return r; break;
		case 1 : r->value = g->value; return r;		break;
		case 2 : return r; break;
		case 3 : r->unaryfunction = g->unaryfunction;
				 r->firstarg = ReplaceAllVariables(g->firstarg, Vars, UserFuncs);
				 return r;
				 break;
		case 4 : r->binaryfunction = g->binaryfunction;
				 r->firstarg = ReplaceAllVariables(g->firstarg, Vars, UserFuncs);
				 r->secondarg = ReplaceAllVariables(g->secondarg, Vars, UserFuncs);
				 return r;
				 break;
		case 5:	 return NULL; break; // MUST NOT HAPPEN, ALREADY CHECKED
		case 6: {
					r->firstarg = ReplaceAllVariables(g->firstarg, Vars, UserFuncs);
					return r;
				}
				break;
		case 7: return r; break;
		case 8: return r; break;
	default : return NULL; //
	}

}

void FunctionParser::ReplaceAllVariables(CString InputString, CString & OutputString, Variable * Vars, Variable * UserFuncs){
	InputString.Remove(' ');
	int CommentPos = InputString.Find("'");
	if (CommentPos>=0){
		InputString = InputString.Left(CommentPos);
	}
	int l = InputString.GetLength();
	int LastPos = l;
	if (LastPos<1){
		OutputString = InputString;
		return;
	}
	int VariablePos1 = InputString.Find("w[");
	int VariablePos2 = InputString.Find("W[");
	if ((VariablePos1<0) && (VariablePos2<0)){
		OutputString = InputString;
		return;
	}
	int VariablePos = VariablePos1;
	if (VariablePos1<0){
		VariablePos = VariablePos2;
	}
	else{
		if (VariablePos2>=0){
			if (VariablePos2<VariablePos1){
				VariablePos = VariablePos2;
			}
		}
	}

	int EndVar = InputString.Find("]", VariablePos);
	if ((EndVar<0) || (EndVar>LastPos)){
		OutputString = InputString;
		return;
	}

	CString num = InputString.Mid(VariablePos+2, EndVar-VariablePos-2);
	//Alert0(num);

	int index = atoi(num);

	CString VarString;
	ReplaceAllVariables(Vars[index].FunctionString, VarString, Vars, UserFuncs);
//	Alert0(VarString);
//	Alert0(InputString.Mid(EndVar+1, l-EndVar-1));

	OutputString = InputString.Left(VariablePos)
					+CString("(")
					+VarString
					+CString(")")
					+InputString.Mid(EndVar+1, l-EndVar-1);

//	Alert0(OutputString);

	CString NewOut;
	ReplaceAllVariables(OutputString, NewOut, Vars, UserFuncs);
	OutputString = NewOut;

}

bool FunctionParser::GetGatingValence(CString InputString, CString & OutputString){
//	Alert0("HI FunctionParser::GetGatingValence");

	InputString.MakeUpper();
	InputString.Remove(' ');
	int l=InputString.GetLength();

	if (l<4) return false;

	int ExpPos = InputString.Find("EXP(");
//	ShowFloat(ExpPos, "ExpPos");

	if (ExpPos<0) return false;

	int BrackCount=1;

	int pos = ExpPos+4;

	int BrackPos = -1;

	while (true){
		if (pos>=l){
			return false;
		}
		char s = InputString.GetAt(pos);
		if (s=='('){
			BrackCount++;
		}
		if (s==')'){
			BrackCount--;
		}
		if (BrackCount==0){
			BrackPos = pos;
			break;
		}
		pos++;
	}
//	ShowFloat(BrackPos, "brackpos");

//	int BrackPos = InputString.Find(")", ExpPos+1);

	if (BrackPos<0) return false;

	CString ExpString = InputString.Mid(ExpPos+4, BrackPos-ExpPos-4);
	ExpString.TrimLeft();
	ExpString.TrimRight();

//	Alert0(ExpString);

	int VoltPos = ExpString.Find("V");
//	ShowFloat(VoltPos, "voltpos");

//	Alert0(InputString.Right(l-BrackPos-1));
	if (VoltPos<0){
//		Alert0("VoltPos<0");
		return GetGatingValence(InputString.Right(l-BrackPos-1), OutputString);
	}

	int Str25 = ExpString.Find("/25");
//	ShowFloat(Str25, "Str25");
	if (Str25<0){
		return GetGatingValence(InputString.Right(l-BrackPos-1), OutputString);
	}
	ExpString = ExpString.Left(Str25);
//	Alert0(ExpString);

	int lexp = ExpString.GetLength();

	if (lexp==1){
		OutputString="1";
		return true;
	}
	if (lexp==2){
		if (VoltPos==1){
			char s = ExpString.GetAt(0);
			if (s=='+'){
				OutputString="1";
				return true;
			}
			if (s=='-'){
				OutputString="-1";
				return true;
			}
			return GetGatingValence(InputString.Right(l-BrackPos-1), OutputString);
		}
		char s = ExpString.GetAt(0);
		if (s==' '){
			OutputString="1";
			return true;
		}
		return GetGatingValence(InputString.Right(l-BrackPos-1), OutputString);

	}

	int TimesV=ExpString.Find("*V");
	if (TimesV>=0){
		OutputString = ExpString.Left(TimesV)+ExpString.Right(lexp-2-TimesV);
//		Alert0(OutputString);
		if (OutputString.GetLength()==0){
			OutputString="1";
		}
		return true;
	}

	int VTimes=ExpString.Find("V*");
	if (VTimes>=0){
		OutputString = ExpString.Left(VTimes)+ExpString.Right(lexp-2-VTimes);
		if (OutputString.GetLength()==0){
			OutputString="1";
		}
		return true;
	}

	return GetGatingValence(InputString.Right(l-BrackPos-1), OutputString);
}

void FunctionParser::ReconstructFunctionString(Function * g, CString & c){
	c="";
}


/*
void Function::operator = (const Function & f ){
	Type = f.Type;
	value = f.value;
	arrayindex=f.arrayindex;
	secondarrayindex = f.secondarrayindex;

	unaryfunction = f.unaryfunction;
	binaryfunction = f.binaryfunction;

	if (f.firstarg){
		Function * pf = new Function;
		Function & func = *pf;
		func = *(f.firstarg);
		firstarg = pf;
	}
	else{
		firstarg = NULL;
	}
	if (f.secondarg){
		Function * pf = new Function;
		Function & func = *pf;
		func = *(f.secondarg);
		secondarg = pf;
	}
	else{
		secondarg = NULL;
	}

}
*/

bool FunctionParser::FunctionUsesXorVorTVariable(Function * f){
	if (!f){
		return false;
	}
	bool r1;
	switch (f->Type){
		case 0 : return false; break;
		case 1 : return true; break;
		case 2 : return false; break;
		case 3 : return FunctionUsesXorVorTVariable(f->firstarg);
				 break;
		case 4 : r1= FunctionUsesXorVorTVariable(f->firstarg);
				if (r1){
					return true;
				}
				return FunctionUsesXorVorTVariable(f->secondarg);
				break;
		case 5: return false; break;
		case 6: return FunctionUsesXorVorTVariable(f->firstarg);
				break;
		case 7: return false; break;
		case 8: return false; break;
		default : return false; //
	}
}
