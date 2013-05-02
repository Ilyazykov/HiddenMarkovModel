#include <iostream>
#include <string>
#include <fstream>
#include <string>

#include "HMM.h"
#include "TestSequence.h"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

namespace boostUblas = boost::numeric::ublas;
using namespace std;

void main()
{
	TestSequence testSequence("hmmdata");
	HMM test("test.txt");

	boostUblas::vector<string> viterbiState = test.viterbi(testSequence.observations);
	boostUblas::vector<string> fbState = test.forwardBackward(testSequence.observations);

	qualityAssessment resViterbi, resFB;

	resViterbi = testSequence.getQualityAssessment(viterbiState, "St1");
	resFB = testSequence.getQualityAssessment(fbState, "St1");

	//////////////////////////
	ofstream outputStream("resultData");

	outputStream << "¹\tObs\tState\tViterbi\tF/B\n";

	for (int i=0; i<testSequence.observations.size(); i++) {
		outputStream << i << '\t' << testSequence.observations(i) << '\t'
			<< testSequence.states(i) << "\t\t" << viterbiState(i) << "\t\t" << fbState(i) << endl;
	}

	outputStream << endl << "Viterbi" << endl
		<< "TP ->" << resViterbi.truePositives << endl
		<< "TN ->" << resViterbi.trueNegatives << endl
		<< "FP ->" << resViterbi.falsePositives << endl
		<< "FN ->" << resViterbi.falseNegatives << endl
		<< "F ->" << resViterbi.fMera << endl;

	outputStream << endl << "Forward/Backward" << endl
		<< "TP ->" << resFB.truePositives << endl
		<< "TN ->" << resFB.trueNegatives << endl
		<< "FP ->" << resFB.falsePositives << endl
		<< "FN ->" << resFB.falseNegatives << endl
		<< "F ->" << resFB.fMera << endl;

	outputStream.close();

	cout << "See 'resultData' file for result";
};