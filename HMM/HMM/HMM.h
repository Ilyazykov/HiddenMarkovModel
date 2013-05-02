#pragma once

#include <fstream>
#include <string>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

class HMM
{
	int numObservation;
	int numStates;

	vector<double> PI; //the initial probability for each state
	matrix<double> A; //K*K state transition matrix
	matrix<double> B; //K*N observation matrix

	vector<std::string> namesOfStates;

public:
	HMM(std::string url);
	void init(std::string url);
	~HMM();

	vector<std::string> viterbi(vector<char> seqOfObservation);
	vector<std::string> forwardBackward(vector<char> seqOfObservation);
	
	void print();

private:
	vector<int> viterbi(vector<int> seqOfObservationIndex);
	vector<int> forwardBackward(vector<int> seqOfObservationIndex);

	vector<int> getIndexFromEmission(vector<char> seqOfObservation);
	vector<std::string> getNameOfStateFromIndex(vector<int> seqOfStateIndex);
};
