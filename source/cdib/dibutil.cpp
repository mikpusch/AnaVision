// DibUtil.cpp: implementation of the CDibUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DibUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define M_PI 3.14159265358979323846
#include <math.h>

// gray = red * 0.299 + green * 0.587 + blue * 0.114
#define GRAYING(R, G, B) (BYTE)(((UINT)B * 29 + (UINT)G * 150 + (UINT)R * 77 + 128) >> 8)

inline RGBQUAD RGBSUB(RGBQUAD A, RGBQUAD B) {
	RGBQUAD S;
	SHORT tmp;
	S.rgbBlue = (tmp = A.rgbBlue - B.rgbBlue) < 0 ? 0 : tmp;
	S.rgbGreen = (tmp = A.rgbGreen - B.rgbGreen) < 0 ? 0 : tmp;
	S.rgbRed = (tmp = A.rgbRed - B.rgbRed) < 0 ? 0 : tmp;
	return S;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDibUtil::CDibUtil()
{

}

CDibUtil::~CDibUtil()
{

}

BOOL CDibUtil::ReadImage(const CString& filename, CDib& cdib)
{
	CFile file;
	if (!file.Open(filename, CFile::modeRead))
		return false;

	return ReadImage(file, cdib);
}

BOOL CDibUtil::ReadImage(CFile& file, CDib& cdib)
{
	if(!cdib.Read(&file))
		return false;

	if (cdib.m_lpBMIH->biCompression != BI_RGB)
		return false;

	return true;
}

VOID CDibUtil::RandomPixels(CDib& src, CDib& dest, LONG count)
{
	dest.CopyDib(&src);
	CSize imageSize = src.GetDimensions();
	
	srand( (UINT)time( NULL ) );
	
	RGBQUAD color;
	LONG x, y;
	for (LONG i = 0; i != count; ++i)
	{
		// Random color
		color.rgbRed = (BYTE)((DOUBLE)rand() / (RAND_MAX + 1) * 255);
		color.rgbGreen = (BYTE)((DOUBLE)rand() / (RAND_MAX + 1) * 255);
		color.rgbBlue = (BYTE)((DOUBLE)rand() / (RAND_MAX + 1) * 255);
		
		// Random position
		x = (LONG)((DOUBLE)rand() / (RAND_MAX + 1) * imageSize.cx);
		y = (LONG)((DOUBLE)rand() / (RAND_MAX + 1) * imageSize.cy);
		
		// Write
		dest.WritePixel(x, y, color);
	}
}

VOID CDibUtil::CopyRect(CDib& src, CDib& dest, const CRect rect)
{
	// TODO: The validation of rect in source and destination
	// images should be checked
	for (LONG y = rect.top; y <= rect.bottom; ++y)
		for (LONG x = rect.left; x <= rect.right; ++x)
			dest.WritePixel(x, y, src.GetPixel(x, y));
}

VOID CDibUtil::Graying(CDib& src, CDib& dest)
{
	dest.CopyDib(&src);
	CSize imageSize = src.GetDimensions();

	RGBQUAD color;
	for (LONG y = 0; y != imageSize.cy; ++y)
		for (LONG x = 0; x != imageSize.cx; ++x)
		{
			color = src.GetPixel(x, y);
			color.rgbRed = color.rgbGreen = color.rgbBlue
				= GRAYING(color.rgbRed, color.rgbGreen, color.rgbBlue);
			dest.WritePixel(x, y, color);
		}
}

VOID CDibUtil::Binaryzation(CDib& src, CDib& dest, BYTE threshold)
{
	dest.CopyDib(&src);
	CSize imageSize = src.GetDimensions();
	
	RGBQUAD color;
	for (LONG y = 0; y != imageSize.cy; ++y)
		for (LONG x = 0; x != imageSize.cx; ++x)
		{
			color = src.GetPixel(x, y);
			color.rgbRed = color.rgbGreen = color.rgbBlue
				= (GRAYING(color.rgbRed, color.rgbGreen, color.rgbBlue) < threshold) ? 0x00 : 0xFF;
			dest.WritePixel(x, y, color);
		}
}

VOID CDibUtil::Subtract(CDib& imgA, CDib& imgB, CDib& dest)
{
	dest.CopyDib(&imgA);
	CSize imageSize = imgA.GetDimensions();

	for (LONG y = 0; y != imageSize.cy; ++y)
		for (LONG x = 0; x != imageSize.cx; ++x)
		{
			dest.WritePixel(x, y, RGBSUB(imgA.GetPixel(x, y), imgB.GetPixel(x, y)));
		}
}

VOID CDibUtil::Translation(CDib& src, CDib& dest, CPoint& translation)
{
	CSize imageSize = src.GetDimensions();

	ASSERT(imageSize.cx + translation.x >= 0);
	ASSERT(imageSize.cy + translation.y >= 0);
	dest.CreateCDib(CSize(imageSize.cx + translation.x, imageSize.cy + translation.y),
		src.m_lpBMIH->biBitCount);

	for (LONG y = 0, dy = translation.y; y != imageSize.cy; ++y, dy = y + translation.y)
		for (LONG x = 0, dx = translation.x; x != imageSize.cx; ++x, dx = x + translation.x)
		{
			if (dy >= 0 && dx >= 0)
			{
				dest.WritePixel(dx, dy, src.GetPixel(x, y));
			}
		}
}

/*
 * See http://www.leunen.com/cbuilder/rotbmp.html
 */
VOID CDibUtil::Rotation(CDib& src, CDib& dest, DOUBLE angle)
{
	CSize imageSize = src.GetDimensions();
	LONG width = imageSize.cx;
	LONG height = imageSize.cy;

	// Convert angle to radians
	FLOAT radians = (FLOAT)(angle * M_PI / 180.0);
	FLOAT cosine = (FLOAT)cos(radians);
	FLOAT sine = (FLOAT)sin(radians);
	
	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	FLOAT x1 = (-height * sine);
	FLOAT y1 = ( height * cosine);
	FLOAT x2 = ( width  * cosine - height * sine);
	FLOAT y2 = ( height * cosine + width * sine);
	FLOAT x3 = ( width  * cosine);
	FLOAT y3 = ( width  * sine);
	
	FLOAT minx = min(0, min(x1, min(x2, x3)));
	FLOAT miny = min(0, min(y1, min(y2, y3)));
	FLOAT maxx = max(x1, max(x2, x3));
	FLOAT maxy = max(y1, max(y2, y3));
	
	LONG newWidth = (LONG)ceil(fabs(maxx) - minx);
	LONG newHeight = (LONG)ceil(fabs(maxy) - miny);

	dest.CreateCDib(CSize(newWidth, newHeight), src.m_lpBMIH->biBitCount);

	LONG sx, sy;
	for(LONG y = 0; y < newHeight; ++y )
		for(LONG x = 0; x < newWidth; ++x )
		{
			sx = (LONG)((x + minx) * cosine + (y + miny) * sine);
			sy = (LONG)((y + miny) * cosine - (x + minx) * sine);
			if( sx >= 0 && sx < width && sy >= 0 && sy < height )
				dest.WritePixel(x, y, src.GetPixel(sx, sy));
		}
}