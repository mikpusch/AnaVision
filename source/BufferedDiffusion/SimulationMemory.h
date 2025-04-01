class SimParams{
public:
	SimParams();
	~SimParams();

public:

	void Init();
	void Alloc();
	void DeAlloc();

	int m_NumberOfPoints;

	int m_MaxNumberOfCurves;

	int m_NumberOfCurves;



	double m_Hinitial;

	double m_Binitial;

	double * m_Radius;

	double ** m_H;

	double ** m_B;

	double m_dr, m_dt;

	double m_Norm;

};


class SimulationMemory{

public:

	SimulationMemory();
	~SimulationMemory();

	void Create(int maxnumSimulations);

	void AddSimulation(int NumberOfPoints, int MaxNumberOfCurves, double InitialpH, double InitialB, 
											double dr, double dt, double *Radius);

	void AddCurve(double *H, double *B);

	void GetCurve(int index, int curve, double *H, double *B);

	int GetNumberOfPoints(int index);

	int GetNumberOfCurves(int index);

	double GetRadius(int index, int J);

	void GetValues(int index, int curve, int j, double & H, double & B);

	double GetpHBulk(int index);

	double GetInitialFreeBuffer(int index);

	double Getdt(int index);

	double Getdr(int index);

	int GetMaxNumberOfSimulations();

	int GetNumberOfSimulations();

	void SetNorm(int index, double Norm);

	double GetNorm(int index);

	bool DoShow;

	bool SaveOnFile(CFile & f);

	bool LoadFromFile(CFile & f);

private:

	BOOL initialized;

	int m_ActualSimulation;

	int m_maxnumSimulations;

	int m_NumberOfSimulations;

	SimParams * Sims;
	

	void DestroyAll();

	int GetAbsoluteSimulationIndex(int relindex);

	bool DoRead(SimulationMemory & ss, CFile & f);


};
