#include "stdafx.h"
#include "MyDrawClass.h"
#include "utils.h"


MyDrawClass::MyDrawClass(){
	pDC = NULL;
	rect.left = rect.top = 0;
	rect.right = rect.bottom = 100;;

	MinX=0;
	MaxX=1;
	MinY = 0;
	MaxY = 1;

	AutoScale=true;
	ClearWindow=true;

	offsetx = 0;
	offsety = 0;
	ScaleX = ScaleY = 1;


}

MyDrawClass::~MyDrawClass(){
}

int MyDrawClass::DoScalex(double x){
	return offsetx + int (x*ScaleX);
}
int MyDrawClass::DoScaley(double y){
	return rect.bottom -( offsety + int ((y-MinY)*ScaleY));
}

void MyDrawClass::ShowValue(double value, int x, int y, CDC * PDC){
	if (!PDC){
		PDC = pDC;
	}
	if (!PDC){
		return;
	}
	if (x<0) x=100;
	if (y<0) y=100;

	char s[20];

	_gcvt(value, 8, s);

	PDC->TextOut(x, y, CString(s));

}

void MyDrawClass::DrawLine(int NPoints, double * y, CDC * PDC){

	if (NPoints<2) return;

	if (!PDC){
		PDC = pDC;
	}
	if (!PDC){
		return;
	}
	if (AutoScale){
		MinY = MinDouble(y, NPoints);
		MaxY = MaxDouble(y, NPoints);
	}

	if (ClearWindow){
		PDC->FillRect(&rect, PDC->GetCurrentBrush());
	}

	ScaleX = double(rect.right-rect.left)/double(NPoints+1);
	offsetx = rect.left;
	ScaleY = double(rect.bottom-rect.top)/(MaxY-MinY);

//	ShowFloat(rect.left, "left");
	for (int i=0; i<NPoints; i++){

		int x = DoScalex(i);
		int yy = DoScaley(y[i]);
		if (i==0){
			PDC->MoveTo(x,yy);
		}
		else{
			PDC->LineTo(x,yy);
		}
	}



}

