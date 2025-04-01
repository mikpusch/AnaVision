// DibUtil.h: interface for the CDibUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIBUTIL_H__3F345EA3_59E3_44E1_8032_4BE10FBD958F__INCLUDED_)
#define AFX_DIBUTIL_H__3F345EA3_59E3_44E1_8032_4BE10FBD958F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDib.h"

class CDibUtil  
{
public:
	CDibUtil();
	virtual ~CDibUtil();

// Static Operations
public:
	static BOOL ReadImage(CFile& file, CDib& cdib);
	static BOOL ReadImage(const CString& filename, CDib& cdib);
	static VOID RandomPixels(CDib& src, CDib& dest, LONG count);
	static VOID CopyRect(CDib& src, CDib& dest, const CRect rect);
	static VOID Graying(CDib& src, CDib& dest);
	static VOID Binaryzation(CDib& src, CDib& dest, BYTE threshold);
	static VOID Subtract(CDib& imgA, CDib& imgB, CDib& dest);
	static VOID Translation(CDib& src, CDib& dest, CPoint& translation);
	static VOID Rotation(CDib& src, CDib& dest, DOUBLE angle);
};

#endif // !defined(AFX_DIBUTIL_H__3F345EA3_59E3_44E1_8032_4BE10FBD958F__INCLUDED_)