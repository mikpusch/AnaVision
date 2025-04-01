// FunctionParser.cpp : implementation file
//

#include "stdafx.h"
#include "../StateEditor/StateEditor.h"
#include <Math.h>
//#include "FunctionParser.h"

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
		delete f;
	}
	f=NULL;
}

BOOL Variable::SaveOnFile(CFile & file){
	SaveString(FunctionString, file);
	return TRUE;
}

BOOL Variable::LoadFromFile(CFile & file){
	if (!LoadString(FunctionString, file)){
		return FALSE;
	}
	FunctionParser fp;
	f=fp.MakeFunction(FunctionString);
	return (f != NULL);
}


int Variable::CheckVariable(BOOL * valid, int MaxAllowed){	// 0: invalid use of variables
										// 1: not defined (NIL)
										// 2: defined and ok
	if (f==NULL) return 1;
	return f->CheckVariable(valid, MaxAllowed);
}



double step(double arg1){
	if (arg1<0.0) return 0.0;
	return 1.0;
}

double plus(double arg1, double arg2){
	return arg1 + arg2;
}
double minus(double arg1, double arg2){
	return arg1 - arg2;
}
double times(double arg1, double arg2){
	return arg1*arg2;
}
double divide(double arg1, double arg2){
	return arg1 / arg2;
}

double inv(double arg1){
	return 1.0/arg1;
}

double negative(double arg1){
	return -arg1;
}


BOOL IsNumber(CString c, int first){
	if (c==CString("0")) return TRUE;
	if (c==CString("1")) return TRUE;
	if (c==CString("2")) return TRUE;
	if (c==CString("3")) return TRUE;
	if (c==CString("4")) return TRUE;
	if (c==CString("5")) return TRUE;
	if (c==CString("6")) return TRUE;
	if (c==CString("7")) return TRUE;
	if (c==CString("8")) return TRUE;
	if (c==CString("9")) return TRUE;
	if (c==CString(".")) return TRUE;
	if ((c==CString("-")) & first) return TRUE;
	return FALSE;
}

double GetNumber(CString InString, int & length){
	CString c=InString.Left(1);
	int l=InString.GetLength();
	int i=1;
	int expfound = 0;
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
	return atof(c);
}



/////////////////////////////////////////////////////////////////////////////
// FunctionParser

IMPLEMENT_DYNCREATE(FunctionParser, CCmdTarget)

FunctionParser::FunctionParser(){
	LastError=CString("");
	Funcs=NULL;
}

FunctionParser::~FunctionParser()
{
	if (Funcs){
		//DeleteFuncs();
		delete [] Funcs;
		Funcs=NULL;
	}
}

BOOL FunctionParser::CheckUserFunc(CString c, int & UserIndex){
	// Minimum is: FUNC[0] i.e. 7 characters
	int l=c.GetLength();
	if (l<7) return FALSE;
	CString test1 = c.Left(5);
	if (test1!=CString("FUNC[")) return FALSE;
	int findresult = c.Find(']');
	if (findresult==(-1)){
		return FALSE;
	}

//	ShowFloat(findresult, "find ]");

	CString s2 = c.Mid(5,findresult-5);
//	Alert0(s2);
	UserIndex = atoi(s2);
//	ShowFloat(UserIndex, "User index");
	return TRUE;
}

int FunctionParser::GetUnaryFunc(CString & c,	
				  unaryfuncptr & lunaryfunction){

//	Alert(CString("enter getunaryfunc :") + c);

	if (c == CString("STEP")){
		lunaryfunction = step;
		return 1;
	}
	if (c == CString("EXP")){
		lunaryfunction = exp;
		return 1;
	}
	if (c == CString("INV")){
		lunaryfunction = inv;
		return 1;
	}
	if (c == CString("SQRT")){
		lunaryfunction = sqrt;
		return 1;
	}
	if (c == CString("LOG")){
		lunaryfunction = log;
		return 1;
	}
	if (c == CString("FABS")){
		lunaryfunction = fabs;
		return 1;
	}
	return 0;
}


Function * FunctionParser::NewFunc(){
	Function * result = new Function;
	Funcs[NFunc]=result;
	NFunc++;
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
		RemoveFunc(Funcs[i]);
//		Funcs[i]->firstarg=NULL;
//		Funcs[i]->secondarg=NULL;
		//Funcs[i]->Remove();
//		delete Funcs[i];
	}
}


BEGIN_MESSAGE_MAP(FunctionParser, CCmdTarget)
	//{{AFX_MSG_MAP(FunctionParser)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FunctionParser message handlers

double Function::eval(double v, double * array, double * p, double * variables,
//					  Function * UserFuncs){
					  Variable * UserFuncs, double ** rates){

  switch (Type) {
	case 0 : return value; break;
	case 1 : return v;		break;
	case 2 : return array[arrayindex]; break;
	case 3 : return (*unaryfunction)(firstarg->eval(v, array, p, variables, UserFuncs, rates));
			  break;
	case 4 : return (*binaryfunction)
				     ( firstarg->eval (v, array, p, variables, UserFuncs, rates),
					   secondarg->eval(v, array, p, variables, UserFuncs, rates));
			break;
	case 5: return variables[arrayindex]; break;
	case 6: {
				double arg = firstarg->eval(v, array, p, variables, UserFuncs, rates);
//				return UserFuncs[arrayindex].eval(arg, array, variables, UserFuncs);
				return UserFuncs[arrayindex].f->eval(arg, array, p, variables, UserFuncs, rates);
			}
		break;
	case 7: return p[arrayindex]; break;
	case 8: return rates[arrayindex][secondarrayindex]; break;
	default : return 0; //
	}
}


double Function::evalwithcheck(double v, double * array, double * p, double * variables,
//					  Function * UserFuncs){
					  Variable * UserFuncs, double ** rates){

  switch (Type) {
	case 0 : return value; break;
	case 1 : return v;		break;
	case 2 : {
				if (array){
					return array[arrayindex];
				}
				else{
					return 0;
				}
				break;
			}
	case 3 : return (*unaryfunction)(firstarg->evalwithcheck(v, array, p, variables, UserFuncs, rates));
			  break;
	case 4 : return (*binaryfunction)
				     ( firstarg->evalwithcheck (v, array, p, variables, UserFuncs, rates),
					   secondarg->evalwithcheck (v, array, p, variables, UserFuncs, rates));
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
					double arg = firstarg->evalwithcheck(v, array, p, variables, UserFuncs, rates);
					return UserFuncs[arrayindex].f->evalwithcheck(arg, array, p, variables, UserFuncs, rates);
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

int Function::CheckVariable(BOOL * valid, int MAXVARALLOWED){
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
		if (secondarg){
			secondarg->Remove();
			secondarg = NULL;
		}
	}
	delete this;

}

Function::Function()
{
//	unaryfunction = exp;
//	binaryfunction = plus;
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
}

Function * FunctionParser::MakeFunction(CString instring){
	CString s=instring;
	s.MakeUpper();
	CString out=CString("");
	int l=s.GetLength();
	int i;
	for (i=0; i<l; i++){
		CString c=s.Mid(i,1);
		if (!c.Compare("'")){
			break;
		}
		if (c.Compare(" ")){
			out += c;
		}
	}
	ParseError=0;
	NFunc=0;

//	Funcs = new Function *[s.GetLength()*10];

	BOOL ErrorOccured=FALSE;
	Function * result = ParseAux(out, ErrorOccured);
	if (ErrorOccured | (!result)){
		result = NULL;
		//DeleteFuncs();
	}
	return result;
}

int GetCorrespondingRightParentIndex(int index, int count, CString & is){
	int l=is.GetLength();
	if (index>=l) return -1;
	CString f=is.Mid(index, 1);
	if (f.Compare("(")==0){
	    return GetCorrespondingRightParentIndex(index+1, count+1, is);
	}
	if (f.Compare(")")==0){
		if (count==1){
			return index;
		}
		else{
		    return GetCorrespondingRightParentIndex(index+1, count-1, is);
		}
	}
    return GetCorrespondingRightParentIndex(index+1, count, is);
}

binaryfuncptr GetBinaryOp(CString c, int & priority, BOOL ErrorOccured){
	if (c==CString("^")){
		priority=0;
		return pow;
	}
	if (c==CString("/")){
		priority=1;
		return divide;
	}
	if (c==CString("*")){
		priority=2;
		return times;
	}
	if (c==CString("+")){
		priority=4;
		return plus;
	}
	if (c==CString("-")){
		priority=3;
		return minus;
	}
	ErrorOccured=TRUE;
	return NULL;
}

Function * FunctionParser::ParseAux(CString is, BOOL & ErrorOccured){
	//Alert(CString("Enter new parse with: ")+is);
	ErrorOccured=FALSE;
	int l= is.GetLength();
	if (l==0) return NULL;

	Function * exprs[MAXEXPRESSIONALLOWEDINPARSE];
	binaryfuncptr bf[MAXEXPRESSIONALLOWEDINPARSE];
	int priority[MAXEXPRESSIONALLOWEDINPARSE];

	int nexprs=0;
	BOOL GoOn=TRUE;
	while (GoOn){
		Function * e = GetFirstExpr(is, ErrorOccured);
		if (ErrorOccured){
			return NULL;
		}
		if (e){
			exprs[nexprs]=e;
			int newl=is.GetLength();
			if (!newl){
				GoOn=FALSE;
			}
			else{
				CString op=is.Mid(0,1); // operator string, i.e. *, ^, /, +, -
				is=is.Right(newl-1);
				//Alert(CString("op = ")+op+CString(" new is = ")+is);
				bf[nexprs] = GetBinaryOp(op, priority[nexprs], ErrorOccured);
				if (ErrorOccured){
					return NULL;
				}
			}
			nexprs++;
			if ((nexprs+1)>=MAXEXPRESSIONALLOWEDINPARSE){
				Alert0("EXPRESSION TOO COMPLICATED");
				ErrorOccured=TRUE;
				return NULL;
			}
		}
		else{ // i.e. no expression found, but no error
			GoOn=FALSE;
		}
	} // end while(GoOn)

	if (nexprs==0){
		ErrorOccured=TRUE;
		return NULL;
	}

	while (1){
		if (nexprs==1){
			return exprs[0];
		}
		int mini=0;
		int minp=priority[0];
		for (int i=1; i<(nexprs-1); i++){
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

Function * FunctionParser::GetFirstExpr(CString & is, BOOL & ErrorOccured){
	//Alert(CString("Enter GetFirstEpr with: ")+is);
	ErrorOccured=FALSE;
	int l=is.GetLength();
	if (l==0) return NULL;
	CString f=is.Mid(0,1);
	if ((f.Compare("V")==0)|(f.Compare("X")==0)|(f.Compare("T")==0)){
		//Alert("V in GetFirstExpr");
		is = is.Right(l-1);
		Function * nf = NewFunc();
		nf->Type=1;
		return nf;
	}
	if (f.Compare("(")==0){
		//Alert(" ( in GetFirstExpr");
		int rpindex=GetCorrespondingRightParentIndex(1, 1, is);
		if (rpindex<0){
			ErrorOccured=TRUE;
			return NULL;
		}
		CString fs=is.Mid(1, rpindex-1);
		if (rpindex==(l-1)){ // i.e. nothing left
			is = CString("");
		}
		else{
			is = is.Mid(rpindex+1, l-rpindex-1);
		}
		return ParseAux(fs, ErrorOccured);
	}
	if (f.Compare("-")==0){
		//Alert(" - in GetFirstExpr");
		if (l==1){
			ErrorOccured=TRUE;
			return NULL;
		}
		CString s = is.Mid(1,1);
		if (IsNumber(s, FALSE)){
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
			ErrorOccured=TRUE;
			return NULL;
		}
		Function * f = NewFunc();
		f->Type=3; // UNARY FUNC;
		f->firstarg=f2;
		f->unaryfunction=negative;
		return f;
	}
	if (IsNumber(f, FALSE)){
		//Alert(" is # in GetFirstExpr");
		int ln;
		double r=GetNumber(is, ln);
		Function * f = NewFunc();
		f->Type=0;
		f->value=r;
		is = is.Right(l-ln);
		return f;
	}
	if (f.Compare("A")==0){
		//Alert(" A in GetFirstExpr");
		if (l<2){
			ErrorOccured=TRUE;
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
		int larrayindex = atoi(s2);
		is = is.Right(l-findresult-1);
		//Alert(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=2;
		f->arrayindex=larrayindex;
		return f;
	}
	if (f.Compare("R")==0){
		//Alert(" R in GetFirstExpr"); // Min: R[0][0] = 7
		if (l<7){
			ErrorOccured=TRUE;
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
		int larrayindex = atoi(s2);
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
		int secondarrayindex = atoi(s2);
		is = is.Right(is.GetLength()-findresult-1);


//		Alert0(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=8;
		f->arrayindex=larrayindex;
		f->secondarrayindex=secondarrayindex;
		return f;
	}
	if (f.Compare("P")==0){
		//Alert(" P in GetFirstExpr");
		if (l<2){
			ErrorOccured=TRUE;
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
		int larrayindex = atoi(s2);
		is = is.Right(l-findresult-1);
		//Alert(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=7;
		f->arrayindex=larrayindex;
		return f;
	}
	if (f.Compare("W")==0){
		//Alert(" W in GetFirstExpr");
		if (l<2){
			ErrorOccured=TRUE;
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
		int larrayindex = atoi(s2);
		is = is.Right(l-findresult-1);
		//Alert(CString("new is: ")+is);
		Function * f = NewFunc();
		f->Type=5;
		f->arrayindex=larrayindex;
		return f;
	}

	// Now can only be unary func
//	Alert(" Can only be unary in GetFirstExpr");
	int lp=is.Find("(");
	if (lp==(-1)){
		ErrorOccured=1;
		return FALSE;
	}
	int rp=GetCorrespondingRightParentIndex(lp+1, 1, is);

	if ((rp<0)|(rp==(lp+1))){
		ErrorOccured=TRUE;
		return NULL;
	}
//	ShowInt(lp, "lp");
//	ShowInt(rp, "rp");

	CString fs=is.Mid(lp+1, rp-lp-1);
	unaryfuncptr lunaryfunction;
	CString UF=is.Left(lp);
	//Check first if user func:
	int UserIndex=0;
	BOOL IsUserFunc = CheckUserFunc(UF, UserIndex);

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
	Function * arg = ParseAux(fs, ErrorOccured);
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



BOOL FunctionParser::DecreaseVarIndex(CString & s, int var){
// starting from var all indexes are decrease by one
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

	while(TRUE){
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
		double d = strtod(number, NULL);
		int index = int(d);
//		ShowString(number, "number");
//		ShowFloat(index, "index");
		if (index==var){
			s = in; // the variable is used!!!
			return FALSE;
		}

		if (index>var){
			index --;
			if (index<0){
				s = in;
				return FALSE;
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
	return TRUE;
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

	while(TRUE){
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
		double d = strtod(number, NULL);
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
	if (f==step) c="step";
	if (f==exp)  c="exp";
	if (f==inv)  c="inv";
	if (f==sqrt) c="sqrt";
	if (f==log)  c="log";
	if (f==fabs) c="fabs";
}

void Function::GetBinaryFuncName(binaryfuncptr f, CString & c){
	if (f==pow)    c="pow";
	if (f==divide) c="/";
	if (f==times)  c="*";
	if (f==plus)   c="+";
	if (f==minus)  c="-";
}

