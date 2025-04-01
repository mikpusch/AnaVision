

#include "stdafx.h"
#include "ConnectedComponent.h"
#include "utils.h"
#include <math.h>
#include "DrawObject.h"

ComponentPoint::ComponentPoint(int w, int h) {
	width = w;
	height = h;
	parent = NULL;
	Previous = Next = NULL;
	Init();
}

ComponentPoint::~ComponentPoint() {
}

void ComponentPoint::Init(){
}

ComponentPoint& ComponentPoint::operator=(ComponentPoint& src){
//	Alert0("hi");
	CopyValuesFrom(&(src));
	return (*this);
}

void ComponentPoint::CopyValuesFrom(ComponentPoint * ps){

	width = ps->width;
	height = ps->height;
	parent = ps->parent;
	point = ps->point;
	value = ps->value;

}


ConnectedComponent::ConnectedComponent() {

	Points = NULL;
//	Alert0("finished      ");

}
void ConnectedComponent::Init(int w, int h) {
	width = w;
	height = h;
	if (Points) delete Points;
	Points = NULL;
}
ConnectedComponent::~ConnectedComponent() {
	if (Points){
		delete Points;
	}
}
bool ConnectedComponent::CheckPointPresentInList(CPoint point){
	ComponentPoint * points = this->Points;
	while (points){
		if ((points->point.x == point.x ) && (points->point.y == point.y)){
			return true;
		}
		points = (ComponentPoint * ) points->Next;
	}

	return false;
}
bool ConnectedComponent::PointIsInRectangle(CPoint TestPoint){
	if (TestPoint.x<0){
		return false;
	}
	if (TestPoint.y<0){
		return false;
	}
	if (TestPoint.x>=width){
		return false;
	}
	if (TestPoint.y>=height){
		return false;
	}
	return true;
}


bool ConnectedComponent::PointIsElegible(CPoint TestPoint, vector<bool> & AlreadySelected){
//	if (CheckPointPresentInList(TestPoint)){
//		return false;
//	}
	if (!PointIsInRectangle(TestPoint)){
		return false;
	}
	Tested[TestPoint.x+TestPoint.y*width] = true;
	if (AlreadySelected[TestPoint.x+TestPoint.y*width]){
		return false;
	}
	return true;
}

double ConnectedComponent::CalcMeanValue(){
	double result = 0.0;
	int N = 0;
	ComponentPoint * points = Points;
	while (points){
		result += points->value;
		N++;
		points = (ComponentPoint * ) points->Next;
	}
	if (N>0){
		result /= double(N);
	}
	return result;
}




bool ConnectedComponent::Scan(CPoint StartPoint,
							  double Threshold,
							  vector<bool> & AlreadySelected,
							  vector<int> & imagedata,
							  CDC * pDC,
							  bool UsePoly,
							  PolygonObject * poly
							  ){

//	Alert0("start scan");

	Selected.resize(width*height);
	Tested.resize(width*height);
	for (int i=0; i<width*height; i++){
		Selected[i] = Tested[i] = AlreadySelected[i];
	}
	CopyToSelected(Selected);
	CopyToSelected(Tested);

	if (Selected[StartPoint.x+StartPoint.y*width]){
		Alert0("start point already selected");
//		return false;
	}

	if (CheckPointPresentInList(StartPoint)){
		Alert0("start point already in list");
//		return false;
	}
	double AbsThreshold = fabs(Threshold/100.0);
	if (!CheckPointPresentInList(StartPoint)){
		ComponentPoint * NewPoint = new ComponentPoint(width, height);
		NewPoint->parent = this;
		NewPoint->point = StartPoint;
		NewPoint->value = imagedata[StartPoint.x+StartPoint.y*width];

		Points = (ComponentPoint * ) ListObject::AddListObjectAtEnd(Points, NewPoint);
	}

	int NPoints = Points->LengthList(Points);
	int delta=0;
//	ShowFloat(NPoints, "NPoints");
	if (NPoints<2){
		// Expand to neighbors
//		Alert0("expand");
		CPoint StartPoint = Points->point;
		for (int dx=-1; dx<2; dx++){
			for (int dy=-1; dy<2; dy++){
				CPoint TestPoint(StartPoint.x+dx, StartPoint.y+dy);
				if (PointIsInRectangle(TestPoint)){
					if (!Tested[TestPoint.x+TestPoint.y*width]){
						Tested[TestPoint.x+TestPoint.y*width] = true;
						if (!UsePoly || (poly&& (poly->IsInside(TestPoint)))){
								if (!Selected[TestPoint.x+TestPoint.y*width]){
													//					if (PointIsElegible(TestPoint, Selected)){
									ComponentPoint * NewPoint = new ComponentPoint(width, height);
									NewPoint->parent = this;
									NewPoint->point = TestPoint;
									NewPoint->value = imagedata[TestPoint.x+TestPoint.y*width];
									CopyToSelected(NewPoint, Selected);
									Points = (ComponentPoint * ) ListObject::AddListObjectAtEnd(Points, NewPoint);
							}
						} // if !Sel
					} // !Tested
				} // In Rect
			} //fpr dy
		} //for dx
	} // if NPoints<2
//	Alert0("end expand");
//	ShowFloat(Points->LengthList(Points), "NPoints");

	bool FoundNewPoint = false;
	bool Restart = true;
	ComponentPoint * points = Points;
	double mean = this->CalcMeanValue();
	NPoints = Points->LengthList(Points);
//	CDC * pDC = GetDC(NULL);

	CopyToSelected(Selected);
	CopyToSelected(Tested);
	ComponentPoint * LastPoint = (ComponentPoint * ) ListObject::Last(Points);
	while (true){
		if (InKey()==STOPKEY){
			return true;
		}
		CPoint TryPoint = points->point;
		
		for (int dx=-1; dx<2; dx++){
			for (int dy=-1; dy<2; dy++){
				CPoint TestPoint(TryPoint.x+dx, TryPoint.y+dy);
				if (PointIsInRectangle(TestPoint)){
					if (!Tested[TestPoint.x+TestPoint.y*width]){
						Tested[TestPoint.x+TestPoint.y*width] = true;
						if (!UsePoly || (poly&& (poly->IsInside(TestPoint)))){
								if (!Selected[TestPoint.x+TestPoint.y*width]){
												//					if (PointIsElegible(TestPoint, Selected)){
									double value = imagedata[TestPoint.x+TestPoint.y*width];
									if ( fabs((value-mean)/mean) <AbsThreshold){
										ComponentPoint * NewPoint = new ComponentPoint(width, height);
										NewPoint->parent = this;
										NewPoint->point = TestPoint;
										NewPoint->value = value;
										CopyToSelected(NewPoint, Selected);
//								Points = (ComponentPoint * )
										ListObject::AddListObjectAtEnd(LastPoint, NewPoint);
										LastPoint = (ComponentPoint * ) ListObject::Last(LastPoint);
										FoundNewPoint = true;
										Restart = false;
										mean = (double(NPoints)*mean+value)/double(NPoints+1);
										NPoints++;
										delta++;
										if (false){
								//if (delta>=1000){
											delta  = 0;
	//							Alert0("found");
											char s[20];
//									_itoa(Points->LengthList(Points), s, 10);
											_itoa(NPoints, s, 10);
											pDC->TextOut(100,100, "                                ");
											pDC->TextOut(100,100, CString(s));
										} // if delta
								}
							} // if fabs
						} // if ! selected
					}  // if ! tested
				} //if in rect
			} //for dy
		}  // for dx
		points = (ComponentPoint *) points->Next;

		if (!points){
			Points = (ComponentPoint * ) ListObject::First(LastPoint);
			points = Points;
			Restart = true;
			return true;
		}
	} // while true
	return true;
}

void ConnectedComponent::CopyToSelected(ComponentPoint * points, vector<bool> & Selected){
	while (points){
		CPoint p = points->point;
		Selected[p.x+p.y*width] = true;
		points = (ComponentPoint *) points->Next;
	}
}


void  ConnectedComponent::CopyToSelected(vector<bool> & Selected){
	CopyToSelected(this->Points, Selected);
}

void ConnectedComponent::RemoveFromSelection(ComponentPoint * points, vector<bool> & Selected){
	while (points){
		CPoint p = points->point;
		Selected[p.x+p.y*width] = false;
		points = (ComponentPoint *) points->Next;
	}
}
void ConnectedComponent::RemoveFromSelection(vector<bool> & Selected){
	RemoveFromSelection(this->Points, Selected);
}


double ConnectedComponent::CalcMeanOverAllSelected(int & N, vector<int> & imagedata){
	double result = 0.0;
	N=0;
	for (int i=0; i<height*width; i++){
		if (Selected[i]){
			result += imagedata[i];
			N++;
		}
	}
	if (N>0){
		result /= double(N);
	}
	return result;

}

double ConnectedComponent::CalcMeanOverCurrentList(int & N){
	double result = 0.0;
	N=0;
	ComponentPoint * points = Points;
	while (points){
		result += points->value;
		N++;
		points = (ComponentPoint * ) points->Next;
	}
	if (N>0){
		result /= double(N);
	}
	return result;
}

bool ConnectedComponent::ScanArea(CPoint StartPoint, double Threshold, int & NumPointsSelected,
					double & mean, bool UseInitialMeanForFill, vector<bool> & AlreadySelected,
					vector<int> & imagedata, CDC * pDC, bool UsePoly, PolygonObject * poly){
	Selected.resize(width*height);
	Tested.resize(width*height);
	for (int i=0; i<width*height; i++){
		Selected[i] = Tested[i] = AlreadySelected[i];
	}
	CopyToSelected(Selected);
	CopyToSelected(Tested);

	if (Selected[StartPoint.x+StartPoint.y*width]){
		Alert0("start point already selected");
		return false;
	}

	if (CheckPointPresentInList(StartPoint)){
		Alert0("start point already in list");
		return false;
	}
	double AbsThreshold = fabs(Threshold/100.0);
	if (!CheckPointPresentInList(StartPoint)){
		ComponentPoint * NewPoint = new ComponentPoint(width, height);
		NewPoint->parent = this;
		NewPoint->point = StartPoint;
		NewPoint->value = imagedata[StartPoint.x+StartPoint.y*width];

		Points = (ComponentPoint * ) ListObject::AddListObjectAtEnd(Points, NewPoint);
	}
	CopyToSelected(Selected);
	CopyToSelected(Tested);
//	ShowFloat(Points->LengthList(Points), "Points->LengthList(Points);");
	if ((!UseInitialMeanForFill) || (NumPointsSelected<2)){
		CPoint StartPoint = Points->point;
		for (int dx=-1; dx<2; dx++){
			for (int dy=-1; dy<2; dy++){
				CPoint TestPoint(StartPoint.x+dx, StartPoint.y+dy);
				if (PointIsInRectangle(TestPoint)){
					if (!Tested[TestPoint.x+TestPoint.y*width]){
						Tested[TestPoint.x+TestPoint.y*width] = true;
						if (!UsePoly || (poly&& (poly->IsInside(TestPoint)))){
								if (!Selected[TestPoint.x+TestPoint.y*width]){
													//					if (PointIsElegible(TestPoint, Selected)){
									ComponentPoint * NewPoint = new ComponentPoint(width, height);
									NewPoint->parent = this;
									NewPoint->point = TestPoint;
									NewPoint->value = imagedata[TestPoint.x+TestPoint.y*width];
									CopyToSelected(NewPoint, Selected);
									Points = (ComponentPoint * ) ListObject::AddListObjectAtEnd(Points, NewPoint);
							}
						} // if !Sel
					} // !Tested
				} // In Rect
			} //fpr dy
		} //for dx
//		Alert0("end expand");
	} // if NPoints<2
//	ShowFloat(Points->LengthList(Points), "Points->LengthList(Points);");
	
	CopyToSelected(Selected);
	CopyToSelected(Tested);

	bool FoundNewPoint = false;
	ComponentPoint * points = Points;
//	if ((!UseInitialMeanForFill) || (NumPointsSelected<2)){
//		mean = this->CalcMeanValue();
//	}
	int NPointsForMean;
	if (UseInitialMeanForFill){
		mean = CalcMeanOverAllSelected(NPointsForMean, imagedata);
	}
	else{
		mean = CalcMeanOverCurrentList(NPointsForMean);
	}
//	ShowFloat(NPointsForMean, "NPointsForMean");


	int delta=0;
	ComponentPoint * LastPoint = (ComponentPoint * ) ListObject::Last(Points);
	while (true){
		if (InKey()==STOPKEY){
			return true;
		}
		CPoint TryPoint = points->point;
		
		for (int dx=-1; dx<2; dx++){
			for (int dy=-1; dy<2; dy++){
				CPoint TestPoint(TryPoint.x+dx, TryPoint.y+dy);
				if (PointIsInRectangle(TestPoint)){
					if (!Tested[TestPoint.x+TestPoint.y*width]){
						Tested[TestPoint.x+TestPoint.y*width] = true;
						if (!UsePoly || (poly&& (poly->IsInside(TestPoint)))){
								if (!Selected[TestPoint.x+TestPoint.y*width]){
												//					if (PointIsElegible(TestPoint, Selected)){
									double value = imagedata[TestPoint.x+TestPoint.y*width];
									if ( fabs((value-mean)/mean) <AbsThreshold){
										ComponentPoint * NewPoint = new ComponentPoint(width, height);
										NewPoint->parent = this;
										NewPoint->point = TestPoint;
										NewPoint->value = value;
										CopyToSelected(NewPoint, Selected);
//								Points = (ComponentPoint * )
										ListObject::AddListObjectAtEnd(LastPoint, NewPoint);
										LastPoint = (ComponentPoint * ) ListObject::Last(LastPoint);
										FoundNewPoint = true;
//								Restart = false;
										mean = (double(NPointsForMean)*mean+value)/double(NPointsForMean+1);
										NPointsForMean++;
										delta++;
										if (false){
//								if (delta>=1000){
											delta  = 0;
	//							Alert0("found");
											char s[20];
//									_itoa(Points->LengthList(Points), s, 10);
											_itoa(NPointsForMean, s, 10);
											pDC->TextOut(100,100, "                                ");
											pDC->TextOut(100,100, CString(s));
										} // if delta
								}
							} // if fabs
						} // if ! selected
					}  // if ! tested
				} //if in rect
			} //for dy
		}  // for dx
		points = (ComponentPoint *) points->Next;

		if (!points){
			Points = (ComponentPoint * ) ListObject::First(LastPoint);
			points = Points;
//			Restart = true;
			return true;
		}
	} // while true
	return true;
}
