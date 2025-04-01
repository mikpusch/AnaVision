#ifndef DRAWOBIMPORTED

#define DRAWOBIMPORTED

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
	static Vect ScaleVect(double lambda, Vect v); // return lambda*p1
	Vect ScaleVect(double lambda); // return lambda*p1
	void Scale(double lambda); // return lambda*p1
	void Normalize();
	static Vect Normalize(Vect v);
	static double GetAngle(Vect a, Vect b);
	static double GetAngle(Vect a);  // Gets angle with the positive x axis


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

	CPoint GetPos();
	int Getx();
	int Gety();

	double GetRadius();

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

#define NPOINTSAREAOBJECT	4

class AreaObject{
public:
	AreaObject();
	~AreaObject();

	void Define(int type, CPoint p0, CPoint p1, CPoint p2, CPoint p3, int xsize, int ysize);
	void Define(int type, CPoint p[NPOINTSAREAOBJECT], int xsize, int ysize);


	void SetSize(int xsize, int ysize);

	int ObjectType; // 0: Rectangle
					// 1: Conus

	// For Reactangle and Conus:
	ObjectPoint P[NPOINTSAREAOBJECT];

	void DrawObject(CDC * pDC, CRect & DrawRect);
	void DrawObject(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos);

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
	void DrawRectangle(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos);
	void DrawConus(CDC * pDC, CRect & DrawRect, int ncursors, double * curspos);

	void RotateRectangle(int index, CPoint newpos);
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




};

#endif