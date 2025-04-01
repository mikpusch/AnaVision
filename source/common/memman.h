#define MINBLOCKSIZE 500000
#define MAXNUMBERBLOCKS 2000


class MemMan{
public:
	MemMan();
	~MemMan();

	BYTE * New(UINT size);
	BOOL Delete(BYTE * p);

	__int16 * AllocInt16(int n);
	int * AllocInt(int n);
	__int32 * AllocInt32(int n);
	double * AllocDouble(int n);
	BOOL * AllocBOOL(int n);

public:
	int callcount;
	UINT NBlocks;
	UINT BlockSize[MAXNUMBERBLOCKS];
	BYTE * BlockAddresses[MAXNUMBERBLOCKS];

	UINT * Starts[MAXNUMBERBLOCKS];
	UINT * Size[MAXNUMBERBLOCKS];
	UINT NEntries[MAXNUMBERBLOCKS];
	UINT MaxEntries[MAXNUMBERBLOCKS];

	BYTE * MakeNewBlock(UINT size);
	BYTE * DoInsert(UINT size, UINT block, int entry);
	void DoDelete(int block, int entry);


};

