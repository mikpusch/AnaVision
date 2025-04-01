#pragma once

//#ifndef DRAWOBIMPORTED

//#define DRAWOBIMPORTED
//#define ImageDataType float

#include "../common/utils.h"
#include <vector>
using namespace std ;

#define AnaVisionImageDataType float

/*
inline int MultZoom(int x, int Zoom, double RealZoom){
	if (Zoom!=0){
		return (x*Zoom);
	}
	return int (double(x)*RealZoom);
}
inline int DivZoom(int x, int Zoom, double RealZoom){
	if (Zoom!=0){
		return (x/Zoom);
	}
	return int (double(x)/RealZoom);
}
*/
inline int MultZoom(int x, int Zoom, bool ZoomIn){
	if (ZoomIn){
		return (x/Zoom);
	}
	else{
		return (x*Zoom);
	}
}
inline int DivZoom(int x, int Zoom, bool ZoomIn){
	if (ZoomIn){
		return (x*Zoom);
	}
	else{
		return (x/Zoom);
	}
}
//inline int MultZoom(int x, int Zoom, double RealZoom);
//inline int DivZoom(int x, int Zoom, double RealZoom);

class Vect{
public:
	Vect(double xx, double yy);
	Vect(int x, int y);
	Vect(CPoint p);
	Vect();


	double x;
	double y;

	double SquareDistance(Vect v);
	double SquareDistance(CPoint p);

	double Length();
	double SquareLength();
	static double SquareDistance(Vect v1, Vect v2);
	static double SquareDistance(CPoint p1, CPoint p2);
	static double SquareDistance(Vect v1, CPoint p2);
	static double SquareDistance(CPoint p1, Vect v2);
	static double Length(Vect v);

	static Vect Add(Vect v1, Vect v2);
	static Vect Sub(Vect v1, Vect v2);

	static Vect RotateVect(Vect v1, double alpha);
	static void RotateVects(vector<Vect> * vects, double alpha);

	static Vect DiffPoints(CPoint p1, CPoint p2); // return p1 - p2
	static Vect DiffPoints(Vect v1, CPoint p2); // return p1 - p2
	static Vect DiffPoints(CPoint p1, Vect v2); // return p1 - p2
	static Vect AddPoints(CPoint p1, CPoint p2); 
	static Vect AddPoints(Vect v1, CPoint p2); 
	static Vect AddPoints(CPoint p1, Vect v2); 
	static double Dot(CPoint p1, CPoint p2); 
	static double Dot(Vect p1, Vect p2); 
	double Dot(CPoint p); 
	double Dot(Vect p); 
	static CPoint MakePoint(Vect v);
	static CPoint GetInterSection(CPoint P1, CPoint P2, CPoint P3, CPoint P4);
	static Vect ScaleVect(double lambda, Vect v); // return lambda*p1
	Vect ScaleVect(double lambda); // return lambda*p1
	void Scale(double lambda); // return lambda*p1
	void Normalize();
	static Vect Normalize(Vect v);
	static double GetAngle(Vect a, Vect b);
	static double GetAngle(Vect a);  // Gets angle with the positive x axis

	static double Cross(Vect a, Vect b);

	CPoint MakePoint();

	static Vect UnitVertVect(Vect v); // Make a unit vertical vector


};


#define MAXUNDOOBJECTPOINT 20

class ObjectPoint{
public:
	ObjectPoint();
	void ResetUndo(); // sets all undo positions to current position
	
	void SetPos(CPoint p);
	void SetPos(int x, int y);

	void SetPosWithUndo(CPoint p);
	void SetPosWithUndo(int x, int y);
	void SetPosWithUndo();

	CPoint GetPos(int Zoom, bool ZoomIn);
	int Getx(int Zoom,  bool ZoomIn );
	int Gety(int Zoom,  bool ZoomIn);

	double GetRadius( );

	void AddVector(CPoint vector);

	void Undo();
	void Redo();

	bool IsClicked(CPoint p);
	double SquareDistance(ObjectPoint p);
	double SquareDistance(CPoint p);

	static double SquareDistance(ObjectPoint p1, ObjectPoint p2);
	static double SquareDistance(CPoint p1, CPoint p2);
	static double LengthVector(CPoint p);

	static CPoint DiffPoints(CPoint p1, CPoint p2); // return p1 - p2
	static CPoint AddPoints(CPoint p1, CPoint p2); // return p1 - p2
	static double Dot(CPoint p1, CPoint p2); // return p1 . p2
	static CPoint ScaleVect(double lambda, CPoint p); // return lambda*p1

	Vect MakeVect();


//private:
	double radius;
	int UndoIndex;
	void Init();
	CPoint pos;
	CPoint oldpos[MAXUNDOOBJECTPOINT];


};

class PointList{
public:
	CPoint p;
	int index;
};


#define NPOINTSAREAOBJECT	4

class AreaObject{
public:
	AreaObject();
	~AreaObject();

	void Define(int type, CPoint p0, CPoint p1, CPoint p2, CPoint p3, int xsize, int ysize);
	void Define(int type, CPoint p[NPOINTSAREAOBJECT], int xsize, int ysize);


	AreaObject &operator=( AreaObject & );  // Right side is the argument.


	void SetSize(int xsize, int ysize);

	int ObjectType; // 0: Rectangle
					// 1: Conus

	// For Rectangle and Conus:
	ObjectPoint P[NPOINTSAREAOBJECT];

	void DrawObject(CDC * pDC, CRect & DrawRect, int Zoom,  bool ZoomIn );
	void DrawObject(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos, int Zoom,  bool ZoomIn);

	int IsClicked(CPoint p); 
		// returns -1 if not clicked; otherwise the index of the clicked point

	void SizePoint(int index, CPoint newpos);
	// recalculates the position of all other points based on the new coordinates of 
	// this point

	void StartRotate(int index);
	void StartSize(int index);

	void RotatePoint(int index, CPoint newpos);
	bool FixMouse();

	void MovePoint(int index, CPoint newpos);
	// recalculates the position of all other points based on the new coordinates of 
	// this point

	void Undo();
	void Redo();

	void SetChanged();

	double Length();

	int GetRelevantIndex(CPoint p);

	bool GetMidPointAndRadius(Vect & M, double & r1, double & r2);
	// For Conus

	Vect GetMidPoint();
	// For Rectangle

	bool PointIsInside(CPoint p);

	bool GeneratePointsOld(bool & initialize, CPoint & p, int & index);
	bool GeneratePoints(bool & initialize, CPoint & p, int & index);


//private:
/*	CPen * redPen, * bluePen, * blackPen;

	CPen grayPen;
*/

	int  XSIZE, YSIZE;
	bool CheckRect(CRect & rect);
	bool RectangleGeneratorInitialized;
	bool ConusGeneratorInitialized;

	void SizeRectangle(int index, CPoint newpos);
	void SizeConus(int index, CPoint newpos);
	void OldSizeConus(int index, CPoint newpos);
	void NotSoOldSizeConus(int index, CPoint newpos);
	void DrawRectangle(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos, int Zoom,  bool ZoomIn );
	void DrawConus(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos, int Zoom,  bool ZoomIn );

	void RotateRectangle(int index, CPoint newpos);
	void NewRotateRectangle(int index, CPoint newpos);
	void NewNewRotateRectangle(int index, CPoint newpos);
	void RotateConus(int index, CPoint newpos);

	void AdjustI2(int i1, int i2, int i3, int i0);
	void AdjustI0(int i1, int i2, int i3, int i0);

	bool AdjustConus(int i0, int i1, int i2, int i3);
	// adjust i2 and i3 such that they are parallel to i0,i1

	void AdjustConusIndexes();

	double LengthRectangle();
	double LengthConus();
	int GetIndexRectangle(CPoint p);
	int GetIndexConus(CPoint p);

	bool PointIsInsideRectangle(CPoint p);
	bool PointIsInsideConus(CPoint p);

	bool GeneratePointsRectangle(bool & initialize, CPoint & p, int & index);
	bool GeneratePointsConus(bool & initialize, CPoint & p, int & index);

	void NewAdjustCone(CPoint pA, CPoint pB ,CPoint pC, CPoint pD, CPoint & pa, CPoint & pb,  CPoint & pc, CPoint & pd) ;
	void VeryOldSizeConus(int index, CPoint newpos);

	void VeryNewRotateConus(int index, CPoint newpos);
	void LastNewRotateConus(int index, CPoint newpos);

	
	void CalcGeneratorList();


private:
	bool DoFixMouse;


	int SaveRotateSizeIndex;

	double SaveRotater1, SaveRotater2, SaveRotatealpha, SaveRotatAlpha1, SaveRotatAlpha2;
	ObjectPoint SaveRotatePoint, SaveRotateMidPoint;
	double CalcConusMTry(double mx, double my, ObjectPoint P, CPoint Q, double r1, double r2, double alpha);


	double SaveSizeri;
	bool IsInnerRadiusOnSize;
	ObjectPoint SaveSizePoint, SaveSizeMidPoint;

	void StartRotateConus(int index);
	void StartSizeConus(int index);

	double GetConusAngle();
	void LastOldRotateConus(int index, CPoint newpos);

/*
	int SizeGeneratorList;
	int BufferSizeGeneratorList;
	PointList * pointlist;
	void UpdateSizeGeneratorList(int length);
	void DeallocPointList();
*/
	BufferClass <PointList> pointlist  ;

	void CalcGeneratorListRectangle();
	void CalcGeneratorListConus();

};

class PolygonDataPoint{
public:
	int x;
	int y;
	AnaVisionImageDataType value;
	bool InsideRect;
};

class PolygonObject{
public:
	PolygonObject();
	~PolygonObject();
//	PolygonObject &operator=( PolygonObject & );  // Right side is the argument.
	PolygonObject &operator=( PolygonObject  );  // Right side is the argument.

	void Copy (PolygonObject & CopyTo);

	bool IsLine;

	void AddPoint(CPoint p);
	void GetVerticalPoints(CPoint P1, CPoint P2, CPoint & Pout, CPoint & Pin, int MembraneSize);
	int NPoints();
	bool IsInside(CPoint p);
	bool IsInside(int x, int y);
	void Draw(CDC * pDC, int offsetx, int offsety, int index , int Zoom,  bool ZoomIn, bool PreliminaryDraw = false);
//	void Draw(CDC * pDC, int index = -1, int Zoom = 1);
//	void Draw(CDC * pDC, int index , int Zoom);
	int ClickedOnAPoint(CPoint p);
	double CalcMeanValue(double Back, double ** image, int xsize, int ysize, int offsetx,
							int offsety, int CalcType);
	double CalcMeanValue(float Back, float ** image, int xsize, int ysize, int offsetx,
							int offsety, int CalcType);
	double CalcMeanValue(int Back, int ** image, int xsize, int ysize, int offsetx,
							int offsety, int CalcType);

	double CalcMembraneValue(float Back, float ** image, int xsize, int ysize, int offsetx,
								int offsety,
								int MembraneSize, bool IsCircular, int CalcType);
	double CalcMembraneValue(double Back, double ** image, int xsize, int ysize, int offsetx,
								int offsety,
								int MembraneSize, bool IsCircular, int CalcType);
	double CalcMembraneValue(int Back, int ** image, int xsize, int ysize, int offsetx,
								int offsety,
								int MembraneSize, bool IsCircular, int CalcType);
	double CalcLocalizationCoefficient(double * Background, float ** I1, float ** I2, int xsize, int ysize,
		int offsetx, int offsety, int CalcType, double Threshold);



	void Track(	int TrackMode,  // 0=Diff, 1=CrossCorr, 2=DiffMean
				AnaVisionImageDataType ** RefImage, AnaVisionImageDataType ** Image, int xsize, int ysize, 
				int offsetrefx, int offsetrefy, int & offsetx, int & offsety);
	// The result of this procedure is new offset values
	int GetNumberOfPointsInPolygon();
	void GetArrayOfPointsInPolygon( PolygonDataPoint * & ArrayOfPoints);

	double CalcTrackSD(int TrackMode, 
						int offx, int offy, int NP, PolygonDataPoint *  RefPoints, 
							PolygonDataPoint *  TrackPoints,
						 AnaVisionImageDataType ** TrackImage,int xsize, int ysize);

	double CalcDiffImage(int offx, int offy, int NP, PolygonDataPoint *  RefPoints,
						 AnaVisionImageDataType ** TrackImage,int xsize, int ysize);
	double CalcCrossCorrel(int offx, int offy, int NP, PolygonDataPoint *  RefPoints, 
							PolygonDataPoint *  TrackPoints,
						 AnaVisionImageDataType ** TrackImage,int xsize, int ysize);

	double CalcDiffMean(int offx, int offy, int NP, PolygonDataPoint *  RefPoints, 
							PolygonDataPoint *  TrackPoints,
						 AnaVisionImageDataType ** TrackImage,int xsize, int ysize);

	void GetPoint(int index , CPoint & p);

	void ShiftPoints(int dx, int dy);
	int GetNumberOfVertices();


//	BufferClass <PointList> pointlist  ;
//	vector <PointList> pointlist  ;
	vector <CPoint> pointlist  ;

//	void UpdateInsidePrecalc();

	vector <CPoint> DataPoints;
	void UpdateDataPoints();
	int GetNumberOfDataPoints();

	void MakeMembranePolygons(PolygonObject *POut1,
							  PolygonObject *POut2,
							  PolygonObject *PIn1,
							  PolygonObject *PIn2,
							  int MembraneSize, bool IsCircular);

	bool MakePoygonAroundLine(PolygonObject & Poly, // the output
							  int size);

	double CalcLength();

	CPoint CalcCenter();

private:
	double DoCalcMeanValue(double Back, int xsize, int ysize, int offsetx, int offsety, int & NData, int CalcType);
	double DoCalcMembraneValue(double Back, int xsize, int ysize, int offsetx, int offsety,
				int MembraneSize, bool IsCircular, int CalcType);
	double DoCalcLocalizationCoefficient( double * Background,
		int xsize, int ysize, int offsetx, int offsety, int & NData, int CalcType, double Threshold);

	double OldDoCalcMeanValue(int xsize, int ysize, int offsetx, int offsety);
	double GetImageValue(int x, int y, char * ptr);
	char * imageptr;
	char * imageptr2;
	int ImageType;
	int MinX, MaxX, MinY, MaxY;

	void GetNewPoints(CPoint P1, CPoint P2, int MembraneSize,
									CPoint & P1Out, 
									CPoint & P1In,
									CPoint & P2Out,
									CPoint & P2In);



//	vector<bool> InsidePrecalc; // for x>=MinX, x<=MaxX , y>=MinY, y<=MaxY:
								// InsidePrecalc[x-Minx+(y-MinY)*(MaxX-MinX+1)] is true or false!


};

class ArrayOfPolygons{
public:
	ArrayOfPolygons();
	~ArrayOfPolygons();
	bool AreLines;
	void AddPolygon(PolygonObject poly);
	PolygonObject * GetPoly(int index);
	int NPoly();
//	void DrawPolys(CDC * pDC, int selectedpolygon, int Zoom);
	void DrawPolys(CDC * pDC, int selectedpolygon, int Zoom, int offsetx, int offsety,  bool ZoomIn );
	int ClickedOnAPoint(CPoint p, int & corner); // return the index of the polygon <0 if not found
	bool SaveOnFile(CFile * fp, int & byteswritten, int Version); // return #bytes written
	bool LoadFromFile(CFile * fp, int & bytesavailable, int Version ); // return #bytes written
	void DeletePolygon(int index);
	bool IsInside(CPoint p, int index);
	void MovePolygon(int poly, int corner, CPoint p, CDC * pDC, int Zoom, int offsetx, int offsety,  bool ZoomIn );
	double CalcMeanValue(double Back, int poly, double ** image, int xsize, int ysize,
							int offsetx, int offsety, int CalcType);
	double CalcMeanValue(float Back, int poly, float ** image, int xsize, int ysize,
							int offsetx, int offsety, int CalcType);
	double CalcMeanValue(int Back, int poly, int ** image, int xsize, int ysize, int offsetx,
							int offsety, int CalcType);

	double CalcMembraneValue(int Back, int poly, int ** image, int xsize, int ysize, int offsetx,
								int offsety,
								int MembraneSize, bool RoiIsCircular, int CalcType);
	double CalcMembraneValue(float Back, int poly, float ** image, int xsize, int ysize,
								int offsetx, int offsety,
								int MembraneSize, bool RoiIsCircular, int CalcType);
	double CalcLocalizationCoefficient(int poly, double * Background, float ** I1, float ** I2, int xsize, int ysize,
		int offsetx, int offsety, int CalcType, double Threshold);
	void MakeMembranePolys(int poly, int MembraneSize, bool RoiIsCircular, bool AssumeIsALine = false);
	void MakeGradientPolysAssumePolyIsaLine(int poly, int MembraneSize, bool RoiIsCircular);
	void MakePolygonAroundLine(int selected, int MembraneSize, ArrayOfPolygons & WhereToAppend);
	void Createnpolygonsinside(int selected, int NPoly);

	void Track(
		int TrackMode,  // 0=Diff, 1=CrossCorr, 2=DiffMean
		int selectedpoly, AnaVisionImageDataType ** RefImage, AnaVisionImageDataType ** Image,
					int xsize, int ysize,
					int offsetrefx, int offsetrefy, int & offsetx, int & offsety);


//	BufferClass <PolygonObject> polygons;
	vector <PolygonObject> polygons;
};

template <class T>
class EdgePoint{
public:
	EdgePoint();
	CPoint point;
	T value;
//	int index;
	bool NotYetUsedInPolygon;
};
template <class T>
EdgePoint<T>::EdgePoint(){
	NotYetUsedInPolygon = true;
}

class VierEck{
public:
	CPoint p[4];

	void DrawLine(CDC * pDC);
	void Rotate(double alpha);
};

void ShowPoint ( CPoint p);
void DrawFilledGradientArrow(
					CDC * pC,
					CPoint StartPoint,
					CPoint EndPoint,
					int rs, int gs, int bs,
					int re, int ge, int be,
					int WidthLine,
					int WidthArrow,
					int SizeTip);

//#endif