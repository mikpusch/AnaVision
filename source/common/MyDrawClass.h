#pragma once

//#ifndef MIK_DRAW_CLASS
//#define MIK_DRAW_CLASS

class MyDrawClass{
public:
	MyDrawClass();
	~MyDrawClass();

	CDC * pDC;
	CRect rect;

	double MinX, MaxX, MinY, MaxY;

	int offsetx, offsety;
	double ScaleX, ScaleY;

	BOOL AutoScale;
	BOOL ClearWindow;

	void DrawLine(int NPoints, double * y, CDC * pDC = NULL);
	void ShowValue(double value, int x=-1, int y=-1, CDC * pDC = NULL);


	int DoScalex(double x);
	int DoScaley(double y);

};

//#endif
