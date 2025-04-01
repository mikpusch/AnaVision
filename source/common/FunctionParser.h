// FunctionParser.h : header file
//

#pragma once
//#ifndef FUNCTION_PARSER_MIK
//#define FUNCTION_PARSER_MIK

#define MAXEXPRESSIONALLOWEDINPARSE 200


typedef double (*unaryfuncptr)(double);

typedef double (*binaryfuncptr)(double, double);


class Variable;

class Function {

public:
//	void operator = (const Function & f );

	int Type;	// 0 = Constant
				// 1 = "voltage"
				// 2 = array of parameters
				// 3 = unary function
				// 4 = binary function
				// 5 = Variable
				// 6 = UserFunc (uses index and supplied array of functions
				// 7 = array of probability
				// 8 = matrix of rates


	double value;
	int arrayindex;
	int secondarrayindex;

	unaryfuncptr unaryfunction;
	binaryfuncptr binaryfunction;

	Function * firstarg;
	Function * secondarg;

	double eval(double v, double * darray, double * p, double * variables,
				Variable * UserFuncs, double ** rates);

	//this function returns 0 if a requested array == NULL
	double evalwithcheck(double v, double * darray, double * p, double * variables,
				Variable * UserFuncs, double ** rates);


	bool NeedParenthesisForMultiply();

	int MaxIndexUsed();
	int HelpMaxIndexUsed(int result);
	int MaxVariableUsed();
	int HelpMaxVariableUsed(int result);

	int CheckVariable(bool * valid, int MAXVARALLOWED);

	void ShowFunction();
	void ShowFunction(int level);
	void GetUnaryFuncName(unaryfuncptr f, CString & c);
	void GetBinaryFuncName(binaryfuncptr f, CString & c);

	void Copy(Function & f);
//	void Copy(Function * & f);
	Function * Copy();

	static void Clear(Function * f);

	void operator=(Function &f );
//	void operator=(Function * & f );

	Function();
	~Function();

	void Remove();
};

class Variable
{
public:
	Variable();
	~Variable();
	CString FunctionString;
	Function * f;

//    void operator=(const Variable & v );

	void Copy(Variable & g);

	bool SaveOnFile(CFile & f);
	bool LoadFromFile(CFile & f);

	int CheckVariable(bool * valid, int MaxAllowed);	// 0: invalid use of variables
										// 1: not defined (NIL)
										// 2: defined and ok
private :
	int DoCheckVariable(Function * g, bool * valid);
};


/////////////////////////////////////////////////////////////////////////////
// FunctionParser command target

#define LEFTPARENT		1
#define RIGHTPARENT		2
#define VOLTAGE			3
#define NUMBER			4
#define ARRAY			5
#define UNARYFUNC		6
#define BINARYFUNC		7


class FunctionParser : public CCmdTarget
{
	DECLARE_DYNCREATE(FunctionParser)

public:
	FunctionParser();           // protected constructor used by dynamic creation

// Attributes
public:
	CString LastError;
private:
	int ParseError;
	int NFunc;
//	Function ** Funcs;

// Operations
public:

	Function * MakeFunction(CString instring, bool * UsesVoltageVariable = NULL);

	bool DecreaseVarIndex(CString & s, int var); // starting from var all indexes are decreased by one
	void IncreaseVarIndex(CString & s, int var); // starting from var all i	ndexes are decreased by one
	bool DecreaseStateIndex(CString & s, int state); // starting from state all indexes in p[] expressions are decreases by one
	bool CheckDecreaseStateIndex(CString s, int state); // starting from state all indexes in p[] expressions are decreases by one
	bool ChangePIndex(CString & s, int OldIndex, int NewIndex);
	void RemoveFunc(Function * f);

//  Some Utility functions

	static bool IsVariable(CString c, int & index);  // returns true and puts the correct index if 
												// the string is of the form "w[..]"

	static bool FunctionUsesXorVorTVariable(Function * f);

//	static bool GetExpArg(CString c, CString & arg);

	static Function * GetExpArgument(Function * g);

	static Function * ReplaceAllVariables(Function * g, Variable * Vars, Variable * UserFuncs);

	static void ReplaceAllVariables(CString InputString, CString & OutputString, Variable * Vars, Variable * UserFuncs);

	static bool GetGatingValence(CString InputString, CString & OutputString);

	static void ReconstructFunctionString(Function * g, CString & c);

private:

	Function * ParseAux(CString is, bool & ErrorOccured, bool * UsesVoltageVariable);
	Function * ParseAuxNewBad(CString is, bool & ErrorOccured);
	Function * ParseAuxOld(CString is, bool & ErrorOccured);

	Function * GetFirstExpr(CString & is, bool & ErrorOccured, bool * UsesVoltageVariable = NULL);
	bool CheckUserFunc(CString c, int & UserIndex);

	Function * Parse(CString & instring);

	Function * NewFunc();

	void DeleteFuncs();

	int GetUnaryFunc(CString & c,	
				  unaryfuncptr & lunaryfunction);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FunctionParser)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FunctionParser();

	// Generated message map functions
	//{{AFX_MSG(FunctionParser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//#endif