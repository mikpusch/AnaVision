#include "stdafx.h"

#include <math.h>

#include "../common/nrutil.h"
#include "../common/Simplex.h"


Simplex::Simplex(){
}

void Simplex::InitSimplex (double StepS,
					  double *Params,
					  int NParams )
{

  double Root, N, p, q;
  double Root2=1.41421356;
  int i, j;

  Best = 0;
  SimpNParams = NParams;
  SimpFlag = 0;
  N=SimpNParams;
  Root = sqrt(N + 1.0);

  p = StepS * (Root + N - 1.0) / N / Root2;
  q = StepS * (Root - 1.0) / N / Root2;

  for (i = 0; i<SimpNParams; i++)
     Vertices[0][i] = Params[i];

  for (i = 1; i<=SimpNParams; i++)
   {
    for (j = 0; j<SimpNParams; j++)
     {
       Vertices[i][j] = Vertices[0][j] * (1.0 + q);
     }
    Vertices[i][i - 1] = Vertices[0][i - 1] * (1.0 + p);
   }
}


void Simplex::GetBestParams(double * Params){
	for (int i=0; i<SimpNParams; i++)
		Params[i]=Vertices[Best][i];
}

void Simplex::CopyParams(double *  from, double * to){
	for (int i=0; i<SimpNParams; i++)
		to[i]=from[i];
}


void Simplex::Contract (double *Params, int Best, int Worst)
{
 int i, j;
 double center, tp1;

 tp1=SimpNParams;

 for (i = 0; i<SimpNParams; i++)
  {
   center = 0;
   for (j = 0; j<=SimpNParams; j++)
    {
     if (j != Worst)
      {
		center += Vertices[j][i];
      }
     }
   Params[i] = 0.5 * (center / tp1 + Vertices[Worst][i]);
  }
}

void Simplex::Expand (double *Params, int Best, int Worst)
{
 int i, j;
 double center, tp1;
 tp1=SimpNParams;

 for (i = 0; i<SimpNParams; i++)
  {
   center = 0;
   for (j = 0; j<= SimpNParams; j++)
    {
      if (j != Worst)
       {
	  center += Vertices[j][i];
       }
     }
   Params[i] = 2.0 * Vertices[Worst][i] - center / tp1;
  }
}

void  Simplex::Order()
{
 int i;

 Best = 0;
 Worst = 0;
 for (i= 0; i<= SimpNParams; i++)
  {
   if (Vertices[i][SimpNParams] < Vertices[Best][SimpNParams])
     Best = i;

   if (Vertices[i][SimpNParams] > Vertices[Worst][SimpNParams])
     Worst = i;
  }
}

void Simplex::Reflect (double *Params, int Best, int Worst)
{
 int i, j;
 double center, tp1;

 tp1=SimpNParams;
 for (i = 0; i< SimpNParams; i++)
  {
   center = 0;
   for (j = 0; j<= SimpNParams; j++)
    {
      if (j != Worst)
	center += Vertices[j][i];
    }
   Params[i] = 2.0 * center / tp1 - Vertices[Worst][i];
  }
 }

void Simplex::Shrink (int Best)
{
 int j, i;
 double Save;

 for (j = 0; j<=SimpNParams; j++)
  for (i = 0; i<SimpNParams; i++)
    Vertices[j][i] = 0.5 * (Vertices[j][i] + Vertices[Best][i]);

 for (i = 0; i<=SimpNParams; i++)
  {
    Save = Vertices[Best][i];
    Vertices[Best][i] = Vertices[0][i];
    Vertices[0][i] = Save;
  }
}

void Simplex::NextSimp (double *Params, double Residuum)
{
int i;

if (SimpFlag <= SimpNParams)
 {
   Vertices[SimpFlag][SimpNParams] = Residuum;
   SimpFlag++;

   if (SimpFlag <= SimpNParams)
    {
      for (i = 0; i<SimpNParams; i++)
	  Params[i] = Vertices[SimpFlag][i];
      return;
    }
  }

if (SimpFlag == (SimpNParams + 1))   /* All vertices have been calculated */
				     /* Now find best and worst */
 {
   Order();
   Reflect(Params, Best, Worst);
   SimpFlag = SimpNParams + 2;
   return;
 }

if (SimpFlag == (SimpNParams + 2)) /* Test the reflected vertix */
 {
   if (Residuum < Vertices[Best][SimpNParams])
    {
      BestResid=Residuum;
      Expand(Params, Best, Worst);
      SimpFlag = SimpNParams + 3;
      return;
    }
   else
    {
      if (Residuum < Vertices[Worst][SimpNParams])
       {
	 Reflect(Params, Best, Worst);
	 for (i = 0; i< SimpNParams; i++)
	    Vertices[Worst][i] = Params[i];

	 Vertices[Worst][SimpNParams] = Residuum;
	 Order();
	 Reflect(Params, Best, Worst);
	 SimpFlag = SimpNParams + 2;

	 return;
	}
       else
	{
	  Contract(Params, Best, Worst);
	  SimpFlag = SimpNParams + 4;
	  return;
	}
       }
      }

if (SimpFlag == (SimpNParams + 3))     /* test expanded vertix */
 {
   if  (Residuum < Vertices[Best][SimpNParams])
    {
      Expand(Params, Best, Worst);
    }
   else
    {
     Reflect(Params, Best, Worst);
     Residuum = BestResid;
    }

   for (i = 0; i<SimpNParams; i++)
     Vertices[Worst][i] = Params[i];

   Vertices[Worst][SimpNParams] = Residuum;

   Order();
   Reflect(Params, Best, Worst);
   SimpFlag = SimpNParams + 2;
   return;
 }

if (SimpFlag == (SimpNParams + 4)) /* Test contracted vertix */
 {
   if (Residuum < Vertices[Worst][SimpNParams])
    {
      Contract(Params, Best, Worst);
      for (i = 0; i<SimpNParams; i++)
	  Vertices[Worst][i] = Params[i];

      Vertices[Worst][SimpNParams] = Residuum;
      Order();
      Reflect(Params, Best, Worst);
      SimpFlag = SimpNParams + 2;
      return;
    }
   else
    {
      Shrink(Best);
      for (i = 0; i<SimpNParams; i++)
	  Params[i] = Vertices[1][i];

      SimpFlag = 1;
      return;
    }
   }

}




