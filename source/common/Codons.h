
// UCAG - 0123

class Codon {
public:
	Codon();
	~Codon();

	char c[3];

	char aa;

	double Usage; //

};


class GeneticCode {
public:

	GeneticCode();
	GeneticCode(CString ReadFileNameForGeneticCode); // format abc d  where abc is the codon and d the aa, at least 64 times
	~GeneticCode();

	Codon codons[64];

	double GetRelativeFrequency(int codonindex);
	double GetRelativeFrequency(char codon[3]);
	int GetCodonIndex(char codon[3]);

	static CString MakeString(char codon[3]);

private:
	Init();
};


