
#pragma once;

#include <vector>

#include "../common/ListObject.h"
#include "DrawObject.h"

class ConnectedComponent;

class ComponentPoint  : public ListObject {
public:			//all public
	//default creator and destructor

	ComponentPoint(int w, int h);
	~ComponentPoint();

	void Init(int w, int h);
	
	ConnectedComponent * parent;
	int height;
	int width;
	CPoint point;

	int value;

	ComponentPoint& operator=(ComponentPoint& src);

	void CopyValuesFrom(ComponentPoint  * ps);
	void Init();
};

// describes the detection of a connected area in an image.
// Starts of by a single point and expands

class ConnectedComponent{
public:

	ConnectedComponent();
	~ConnectedComponent();

	void Init(int w, int h);

	int height;
	int width;

	ComponentPoint * Points; // nested list;

	vector<bool> Selected;
	vector<bool> Tested;

	bool Scan(CPoint StartPoint, double Threshold, vector<bool> & AlreaySelected,
		vector<int> & imagedata, CDC * pC, bool UsePoly = false, PolygonObject * poly = NULL);

	bool ScanArea(CPoint StartPoint, double Threshold, int & NumPointsSelected,
					double & mean, bool UseInitialMeanForFill, vector<bool> & AlreaySelected,
					vector<int> & imagedata, CDC * pC, bool UsePoly = false, PolygonObject * poly = NULL);

	void CopyToSelected(vector<bool> & Selected);
	void CopyToSelected(ComponentPoint * points, vector<bool> & Selected);
	void RemoveFromSelection(ComponentPoint * points, vector<bool> & Selected);
	void RemoveFromSelection(vector<bool> & Selected);

	bool CheckPointPresentInList(CPoint point);
	bool PointIsInRectangle(CPoint point);
	bool PointIsElegible(CPoint p, vector<bool> & AlreaySelected);

	double CalcMeanValue();

	double CalcMeanOverAllSelected(int & NPointsForMean, vector<int> & imagedata);
	double CalcMeanOverCurrentList(int & NPointsForMean);


};

