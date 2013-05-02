#pragma once

#include <fstream>
#include <string>

#include "HMM.h"

#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;

struct qualityAssessment {
	double truePositives;
	double falsePositives;
	double trueNegatives;
	double falseNegatives;
	double fMera;
};

class TestSequence
{
public:
	vector<char> observations;
	vector<std::string> states;

	TestSequence(std::string url);
	~TestSequence(void);

	qualityAssessment getQualityAssessment(vector<std::string> gettedState, std::string mainState);
};

