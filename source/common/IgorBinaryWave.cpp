#include "stdafx.h"

#include "../common/IgorBin.h"
#include "../common/utils.h"


#define CP_FILE_OPEN_ERROR 10000
#define CP_FILE_CLOSE_ERROR 10001
#define CP_FILE_EOF_ERROR 10002
#define CP_FILE_READ_ERROR 10003
#define CP_FILE_WRITE_ERROR 10004
#define CP_FILE_POS_ERROR 10005

#define CP_FILE_REF FILE*


int CPCreateFile(const char* fullFilePath, int overwrite, long macCreator, long macFileType);
int CPDeleteFile(const char* fullFilePath);
int CPOpenFile(const char* fullFilePath, int readOrWrite, CP_FILE_REF* fileRefPtr);
int CPCloseFile(CP_FILE_REF fileRef);
int CPReadFile(CP_FILE_REF fileRef, unsigned long count, void* buffer, unsigned long* numBytesReadPtr);
int CPReadFile2(CP_FILE_REF fileRef, unsigned long count, void* buffer, unsigned long* numBytesReadPtr);
int CPWriteFile(CP_FILE_REF fileRef, unsigned long count, const void* buffer, unsigned long* numBytesWrittenPtr);
int CPGetFilePosition(CP_FILE_REF fileRef, unsigned long* filePosPtr);
int CPSetFilePosition(CP_FILE_REF fileRef, long filePos, int mode);
int CPAtEndOfFile(CP_FILE_REF fileRef);
int CPNumberOfBytesInFile(CP_FILE_REF fileRef, unsigned long* numBytesPtr);


// This file contains utilities for cross-platform file I/O.

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#ifdef WIN32
	#include <Windows.h>
#endif

// The Windows headers create the WIN32 symbol if we are compiling for Windows.
// Here, we create an analogous MACINTOSH symbol if we are compiling for Macintosh.
#if (defined(GENERATINGPOWERPC) || defined(GENERATING68K))
	#define MACINTOSH 1
#endif


/*	CPCreateFile(fullFilePath, overwrite, macCreator, macFileType)

	Creates a file with the location and name specified by fullFilePath.
	
	fullFilePath must be a native path.

	If overwrite is true and a file by that name already exists, it first
	deletes the conflicting file. If overwrite is false and a file by that
	name exists, it returns an error.
	
	macFileType is ignored on Windows. On Macintosh, it is used to set
	the new file's type. For example, use 'TEXT' for a text file.
	
	macCreator is ignored on Windows. On Macintosh, it is used to set
	the new file's creator code. For example, use 'IGR0' (last character is zero)
	for an file.
	
	Returns 0 if OK or an error code.
*/
int
CPCreateFile(const char* fullFilePath, int overwrite, long macCreator, long macFileType)
{
	int err;
		
	if (overwrite)							// Delete file if it exists and if overwrite is specified.
		CPDeleteFile(fullFilePath);			// Ignore error.

	#ifdef MACINTOSH
		if (err = create(fullFilePath, 0, macCreator, macFileType))
			return err;
		return 0;
	#endif
	
	#ifdef WIN32
	{
		HANDLE fileH;
		long accessMode, shareMode;
		
		err = 0;
		accessMode = GENERIC_READ | GENERIC_WRITE;
		shareMode = 0;
		fileH = CreateFile(fullFilePath, accessMode, shareMode, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (fileH == INVALID_HANDLE_VALUE)
			err = GetLastError();
		else
			CloseHandle(fileH);
		return err;
	}
	#endif
}

/*	CPDeleteFile(fullFilePath)

	Deletes the file specified by fullFilePath.
	
	fullFilePath must be a native path.
	
	Returns 0 if OK or an error code.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPDeleteFile(const char* fullFilePath)
{
	#ifdef MACINTOSH
		int err;

		if (err = fsdelete(fullFilePath, 0))
			return err;
		return 0;
	#endif
	
	#ifdef WIN32
	{
		int err;

		err = 0;
		if (DeleteFile(fullFilePath) == 0)
			err = GetLastError();
		return err;
	}
	#endif
}

/*	CPOpenFile(fullFilePath, readOrWrite, fileRefPtr)

	If readOrWrite is zero, opens an existing file for reading and returns a file reference
	via fileRefPtr.

	If readOrWrite is non-zero, opens an existing file for writing or creates a new
	file if none exists and returns a file reference via fileRefPtr.

	fullFilePath must be a native path.
	
	Returns 0 if OK or an error code.
*/
int
CPOpenFile(const char* fullFilePath, int readOrWrite, CP_FILE_REF* fileRefPtr)
{
	*fileRefPtr = fopen(fullFilePath, readOrWrite ? "wb" : "rb");
	if (*fileRefPtr == NULL)
		return CP_FILE_OPEN_ERROR;
	return 0;
}

/*	CPCloseFile(fileRef)

	Closes the referenced file.
	
	Returns 0 if OK or an error code.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPCloseFile(CP_FILE_REF fileRef)
{
	if (fclose(fileRef))
		return CP_FILE_CLOSE_ERROR;
	return 0;
}

/*	CPReadFile(fileRef, count, buffer, numBytesReadPtr)

	Reads count bytes from the referenced file into the buffer.
	
	If numBytesReadPtr is not NULL, stores the number of bytes read in
	*numBytesReadPtr.
	
	Returns 0 if OK or an error code.
	
	If bytes remain to be read in the file and you ask to read more bytes
	than remain, the remaining bytes are returned and the function result is
	zero. If no bytes remain to be read in the file and you ask to read bytes,
	no bytes are returned and the function result is CP_FILE_EOF_ERROR.
	
	CPReadFile is appropriate when you are reading data of variable size, in
	which case you do not want to consider it an error if the end of file is reached
	before reading all of the bytes that you requested. If you are reading a
	record of fixed size, use use CPReadFile2 instead of CPReadFile.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPReadFile(CP_FILE_REF fileRef, unsigned long count, void* buffer, unsigned long* numBytesReadPtr)
{
	size_t numBytesRead;
	
	if (count == 0) {
		if (numBytesReadPtr != NULL)
			*numBytesReadPtr = 0;
		return 0;
	}
	
	clearerr(fileRef);
	numBytesRead = fread(buffer, 1, count, fileRef);
	if (numBytesReadPtr != NULL)
		*numBytesReadPtr = (unsigned long) numBytesRead;
	if (ferror(fileRef))
		return CP_FILE_READ_ERROR;
	if (numBytesRead==0 && CPAtEndOfFile(fileRef))
		return CP_FILE_EOF_ERROR;			// We were at the end of file when asked to read some bytes.
	return 0;
}

/*	CPReadFile2(fileRef, count, buffer, numBytesReadPtr)

	Reads count bytes from the referenced file into the buffer.
	
	If numBytesReadPtr is not NULL, stores the number of bytes read in
	*numBytesReadPtr.
	
	Returns 0 if OK or an error code.
	
	If bytes remain to be read in the file and you ask to read more bytes
	than remain, the remaining bytes are returned and the function result is
	CP_FILE_EOF_ERROR.
	
	CPReadFile2 is appropriate when you are reading a record of fixed size, in
	which case you want to consider it an error if the end of file is reached
	before reading all of the bytes in the record. If you are reading a record
	of variable size then you should use CPReadFile instead of CPReadFile2.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPReadFile2(CP_FILE_REF fileRef, unsigned long count, void* buffer, unsigned long* numBytesReadPtr)
{
	unsigned long numBytesRead;
	
	if (count == 0) {
		if (numBytesReadPtr != NULL)
			*numBytesReadPtr = 0;
		return 0;
	}
	
	clearerr(fileRef);
	numBytesRead = (unsigned long) fread(buffer, 1, count, fileRef);
	if (numBytesReadPtr != NULL)
		*numBytesReadPtr = numBytesRead;
	if (ferror(fileRef))
		return CP_FILE_READ_ERROR;
	if (numBytesRead < count) {					// We did not read all of the bytes requested.
		if (CPAtEndOfFile(fileRef))
			return CP_FILE_EOF_ERROR;			// We hit the end of file.
		return CP_FILE_READ_ERROR;				// Some other occurred but ferror did not reflect it.
	}
	return 0;
}

/*	CPWriteFile(fileRef, count, buffer, numBytesWrittenPtr)

	Writes count bytes from the buffer to the referenced file.
	
	If numBytesWrittenPtr is not NULL, stores the number of bytes written in
	*numBytesWrittenPtr.
	
	Returns 0 if OK or an error code.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPWriteFile(CP_FILE_REF fileRef, unsigned long count, const void* buffer, unsigned long* numBytesWrittenPtr)
{
	unsigned long numBytesWritten;
	
	if (count == 0) {
		if (numBytesWrittenPtr != NULL)
			*numBytesWrittenPtr = 0;
		return 0;
	}
	
	numBytesWritten = (unsigned long) fwrite(buffer, 1, count, fileRef);
	if (numBytesWrittenPtr != NULL)
		*numBytesWrittenPtr = numBytesWritten;
	if (numBytesWritten != count)
		return CP_FILE_WRITE_ERROR;
	return 0;
}

/*	CPGetFilePosition(fileRef, filePosPtr)

	Returns via filePosPtr the current file position of the referenced file.
	
	Returns 0 if OK or an error code.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPGetFilePosition(CP_FILE_REF fileRef, unsigned long* filePosPtr)
{
	long pos;
	
	pos = ftell(fileRef);
	if (pos == -1L)
		return CP_FILE_POS_ERROR;
	*filePosPtr = pos;
	return 0;
}

/*	CPSetFilePosition(fileRef, filePos, mode)

	Sets the current file position in the referenced file.
	
	If mode is -1, then filePos is relative to the start of the file.
	If mode is 0, then filePos is relative to the current file position.
	If mode is 1, then filePos is relative to the end of the file.
	
	Returns 0 if OK or an error code.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPSetFilePosition(CP_FILE_REF fileRef, long filePos, int mode)
{
	int seekMode;
	
	switch(mode) {
		case -1:
			seekMode = SEEK_SET;
			break;
		case 0:
			seekMode = SEEK_CUR;
			break;
		case 1:
			seekMode = SEEK_END;
			break;
		default:
			return CP_FILE_POS_ERROR;
	}
	
	if (fseek(fileRef, filePos, seekMode) != 0)
		return CP_FILE_POS_ERROR;
	return 0;
}

/*	CPAtEndOfFile(fileRef)

	Returns 1 if the current file position is at the end of file, 0 if not.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPAtEndOfFile(CP_FILE_REF fileRef)
{
	if (feof(fileRef))				// Hit end of file?
		return 1;
	return 0;
}

/*	CPNumberOfBytesInFile(fileRef, numBytesPtr)

	Returns via numBytesPtr the total number of bytes in the referenced file.
	
	Returns 0 if OK or an error code.
	
	Added for Igor Pro 3.13 but works with any version. However, some error
	codes returned require Igor Pro 3.13 or later, so you will get bogus error
	messages if you return these error codes to earlier versions of Igor.
*/
int
CPNumberOfBytesInFile(CP_FILE_REF fileRef, unsigned long* numBytesPtr)
{
	long originalPos;

	originalPos = ftell(fileRef);
	if (fseek(fileRef, 0, SEEK_END) != 0)
		return CP_FILE_POS_ERROR;
	*numBytesPtr = ftell(fileRef);
	if (*numBytesPtr == -1L)
		return CP_FILE_POS_ERROR;
	if (fseek(fileRef, originalPos, SEEK_SET) != 0)
		return CP_FILE_POS_ERROR;
	return 0;
}

static void
ReorderBytes(void *p, int bytesPerPoint, long numValues)	// Reverses byte order.
{
	unsigned char ch, *p1, *p2, *pEnd;
	
	pEnd = (unsigned char *)p + numValues*bytesPerPoint;
	while (p < (void *)pEnd) {
		p1 = (unsigned char *)p;
		p2 = (unsigned char *)p + bytesPerPoint-1;
		while (p1 < p2) {
			ch = *p1;
			*p1++ = *p2;
			*p2-- = ch;
		}
		p = (unsigned char *)p + bytesPerPoint;
	}
}

static void
ReorderShort(void* sp)
{
	ReorderBytes(sp, 2, 1);
}

static void
ReorderLong(void* lp)
{
	ReorderBytes(lp, 4, 1);
}

static void
ReorderDouble(void* dp)
{
	ReorderBytes(dp, 8, 1);
}

static void
ReorderBinHeader1(BinHeader1* p)
{
	ReorderShort(&p->version);
	ReorderLong(&p->wfmSize);
	ReorderShort(&p->checksum);
}

static void
ReorderBinHeader2(BinHeader2* p)
{
	ReorderShort(&p->version);
	ReorderLong(&p->wfmSize);
	ReorderLong(&p->noteSize);
	ReorderLong(&p->pictSize);
	ReorderShort(&p->checksum);
}

static void
ReorderBinHeader3(BinHeader3* p)
{
	ReorderShort(&p->version);
	ReorderLong(&p->wfmSize);
	ReorderLong(&p->noteSize);
	ReorderLong(&p->formulaSize);
	ReorderLong(&p->pictSize);
	ReorderShort(&p->checksum);
}

static void
ReorderBinHeader5(BinHeader5* p)
{
	ReorderShort(&p->version);
	ReorderShort(&p->checksum);
	ReorderLong(&p->wfmSize);
	ReorderLong(&p->formulaSize);
	ReorderLong(&p->noteSize);
	ReorderLong(&p->dataEUnitsSize);
	ReorderBytes(&p->dimEUnitsSize, 4, 4);
	ReorderBytes(&p->dimLabelsSize, 4, 4);
	ReorderLong(&p->sIndicesSize);
	ReorderLong(&p->optionsSize1);
	ReorderLong(&p->optionsSize2);
}

static void
ReorderWaveHeader2(WaveHeader2* p)
{
	ReorderShort(&p->type);
	ReorderLong(&p->next);
	// char bname does not need to be reordered.
	ReorderShort(&p->whVersion);
	ReorderShort(&p->srcFldr);
	ReorderLong(&p->fileName);
	// char dataUnits does not need to be reordered.
	// char xUnits does not need to be reordered.
	ReorderLong(&p->npnts);
	ReorderShort(&p->aModified);
	ReorderDouble(&p->hsA);
	ReorderDouble(&p->hsB);
	ReorderShort(&p->wModified);
	ReorderShort(&p->swModified);
	ReorderShort(&p->fsValid);
	ReorderDouble(&p->topFullScale);
	ReorderDouble(&p->botFullScale);
	// char useBits does not need to be reordered.
	// char kindBits does not need to be reordered.
	ReorderLong(&p->formula);
	ReorderLong(&p->depID);
	ReorderLong(&p->creationDate);
	// char wUnused does not need to be reordered.
	ReorderLong(&p->modDate);
	ReorderLong(&p->waveNoteH);
	// The wData field marks the start of the wave data which will be reordered separately.
}

static void
ReorderWaveHeader5(WaveHeader5* p)
{
	ReorderLong(&p->next);
	ReorderLong(&p->creationDate);
	ReorderLong(&p->modDate);
	ReorderLong(&p->npnts);
	ReorderShort(&p->type);
	ReorderShort(&p->dLock);
	// char whpad1 does not need to be reordered.
	ReorderShort(&p->whVersion);
	// char bname does not need to be reordered.
	ReorderLong(&p->whpad2);
	ReorderLong(&p->dFolder);
	ReorderBytes(&p->nDim, 4, 4);
	ReorderBytes(&p->sfA, 8, 4);
	ReorderBytes(&p->sfB, 8, 4);
	// char dataUnits does not need to be reordered.
	// char dimUnits does not need to be reordered.
	ReorderShort(&p->fsValid);
	ReorderShort(&p->whpad3);
	ReorderDouble(&p->topFullScale);
	ReorderDouble(&p->botFullScale);
	ReorderLong(&p->dataEUnits);
	ReorderBytes(&p->dimEUnits, 4, 4);
	ReorderBytes(&p->dimLabels, 4, 4);
	ReorderLong(&p->waveNoteH);
	ReorderBytes(&p->whUnused, 4, 16);
	ReorderShort(&p->aModified);
	ReorderShort(&p->wModified);
	ReorderShort(&p->swModified);
	// char useBits does not need to be reordered.
	// char kindBits does not need to be reordered.
	ReorderLong(&p->formula);
	ReorderLong(&p->depID);
	ReorderShort(&p->whpad4);
	ReorderShort(&p->srcFldr);
	ReorderLong(&p->fileName);
	ReorderLong(&p->sIndices);
	// The wData field marks the start of the wave data which will be reordered separately.
}

static int
Checksum(short *data, int needToReorderBytes, int oldcksum, int numbytes)
{
	unsigned short s;
	
	numbytes >>= 1;				// 2 bytes to a short -- ignore trailing odd byte.
	while(numbytes-- > 0) {
		s = *data++;
		if (needToReorderBytes)
			ReorderShort(&s);
		oldcksum += s;
	}
	return oldcksum&0xffff;
}

/*	NumBytesPerPoint(int type)
	
	Given a numeric wave type, returns the number of data bytes per point.
*/
static int
NumBytesPerPoint(int type)
{
	int numBytesPerPoint;
	
	// Consider the number type, not including the complex bit or the unsigned bit.
	switch(type & ~(NT_CMPLX | NT_UNSIGNED)) {
		case NT_I8:
			numBytesPerPoint = 1;		// char
			break;
		case NT_I16:
			numBytesPerPoint = 2;		// short
			break;
		case NT_I32:
			numBytesPerPoint = 4;		// long
			break;
		case NT_FP32:
			numBytesPerPoint = 4;		// float
			break;
		case NT_FP64:
			numBytesPerPoint = 8;		// double
			break;
		default:
			return 0;
			break;
	}

	if (type & NT_CMPLX)
		numBytesPerPoint *= 2;			// Complex wave - twice as many points.
	
	return numBytesPerPoint;
}

/*	LoadNumericWaveData(fr, type, npnts, waveDataSize, needToReorderBytes, pp)

	fr is a file reference.
	type is the Igor number type.
	npnts is the total number of elements in all dimensions.
	waveDataSize is the number of data bytes stored in the file.
	needToReorderBytes if the byte ordering of the file is not the byte ordering of the current platform.
	pp is a pointer to a pointer.
	
	If an error occurs, LoadWaveData returns a non-zero error code and sets
	*pp to NULL.
	
	If no error occurs, LoadWaveData returns 0 and sets *pp to a pointer allocated
	via malloc. This pointer must be freed by the calling routine.
*/
static int
LoadNumericWaveData(CP_FILE_REF fr, int type, long npnts, unsigned long waveDataSize, int needToReorderBytes, void**pp)
{
	int numBytesPerPoint;
	unsigned long numBytesToRead, numBytesToAllocate;
	unsigned long numBytesRead;
	void* p;
	int err;
	
	*pp = NULL;							// Assume that we can not allocate memory.

	numBytesPerPoint = NumBytesPerPoint(type);
	if (numBytesPerPoint <= 0) {
		printf("Invalid wave type (0x%x).\n", type);
		return -1;
	}
	numBytesToRead = npnts * numBytesPerPoint;

	numBytesToAllocate = numBytesToRead;
	if (numBytesToAllocate == 0)
		numBytesToAllocate = 8;			// This is just because malloc refuses to allocate a zero byte block.
	p = malloc(numBytesToAllocate);		// Allocate memory to store the wave data.
	if (p == NULL) {
		printf("Unable to allocate %ld bytes to store data.\n", numBytesToAllocate);
		return -1;
	}
	if (numBytesToRead > 0) {
		if (waveDataSize < numBytesToRead) {
			/*	If here, this should be a wave governed by a dependency formula
				for which no wave data was written to the file. Since we can't
				execute the dependency formula we have no way to recreate the wave's
				data. Therefore, we return 0 for all points in the wave.
			*/
			memset(p, 0, numBytesToRead);
		}
		else {
			if (err = CPReadFile(fr, numBytesToRead, p, &numBytesRead)) {
				free(p);
				printf("Error %d occurred while reading the wave data.\n", err);
				return err;
			}
			if (needToReorderBytes) {
				if (type != 0)				// Text wave data does not need to be reordered.
					ReorderBytes(p, numBytesPerPoint, numBytesToRead/numBytesPerPoint);
			}
		}
	}
	
	*pp = p;							// Return the pointer to the calling routine.
	return 0;
}

/*	ReadWave(fr, typePtr, npntsPtr, waveDataPtrPtr)

	Reads the wave file and prints some information about it.
	
	Returns to the calling routine the wave's type, number of points, and the
	wave data. The calling routine must free *waveDataPtrPtr if it is
	not null.
	
	Returns 0 or an error code.
	
	This routine is written such that it could be used to read waves
	from an Igor packed experiment file as well as from a standalone
	Igor binary wave file. In order to achieve this, we must not assume
	that the wave is at the start of the file. We do assume that, on entry
	to this routine, the file position is at the start of the wave.
*/
//static
int
ReadIBW(CP_FILE_REF fr, int* typePtr, long* npntsPtr, void** waveDataPtrPtr)
{
	unsigned long startFilePos;
	short version;
	short check;
	int binHeaderSize, waveHeaderSize, checkSumSize;
	unsigned long waveDataSize;
	unsigned long numBytesRead;
	int needToReorderBytes;
	char buffer[512];
	unsigned long modDate;
	long wfmSize;
	long npnts;
	int type;
	char name[64];
	int err;
	
	*waveDataPtrPtr = NULL;
	*typePtr = 0;
	*npntsPtr = 0;
	
	if (err = CPGetFilePosition(fr, &startFilePos)){
//		MessageBox(NULL, "Error", "Error get file poos", MB_OK);
		return err;
	}
	
	// Read the file version field.
	if (err = CPReadFile(fr, 2, &version, &numBytesRead)) {
	//	printf("Error %d occurred while reading the file version.\n", err);
		MessageBox(NULL, "Error", "Error occurred while reading the file version.", MB_OK);
		return err;
	}
	
	/*	Reorder version field bytes if necessary.
		If the low order byte of the version field of the BinHeader structure
		is zero then the file is from a platform that uses different byte-ordering
		and therefore all data will need to be reordered.
	*/
//	ShowFloat(version, "version");

	needToReorderBytes = (version & 0xFF) == 0;
	if (needToReorderBytes){
//		MessageBox(NULL, "reorder bytes.", "reorder bytes", MB_OK);
		ReorderShort(&version);

	}

	ShowFloat(version, "version");
		
	// Check the version.
	switch(version) {
		case 1:
			printf("This is a version 1 file.\n");
			binHeaderSize = sizeof(BinHeader1);
			waveHeaderSize = sizeof(WaveHeader2);
			checkSumSize = binHeaderSize + waveHeaderSize;
			break;
		case 2:
			printf("This is a version 2 file.\n");
			binHeaderSize = sizeof(BinHeader2);
			waveHeaderSize = sizeof(WaveHeader2);
			checkSumSize = binHeaderSize + waveHeaderSize;
			break;
		case 3:
			printf("This is a version 3 file.\n");
			binHeaderSize = sizeof(BinHeader3);
			waveHeaderSize = sizeof(WaveHeader2);
			checkSumSize = binHeaderSize + waveHeaderSize;
			break;
		case 5:
			printf("This is a version 5 file.\n");
			binHeaderSize = sizeof(BinHeader5);
			waveHeaderSize = sizeof(WaveHeader5);
			checkSumSize = binHeaderSize + waveHeaderSize - 4;	// Version 5 checksum does not include the wData field.
			break;
		default:
			MessageBox(NULL, "This does not appear to be a valid Igor binary wave file.", "Error", MB_OK);
			printf("This does not appear to be a valid Igor binary wave file. The version field = %d.\n", version);
			return -1;
			break;	
	}
	
	// Load the BinHeader and the WaveHeader into memory.
	CPSetFilePosition(fr, startFilePos, -1);
	if (err = CPReadFile(fr, binHeaderSize+waveHeaderSize, buffer, &numBytesRead)) {
		printf("Error %d occurred while reading the file headers.\n", err);
		return err;
	}
	
	// Check the checksum.	
	check = Checksum((short*)buffer, needToReorderBytes, 0, checkSumSize);
	if (check != 0) {
		printf("Error in checksum - should be 0, is %d.\n", check);
		printf("This does not appear to be a valid Igor binary wave file.\n");
		return -1;
	}
	
	// Do byte reordering if the file is from another platform.	
	if (needToReorderBytes) {
		switch(version) {
			case 1:
				ReorderBinHeader1((BinHeader1*)buffer);
				break;
			case 2:
				ReorderBinHeader2((BinHeader2*)buffer);
				break;
			case 3:
				ReorderBinHeader3((BinHeader3*)buffer);
				break;
			case 5:
				ReorderBinHeader5((BinHeader5*)buffer);
				break;
		}
		switch(version) {
			case 1:				// Version 1 and 2 files use WaveHeader2.
			case 2:
			case 3:
				ReorderWaveHeader2((WaveHeader2*)(buffer+binHeaderSize));
				break;
			case 5:
				ReorderWaveHeader5((WaveHeader5*)(buffer+binHeaderSize));
				break;
		}
	}
	
	// Read some of the BinHeader fields.
	switch(version) {
		case 1:
			{
				BinHeader1* b1;
				b1 = (BinHeader1*)buffer;
				wfmSize = b1->wfmSize;
			}
			break;
			
		case 2:
			{
				BinHeader2* b2;
				b2 = (BinHeader2*)buffer;
				wfmSize = b2->wfmSize;
			}
			break;
			
		case 3:
			{
				BinHeader3* b3;
				b3 = (BinHeader3*)buffer;
				wfmSize = b3->wfmSize;
			}
			break;
			
		case 5:
			{
				BinHeader5* b5;
				b5 = (BinHeader5*)buffer;
				wfmSize = b5->wfmSize;
			}
			break;
	}
	
	// Read some of the WaveHeader fields.
	switch(version) {
		case 1:
		case 2:
		case 3:
			{
				WaveHeader2* w2;
				w2 = (WaveHeader2*)(buffer+binHeaderSize);
				modDate = w2->modDate;
				npnts = w2->npnts;
				type = w2->type;
				strcpy(name, w2->bname);
			}
			break;
			
		case 5:
			{
				WaveHeader5* w5;
				w5 = (WaveHeader5*)(buffer+binHeaderSize);
				modDate = w5->modDate;
				npnts = w5->npnts;
				type = w5->type;
				strcpy(name, w5->bname);
			}
			break;
	}
	printf("Wave name=%s, npnts=%ld, type=0x%x, wfmSize=%ld.\n", name, npnts, type, wfmSize);
	
	// Return information to the calling routine.
	*typePtr = type;
	*npntsPtr = npnts;
	
	// Determine the number of bytes of wave data in the file.
	switch(version) {
		case 1:
		case 2:
		case 3:
			waveDataSize = wfmSize - offsetof(WaveHeader2, wData) - 16;
			break;
		case 5:
			waveDataSize = wfmSize - offsetof(WaveHeader5, wData);
			break;
	}
	
	// Position the file pointer to the start of the wData field.
	switch(version) {
		case 1:
		case 2:
		case 3:
			CPSetFilePosition(fr, startFilePos+binHeaderSize+waveHeaderSize-16, -1);	// 16 = size of wData field in WaveHeader2 structure.
			break;
		case 5:
			CPSetFilePosition(fr, startFilePos+binHeaderSize+waveHeaderSize-4, -1);		// 4 = size of wData field in WaveHeader2 structure.
			break;
	}
	
	if (type == 0) {
		// For simplicity, we don't load text wave data in this example program.
		printf("This is a text wave.\n");
		return 0;
	}

	// Load the data and allocates memory to store it.
	if (err = LoadNumericWaveData(fr, type, npnts, waveDataSize, needToReorderBytes, waveDataPtrPtr))
		return err;
	
	return 0;
}


//int DoReadTest(const char* filePath);
int
DoReadTest(const char* filePath)
{
	CP_FILE_REF fr;
	int type;
	long npnts;
	void* waveDataPtr;
	int err;
	
	if (err = CPOpenFile(filePath, 0, &fr)) {
		printf("Error %d occurred while opening the file.\n", err);
		return err;
	}
	
	err = ReadIBW(fr, &type, &npnts, &waveDataPtr);
	
	// Here you would do something with the data.
	
	CPCloseFile(fr);
	
	if (waveDataPtr != NULL)
		free(waveDataPtr);
	
	printf("End of read test.\n");
	
	return err;
}
