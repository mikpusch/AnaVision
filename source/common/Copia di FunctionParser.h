// FunctionParser.h : header file
//

#define MAXEXPRESSIONALLOWEDINPARSE 200


typedef double (*unaryfuncptr)(double);

typedef double (*binaryfuncptr)(double, double);


class Variable;

class Function {

public:

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

	double eval(double v, double * array, double * p, double * variables,
				Variable * UserFuncs, double ** rates);

	//this function returns 0 if a requested array == NULL
	double evalwithcheck(double v, double * array, double * p, double * variables,
				Variable * UserFuncs, double ** rates);

	int MaxIndexUsed();
	int HelpMaxIndexUsed(int result);
	int MaxVariableUsed();
	int HelpMaxVariableUsed(int result);

	int CheckVariable(BOOL * valid, int MAXVARALLOWED);

	void ShowFunction();
	void ShowFunction(int level);
	void GetUnaryFuncName(unaryfuncptr f, CString & c);
	void GetBinaryFuncName(binaryfuncptr f, CString & c);

	

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

	BOOL SaveOnFile(CFile & f);
	BOOL LoadFromFile(CFile & f);

	int CheckVariable(BOOL * valid, int MaxAllowed);	// 0: invalid use of variables
										// 1: not defined (NIL)
										// 2: defined and ok
private :
	int DoCheckVariable(Function * g, BOOL * valid);
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

	Function * MakeFunction(CString instring);

	BOOL DecreaseVarIndex(CString & s, int var); // starting from var all indexes are decrease by one
	void IncreaseVarIndex(CString & s, int var); // starting from var all indexes are decrease by one

	void RemoveFunc(Function * f);
private:

	Function * ParseAux(CString is, BOOL & ErrorOccured);
	Function * GetFirstExpr(CString & is, BOOL & ErrorOccured);
	BOOL CheckUserFunc(CString c, int & UserIndex);

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
