#include "TestSequence.h"

TestSequence::TestSequence( std::string url )
{
	std::ifstream inputFile(url);

	std::string temp;
	inputFile >> temp >> temp >> temp;

	int num;
	while (!inputFile.eof()) {
		inputFile >> num >> temp >> temp;
	}
	num++;
	inputFile.close();



	states.resize(num);
	observations.resize(num);

	inputFile.open(url);
	inputFile >> temp >> temp >> temp;

	for (int i=0;i<num;i++) {
		inputFile >> temp >> states(i) >> observations(i);
	}

	inputFile.close();
}

TestSequence::~TestSequence( void )
{

}

qualityAssessment TestSequence::getQualityAssessment( vector<std::string> gettedState, std::string mainState )
{
	qualityAssessment res;

	res.truePositives = res.trueNegatives = res.falsePositives = res.falseNegatives = 0;

	for (int i=0; i<gettedState.size();i++) {
		if (gettedState(i) == states(i)) {
			if (gettedState(i) == mainState) {
				res.truePositives++;
			} else {
				res.trueNegatives++;
			}
		} else {
			if (gettedState(i) == mainState) {
				res.falsePositives++;
			} else {
				res.falseNegatives++;
			}
		}
	}

	double TP = (double)(res.truePositives);
	double FP = (double)(res.falsePositives);
	double FN = (double)(res.falseNegatives);

	double precision = TP / (TP + FP);
	double recall = TP / (TP + FN);
	res.fMera = 2 * (precision * recall) / (precision + recall);

	return res;
}
