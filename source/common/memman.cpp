
#include "stdafx.h"
#include "memman.h"
#include "../common/utils.h"

#define MINNENTRIESINBLOCK 100
#define WASTEALLOWED 4

#define SHOWALLOC FALSE
#define SHOWDEALLOC FALSE
#define DEBUGSHOW FALSE

MemMan::MemMan(){
	callcount=0;
	NBlocks=0;
	MakeNewBlock(MINBLOCKSIZE);
	NEntries[0]=0;
}

MemMan::~MemMan(){
	UINT i;
	for (i=0; i<NBlocks; i++){
		delete [] BlockAddresses[i];
		delete [] Starts[i];
		delete [] Size[i];
	}
}

__int16 * MemMan::AllocInt16(int n){
	if (n<=0) return NULL;
	if (USEMYMEM){
		return (__int16 *)(New(sizeof(__int16)*n));
	}
	else{
		BYTE * pb = new BYTE[n*sizeof(__int16)];
		return (__int16 *)(pb);
	}
}

int * MemMan::AllocInt(int n){
	if (n<=0) return NULL;
	if (USEMYMEM){
		return (int *)(New(sizeof(int)*n));
	}
	else{
		BYTE * pb = new BYTE[n*sizeof(int)];
		return (int *)(pb);
//		return new int[n];
	}
}
__int32 * MemMan::AllocInt32(int n){
	if (n<=0) return NULL;
	if (USEMYMEM){
		return (__int32 *)(New(sizeof(__int32)*n));
	}
	else{
		BYTE * pb = new BYTE[n*sizeof(__int32)];
		return (__int32 *)(pb);
//		return new int[n];
	}
}

double * MemMan::AllocDouble(int n){
	if (n<=0) return NULL;
	if (USEMYMEM){
		return (double *)(New(sizeof(double)*n));
	}
	else{
		BYTE * pb = new BYTE[n*sizeof(double)];
		return (double *)(pb);
//		return new double[n];
	}
}
BOOL * MemMan::AllocBOOL(int n){
	if (n<=0) return NULL;
	if (USEMYMEM){
		return (BOOL *)(New(sizeof(BOOL)*n));
	}
	else{
		BYTE * pb = new BYTE[n*sizeof(BOOL)];
		return (BOOL *)(pb);
//		return new double[n];
	}
}


BYTE * MemMan::MakeNewBlock(UINT size){
	if (NBlocks>=MAXNUMBERBLOCKS){
		Alert0("no more free blocks in MemMan::New");
		return NULL;
	}

//	if (callcount!=0){
//		Alert0("CALLCOUNT!=0");
//	}
//	callcount++;
	UINT blocksize=size;

	if (blocksize<MINBLOCKSIZE){
		blocksize=MINBLOCKSIZE;
	}
	if (NBlocks>0){
		if (blocksize<BlockSize[NBlocks-1]){
			blocksize=BlockSize[NBlocks-1]+2;
		}
	}

	BlockSize[NBlocks]=blocksize;
	
	BlockAddresses[NBlocks]=new BYTE[blocksize];
	Starts[NBlocks] = new UINT [MINNENTRIESINBLOCK];
	Size[NBlocks] = new UINT[MINNENTRIESINBLOCK];
	MaxEntries[NBlocks]=MINNENTRIESINBLOCK;

	NEntries[NBlocks]=1;
	Starts[NBlocks][0]=0;
	Size[NBlocks][0]=size;

	NBlocks++;

//	callcount--;
	return BlockAddresses[NBlocks-1] + Starts[NBlocks-1][0];

	/*
	char s[20];
	CString c=CString("created new block; Now  ");
	_itoa(NBlocks, s, 10);
	c += CString(s)+CString(" blocks. Last one is ");
	_itoa(size, s, 10);
	c += CString(s)+CString(" bytes large ");
	Alert0(c);
*/

}


BYTE * MemMan::DoInsert(UINT size, UINT block, int entry){
//	if (callcount!=0){
//		Alert0("CALLCOUNT!=0");
//	}
//	callcount++;
	if (entry==(-1)){ // insert as first entry in a block
		NEntries[block]=1;
		Size[block][0]=size;
		Starts[block][0]=0;
		if (SHOWALLOC){
			char s[20];
			CString c=CString("inserted as first entry in block ");
			_itoa(block, s, 10);
			c += CString(s);
			Alert0(c);
		}

//		callcount--;
		return BlockAddresses[block];
	}
	
	UINT * tstart;
	UINT * tsize;
	UINT i;
	if (NEntries[block]==MaxEntries[block]){

		UINT NE=2*MaxEntries[block];
		tstart = new UINT[NE];
		tsize = new UINT[NE];
		MaxEntries[block]=NE;

		for (i=0; i<NEntries[block]; i++){
			tstart[i]=Starts[block][i];
			tsize[i]=Size[block][i];
		}
		delete [] Starts[block];
		Starts[block]=tstart;

		delete [] Size[block];
		Size[block]=tsize;
	}

	tstart = Starts[block];
	tsize = Size[block];

	if (entry==(-2)){  // insert at position 0 but there are already entries
		for (i=NEntries[block]; i>0; i--){
			tstart[i]=tstart[i-1];
			tsize[i]=tsize[i-1];
		}
		tstart[0]=0;
		tsize[0]=size;
		NEntries[block]++;
		return BlockAddresses[block];
	}


	for (int i=NEntries[block]; i>(entry+1); i--){
		tstart[i]=tstart[i-1];
		tsize[i]=tsize[i-1];
	}

	UINT i0=tstart[entry]+tsize[entry];
	if ((i0 % 2)!=0){
		i0++;
	}

	tstart[entry+1]=i0;
	tsize[entry+1]=size;

	if (SHOWALLOC){
		char s[20];
		CString c=CString("inserted after entry # ");
		_itoa(entry, s, 10);
		c += CString(s)+CString(" at position ");
		_itoa(i0, s, 10);
		c += CString(s)+CString(" with size  ");
		_itoa(size, s, 10);
		c += CString(s)+CString(" .");

		Alert0(c);
	}

	NEntries[block]++;

//	callcount--;
	return BlockAddresses[block]+tstart[entry+1];
}


BYTE * MemMan::New(UINT size){
	if (size==0) return NULL;
	if (!USEMYMEM){
		return new BYTE[size];
	}
//	if (callcount!=0){
//		Alert0("CALLCOUNT!=0");
//	}
//	callcount++;
	char s[20];

	CString c;

	if (SHOWALLOC){
		CString c=CString("enter new with ");
		_itoa(size, s, 10);
		c += CString(s);

		Alert0(c);
	}

	int minblock=0;
	while (1){
		if (minblock==NBlocks){
//			callcount--;
			return MakeNewBlock(size);
		}
		if (size<=BlockSize[minblock]){
			break;
		}
		minblock++;
	}


	int block=minblock;
	int minsize=BlockSize[NBlocks-1]+2;

	if (DEBUGSHOW){
		_itoa(size,s,10);
		CString c=CString("size= ")+CString(s);

		_itoa(minblock,s,10);

		c += CString("; minblock")+CString(s);

		_itoa(NBlocks,s,10);
		c += CString("; NBlocks")+CString(s);
		_itoa(minsize,s,10);
		c += CString("minsize initial:")+CString(s);
		Alert0(c);
	}

	int mb=0;
	int me=0;

	int i0, i1;
	int e=0;
	int ee;
	
//	if ((size==200000) | 1){
//			i0=Starts[0][0]+Size[0][0];
//			ShowFloat(i0, "i0");
//	}
	
	for (UINT block=minblock; block<NBlocks; block++){
		int NE=NEntries[block];
		UINT * tstart = Starts[block];
		UINT * tsize = Size[block];

		e=0;
		BOOL notyettried0=true;
		while (true){
			ee=e;
			if (NE==0){
				i0=0;
				i1=BlockSize[block];
				ee=-1;
			}
			else{
				if (NE==e){
					break;
				}
				else{
					if ((e==0) & (tstart[e]!=0) & notyettried0){
						i0=0;
						i1=tstart[e];
						ee=-2;
						e=-1;
						notyettried0=false;
					}
					else{
						i0=tstart[e]+tsize[e];
						if ((i0 % 2)!=0){
							i0++;
						}
						if (e==(NE-1)){
							i1=BlockSize[block];
						}
						else{
							i1=tstart[e+1];
						}
					}
				}
			}
			int trysize=i1-i0;
			if (trysize>=int(size)){
				if ((trysize-size)<WASTEALLOWED){
//					Alert0("<WASTEALLOWED");
//					callcount--;
					return DoInsert(size, block, ee);
				}
				if (trysize<minsize){
					minsize=trysize;
					mb=block;
					me=ee;
				}
			}
			if (ee==(-1)) break;
			e++;
		}
	}
	if (minsize==(BlockSize[NBlocks-1]+2)){
		if (DEBUGSHOW) Alert0("RETURN MAKENEW BLOCK");
//		callcount--;
		return MakeNewBlock(size);
	}


	if (DEBUGSHOW){
		_itoa(mb, s, 10);
		c = CString("RETURN DOINSERT with mb = ") +CString(s);
		_itoa(me, s, 10);
		c+=CString(" me = ")+CString(s);
		Alert0(c);
	}

//	callcount--;
	return DoInsert(size, mb, me);

}

BOOL MemMan::Delete(BYTE * p){
	if (!USEMYMEM){
		delete [] p;
		return true;
	}
//	if (callcount!=0){
//		Alert0("CALLCOUNT!=0");
//	}
//	callcount++;
	int block=0;
//	int entry;


	char s[20];
	if (SHOWDEALLOC){
		CString c=CString("enter dealloc:  ");
		_itoa(NBlocks, s, 10);
		c += CString ("Nblocks =")+CString(s);
		_itoa(NEntries[NBlocks-1], s, 10);
		
		c += CString ("NEntries[NBlocks-1]=")+CString(s);
		Alert0(c);
	}

	for (UINT block=0; block<NBlocks; block++){
		if (NEntries[block]>0){
			for (UINT entry=0; entry<NEntries[block]; entry++){
			
				if ((BlockAddresses[block] + Starts[block][entry])==p){
			//		callcount--;
					DoDelete(block, entry);
					return true;
				}
			}
		}
	}

	Alert0("not found address in MemMan::Delete");
//	callcount--;
	return false;
}

void MemMan::DoDelete(int block, int entry){
//	if (callcount!=0){
//		Alert0("CALLCOUNT!=0");
//	}
//	callcount++;

	int NE=NEntries[block];
	int e;
	char s[20];
	if (SHOWDEALLOC){
		CString c=CString("Delete entry:  ");
		_itoa(block, s, 10);
		c += CString(" block: ")+CString(s);
		_itoa(entry, s, 10);
		c += CString("; entry: ")+CString(s);
		Alert0(c);
	}
	for (e=entry; e<(NE-1); e++){
		Starts[block][e]=Starts[block][e+1];
		Size[block][e]=Size[block][e+1];
	}
	NEntries[block] = NEntries[block] -1 ;
	//ShowFloat(NEntries[block], "NEntries[block] after delete");
	if (0){
			int i0=Starts[0][0]+Size[0][0];
			ShowFloat(i0, "i0");
	}

//	callcount--;
}

