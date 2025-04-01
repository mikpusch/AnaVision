#pragma once

#define SS_MARKOV_MAX_STATES 50000

class SSMarkov
{
public:
	SSMarkov();
	~SSMarkov();
	void InitMarkov(int NStates);
	void SetRates(float ** rates);
	void ClearRates(float ** r);

	float ** R;
	double * pinf;


private:
	int NS;

	float ** AW;
	float ** IW;
	double * rs;
	void DeleteSSMarkov();


};

