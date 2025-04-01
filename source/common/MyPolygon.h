#pragma once

class MyPolygon{
public:
static	bool pointInPolygon(int polySides, double x, double y, double * polyX, double * polyY ) ;
static	bool pointInPolygon(int polySides, int x, int y, int * polyX, int * polyY ) ;
static	bool pointInPolygon(int polySides, CPoint p, double * polyX, double * polyY ) ;
static	bool pointInPolygon(int polySides, CPoint p, CPoint * polyXY ) ;
static	bool pointInPolygon(int polySides, CPoint p, vector <CPoint> & polyXY ) ;


//******   inpoly uses a different algorithm, but is actually slower!!!

static  bool inpoly( int polySides, int x, int y, int * polyX, int * polyY);
static	bool inpoly(int polySides, CPoint p, CPoint * polyXY ) ;
};
