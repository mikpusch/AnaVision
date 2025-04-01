/////////////////////////////////////////////////////
/*

  This class implements the compression of an array of integers

  input: array of integers of length A bits (assumed to be 8, 16, 32, or 64)

  the output array has the following format:

  64 bit	(as __int64) total number of data points
  8  bit	the size of the input data (8, 16, 32, 64, 128 = A

  Then repeatedly chunks of the following type
  A  bit	x0
  64 bit	N=Number of data points in the following chunk

  if N=1 the following slots are not used 
  8  bit	Number of bits used in the following differences = B
  B  bit	x[1]-x[0]
  B  bit	x[2]-x[1]
  .
  .
  .
  B  bit    X[N-1]-X[N-2]

*//////////////////////////////////////////////////

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8  uint8;

class CompressIntData{
public:
	CompressIntData();
	~CompressIntData();

	BYTE * Encode(uint64 NData, unsigned __int8 datasizeinbit, BYTE * data);

	BYTE * Decode(BYTE * data);

private:

	static uint64 MinCounts[64];
	void DoEncode(BOOL Check, uint64 & NBits, uint64 NData,
							   unsigned __int8 datasizeinbit, BYTE * data,
							   BYTE * out);



};