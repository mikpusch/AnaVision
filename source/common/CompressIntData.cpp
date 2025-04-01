#include "stdafx.h"

#include "../common/CompressIntData.h"


uint64 CompressIntData::MinCounts[64] = {
	2, 3, 4, 5,6, 8,11,15,20,26,
		33,41,50,60,71,83,96,110,110,110,
		110,110,110,110,110,110,110,110,110,110,
		110,110,110,110,110,110,110,110,110,110,
		110,110,110,110,110,110,110,110,110,110,
		110,110,110,110,110,110,110,110,110,110,
		110,110,110,110
};


CompressIntData::CompressIntData(){
}
CompressIntData::~CompressIntData(){
}

BOOL GetBit(BYTE ib, uint16 inpos){
	uint8 u = ib;
	switch (inpos) {
		case 0	: return (ib & ((uint8) (1))); break;
		case 1	: return (ib & ((uint8) (2))); break;
		case 2	: return (ib & ((uint8) (4))); break;
		case 3	: return (ib & ((uint8) (8))); break;
		case 4	: return (ib & ((uint8) (16))); break;
		case 5	: return (ib & ((uint8) (32))); break;
		case 6	: return (ib & ((uint8) (64))); break;
		case 7	: return (ib & ((uint8) (128))); break;
		default	: return false;break;
	}


}
void WriteBit(BOOL bit, uint16 pos, BYTE & ob){
	uint8 u;
	if (bit){
		switch (pos) {
			case 0	: u = 1; break;
			case 1	: u = 2; break;
			case 2	: u = 4; break;
			case 3	: u = 8; break;
			case 4	: u = 16; break;
			case 5	: u = 32; break;
			case 6	: u = 64; break;
			case 7	: u = 128; break;
			default	: u = 1; break;
		}
		ob = ob | u;
	}
	else {
		switch (pos) {
			case 0	: u = 254; break;
			case 1	: u = 253; break;
			case 2	: u = 251; break;
			case 3	: u = 247; break;
			case 4	: u = 239; /*255-16;*/ break;
			case 5	: u = 223; /* 255-32; */ break;
			case 6	: u = 191; /* 255-64; */ break;
			case 7	: u = 127; /* 255-128; */ break;
			default	: u = 1; break;
		}
		ob = ob & u;
	}

}

void CopyBits(uint16 outpos, uint16 NBits, uint16 inpos, BYTE ib, BYTE ib1, BYTE & ob){
/*

  outpos is the start position to write (0, ..., 7)
  inpos  is the start position to read
  ib is the input byte
  if inpos+NBits> 8  the second in byte, ib1 is used
*/
	while (NBits){
		if (inpos>=8){
			inpos = 0;
			ib = ib1;
		}
		BOOL bit = GetBit(ib, inpos);
		WriteBit(bit, outpos, ob);
		NBits--;
		inpos++;
		outpos++;
	}
}

void PutInBitPos(uint64 & BitPos, BYTE * input, uint64 NBits, BYTE * output){
	uint64 OutByte = BitPos / 8;
	uint64 OutStartBits = BitPos % 8;

	uint64 InByte = 0;
	BYTE ob = output[OutByte];
	BYTE ib = input[InByte];
	BYTE ib1;

	uint64 inpos = 0;

	uint64 BitsToWrite = NBits;

	while (BitsToWrite){
		uint64 BitsThisByte = 8 - OutStartBits;
		if (BitsThisByte > BitsToWrite){
			BitsThisByte = BitsToWrite;
		}
		if ( (inpos + BitsThisByte)>8){
			ib1 = input[InByte+1];
		}

		CopyBits(uint16(OutStartBits), uint16(BitsThisByte), uint16(inpos), ib, ib1, ob);

		inpos += BitsThisByte;
		if (inpos>=8){
			inpos -= 8;
			InByte++;
			ib = input[InByte];
		}
		BitsToWrite -= BitsThisByte;
		if (BitsToWrite){
			OutByte++;
			ob = output[OutByte];
			OutStartBits = 0;
		}
	}

	BitPos += NBits;

}

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

BYTE * CompressIntData::Encode(uint64 NData, unsigned __int8 datasizeinbit, BYTE * data){
	if (NData==0) return NULL;
	if (NData==1){
		uint64 NByte = 8 + 1 + 8;
		uint64 NExtra = datasizeinbit;
		if ( (NExtra % 8) == 0){
			NByte += NExtra/8;
		}
		else{
			NByte += 1 + (NExtra/8);
		}

		BYTE * out = new BYTE[unsigned int (NByte)];

		uint64 BitPos = 0;

		PutInBitPos(BitPos, (BYTE * )&(NData), 64, out);
		PutInBitPos(BitPos, (BYTE * )&(datasizeinbit), 8, out);
		PutInBitPos(BitPos, (BYTE * )&(data[0]), datasizeinbit, out);
		return out;
		
	}

	uint64 NBits = 0;
	DoEncode(TRUE, NBits, NData, datasizeinbit, data, NULL);

	uint64 NBytes = NBits / 8;
	if (NBits % 8){
		NBytes ++;
	}

	BYTE * out = new BYTE[unsigned int (NBytes)];

	if (!out) return false;

	DoEncode(false, NBits, NData, datasizeinbit, data, out);

	return out;
}

void CompressIntData::DoEncode(BOOL Check, uint64 & NBits, uint64 NData,
							   unsigned __int8 datasizeinbit, BYTE * data,
							   BYTE * out){
	NBits = 64+8;
	uint64 BitPos = 0;

	if (!Check){
		PutInBitPos(BitPos, (BYTE * )&(NData), 64, out);
		PutInBitPos(BitPos, (BYTE * )&(datasizeinbit), 8, out);
	}

	uint64 idata = 0;
//	while (TRUE){
//		uint64 x0 = Get


}

BYTE * CompressIntData::Decode(BYTE * data){
	return NULL;
}

