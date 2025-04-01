
#define MAX_NVALUES_SERIES_SIMULATION 1000

class SeriesSimulation{

public:

	SeriesSimulation();
	~SeriesSimulation();

	int NValues;
	double dr[MAX_NVALUES_SERIES_SIMULATION];
	double dt[MAX_NVALUES_SERIES_SIMULATION];
	double norm[MAX_NVALUES_SERIES_SIMULATION];
	bool WriteOnFile(CFile &f);
	bool LoadFromFile(CFile &f);


	void MakeToString(CString & c);
	void AddString(CString & c);

	int GetMinIndex();

};
