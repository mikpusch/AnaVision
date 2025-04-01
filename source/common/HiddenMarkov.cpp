#include "stdafx.h"
//#include <math.h>
//#include <windows.h>
//#include <stdio.h>

//#include "..\common\utils.h"
//#include "..\common\nrutil.h"
//#include "..\common\simplex.h"
//#include "../common/FunctionParser.h"
//#include "../common/SSMarkov.h"
//#include "../common/SparseMarkov.h"
//#include "../common/Powell.h"

#include "../common/HiddenMarkov.h"
//#include "../common/DefMarkov.h"
//#include "../StateEditor/Model.h"




StateVector &StateVector::operator=( StateVector &ptRHS ){
	v.UpdateBuffer(ptRHS.v.GetSize(), true);
	return *this;
}

int MyPowHM(int A, int B){
	int result = 1;
	for (int i=0; i<B; i++){
		result *= A;
	}
	return result;
}


