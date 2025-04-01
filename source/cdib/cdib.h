// cdib.h declaration for Inside Visual C++ CDib class

#ifndef _INSIDE_VISUAL_CPP_CDIB
#define _INSIDE_VISUAL_CPP_CDIB

class CDib : public CObject
{
	enum Alloc {noAlloc, crtAlloc, heapAlloc};
	DECLARE_SERIAL(CDib)
public:
	
	LPVOID m_lpvColorTable;
	HBITMAP m_hBitmap;
	LPBYTE m_lpImage;  // starting address of DIB bits
	LPBITMAPINFOHEADER m_lpBMIH; //  buffer containing the BITMAPINFOHEADER
    int m_nColorTableEntries;
	DWORD m_dwSizeImage;    
private:
	HGLOBAL m_hGlobal; // For external windows we need to free;
	                   //  could be allocated by this class or allocated externally
	Alloc m_nBmihAlloc;
	Alloc m_nImageAlloc;
	
	HANDLE m_hFile;
	HANDLE m_hMap;
	LPVOID m_lpvFile;
	HPALETTE m_hPalette;
public:
	CDib();
	CDib(CSize size, int nBitCount);	// builds BITMAPINFOHEADER
	~CDib();
	int GetSizeImage() {return m_dwSizeImage;}
	int GetSizeHeader()
		{return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;}
	CSize GetDimensions();
	BOOL AttachMapFile(const char* strPathname, BOOL bShare = false);
	BOOL CopyToMapFile(const char* strPathname);
	BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = false, HGLOBAL hGlobal = NULL);
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);  // until we implemnt CreateDibSection
	HBITMAP CreateSection(CDC* pDC = NULL);
	UINT UsePalette(CDC* pDC, BOOL bBackground = false);
	BOOL MakePalette();
	BOOL SetSystemPalette(CDC* pDC);
	BOOL Compress(CDC* pDC, BOOL bCompress = true); // FALSE means decompress
	HBITMAP CreateBitmap(CDC* pDC);
	BOOL Read(CFile* pFile);
	BOOL ReadSection(CFile* pFile, CDC* pDC = NULL);
	BOOL Write(CFile* pFile);
	void Serialize(CArchive& ar);
	void Empty();
    BOOL IsEmpty();
    CSize GetDibSaveDim();
    LONG GetPixelOffset(int  x, int y);
	LONG CDib::GetAviPixelOffset(int  x, int y);
    RGBQUAD GetPixel(int x, int y);
    void WritePixel(int x, int y,RGBQUAD color);
    void CreateCDib(CSize size, int nBitCount);
	void CopyDib(CDib* pDibSrc);
	BOOL CDib::Save(LPCSTR lpszDibFile);
   //=================================================================================================
    
private:
	void DetachMapFile();
	void ComputePaletteSize(int nBitCount);
	void ComputeMetrics();
};
#endif // _INSIDE_VISUAL_CPP_CDIB