#include "stdafx.h"
#include "../common/codons.h"
#include "../common/utils.h"

Codon::Codon(){
	c[0]=c[1]=c[2]='U';
	aa='F';
	Usage = 0;
};
Codon::~Codon(){
}



GeneticCode::GeneticCode(CString ReadFileNameForGeneticCode){ // format abc d  where abc is the codon and d the aa, at least 64 times
	Init();
	CFile f;
	if (!f.Open(ReadFileNameForGeneticCode, CFile::modeRead)){
		Alert0(CString("The file for reading the code - ")+ReadFileNameForGeneticCode+CString(" - could not be opened"));
		return ;
	}
	CString c;
	int i=0;
	while (ReadLine(f, c)){
		i++;
//		Alert0(c);
		c.MakeUpper();
		c.TrimLeft();
		c.TrimRight();
		Codon x;
		if (c.GetLength()>=5){
			for (int i=0; i<3; i++){
				x.c[i]=c.GetAt(i);
			}
			int index = GetCodonIndex(x.c);
			if (index>=0){
				i=3;
				while (i<c.GetLength()){
//					ShowFloat(i, "i");
					char a=c.GetAt(i);
					char s[2];
					s[1]=0;
					s[0]=a;
					i++;
					if (a != ' '){
						codons[index].aa = a;
//						Alert0(CString(s));
						break;
					}
				}
			}
		}
	}
//	ShowFloat(i, "i");

	for (i=0; i<64; i++){
		CString c=this->MakeString(codons[i].c);
		char s[2];
		s[0]=codons[i].aa;
		s[1]=0;
		c += CString(" ")+CString(s);
//		Alert0(c);
	}

}


GeneticCode::GeneticCode(){
	Init();
}

GeneticCode::Init(){
	char c[4];
	c[0]='U';
	c[1]='C';
	c[2]='A';
	c[3]='G';
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			for (int k=0; k<4; k++){
				int l=k+4*(j+4*i);
				codons[l].c[0] = c[i]; 
				codons[l].c[1] = c[j]; 
				codons[l].c[2] = c[k];
			}
		}
	}
	for (int j=0; j<64; j++){
//		Alert0(MakeString(codons[j].c));
	}

}
GeneticCode::~GeneticCode(){
}

int GeneticCode::GetCodonIndex(char codon[3]){
	CString c0=MakeString(codon);
	for (int i=0; i<64; i++){
		if (c0.Compare(MakeString(codons[i].c))==0){
			return i;
		}
	}
	return -1;
}

double GeneticCode::GetRelativeFrequency(int codonindex){
	return 0.0;
}
double GeneticCode::GetRelativeFrequency(char codon[3]){
	return 0.0;
}

CString GeneticCode::MakeString(char c[3]){
	char s[20]; 
	s[0]=c[0];
	s[1]=c[1];
	s[2]=c[2];
	s[3]=0;
	CString str = CString(s);
	str.MakeUpper();
	return str;
}
