#include "stdafx.h"
#include "MyPolygon.h"
/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "utils.h"
#include "nrutil.h"
#include "simplex.h"
#include "../common/MTRand.h"
*/

//  Globals which should be set before calling this function:
//
//  int    polySides  =  how many corners the polygon has
//  float  polyX[]    =  horizontal coordinates of corners
//  float  polyY[]    =  vertical coordinates of corners
//  float  x, y       =  point to be tested
//
//  (Globals are used in this example for purposes of speed.  Change as
//  desired.)
//
//  The function will return YES if the point x,y is inside the polygon, or
//  NO if it is not.  If the point is exactly on the edge of the polygon,
//  then the function may return YES or NO.
//
//  Note that division by zero is avoided because the division is protected
//  by the "if" clause which surrounds it.

bool MyPolygon::pointInPolygon(int polySides, double x, double y, double * polyX, double * polyY ) {

  int      i, j=polySides-1 ;
  bool  oddNodes=false      ;

  for (i=0; i<polySides; i++) {
    if (polyY[i]<y && polyY[j]>=y
    ||  polyY[j]<y && polyY[i]>=y) {
      if (polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x) {
//      if (polyX[i]+    (y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])     <x) {
        oddNodes=!oddNodes; }}
    j=i; }

  return oddNodes;
}

bool MyPolygon::pointInPolygon(int polySides, int x, int y, int * polyX, int * polyY ) {

  int      i, j=polySides-1 ;
  bool  oddNodes=false      ;

  for (i=0; i<polySides; i++) {
    if (polyY[i]<y && polyY[j]>=y
    ||  polyY[j]<y && polyY[i]>=y) {
      if (polyX[i]+ int(double(y-polyY[i])/double(polyY[j]-polyY[i])*double(polyX[j]-polyX[i]))<x) {
        oddNodes=!oddNodes; }}
    j=i;
	}
  return oddNodes;
} 

bool MyPolygon::pointInPolygon(int polySides, CPoint p, double * polyX, double * polyY ) {
	return pointInPolygon(polySides, double(p.x), double(p.y), polyX, polyY);
}

bool MyPolygon::pointInPolygon(int polySides, CPoint p, vector <CPoint> & polyXY ){
//bool MyPolygon::pointInPolygon(int polySides, CPoint p, PointList * polyXY ){
	int  i;
	int	 j=polySides-1 ;
	bool oddNodes=false      ;
	int x=p.x;
	int y=p.y;

	for (i=0; i<polySides; i++) {
		int pyi=polyXY[i].y;
		int pyj=polyXY[j].y;
		if (    ((pyi<y) && (pyj>=y) )     || ( (pyj<y) && (pyi>=y))) {
			int xi=polyXY[i].x;
			int xj=polyXY[j].x;
			if (xi + int(double(y-pyi)/double(pyj-pyi)*double(xj-xi))<x) {
				oddNodes=!oddNodes;
			}
		}
		j=i;
	}
	return oddNodes;
} 



bool                                /*   1=inside, 0=outside                */
MyPolygon::inpoly(                            /* is target point inside a 2D polygon? */
//unsigned int poly[][2],            /*   polygon points, [0]=x, [1]=y       */
//int npoints,                       /*   number of points in polygon        */
//unsigned int xt,                   /*   x (horizontal) of target point     */
//unsigned int yt)                   /*   y (vertical) of target point       */
int polySides, int xt, int yt, int * polyX, int * polyY)
{
     int xnew,ynew;
     int xold,yold;
     int x1,y1;
     int x2,y2;
     int i;
     bool inside=false;

     if (polySides < 3) {
          return false;
     }
     xold=polyX[polySides-1];
     yold=polyY[polySides-1];
     for (i=0 ; i < polySides ; i++) {
          xnew=polyX[i];
          ynew=polyY[i];
          if (xnew > xold) {
               x1=xold;
               x2=xnew;
               y1=yold;
               y2=ynew;
          }
          else {
               x1=xnew;
               x2=xold;
               y1=ynew;
               y2=yold;
          }
          if ((xnew < xt) == (xt <= xold)          /* edge "open" at one end */
           && (yt-y1)*(x2-x1)
            < (y2-y1)*(xt-x1)) {
               inside=!inside;
          }
          xold=xnew;
          yold=ynew;
     }
     return(inside);
}

bool MyPolygon::inpoly(int polySides, CPoint p, CPoint * polyXY ) {
	int xt = p.x;
	int yt=p.y;
     int xnew,ynew;
     int xold,yold;
     int x1,y1;
     int x2,y2;
     int i;
     bool inside=false;

     if (polySides < 3) {
          return false;
     }
     xold=polyXY[polySides-1].x;
     yold=polyXY[polySides-1].y;
     for (i=0 ; i < polySides ; i++) {
          xnew=polyXY[i].x;
          ynew=polyXY[i].y;
          if (xnew > xold) {
               x1=xold;
               x2=xnew;
               y1=yold;
               y2=ynew;
          }
          else {
               x1=xnew;
               x2=xold;
               y1=ynew;
               y2=yold;
          }
          if ((xnew < xt) == (xt <= xold)          /* edge "open" at one end */
           && (yt-y1)*(x2-x1)
            < (y2-y1)*(xt-x1)) {
               inside=!inside;
          }
          xold=xnew;
          yold=ynew;
     }
     return(inside);
}

