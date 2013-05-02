#include "HMM.h"

HMM::HMM( std::string url )
{
	init(url);
}

void HMM::init( std::string url )
{
	std::ifstream inputFile(url);
	inputFile >> numStates >> numObservation;

	namesOfStates.resize(numStates);
	for (int i=0; i<numStates; i++) {
		inputFile >> namesOfStates(i);
	}

	PI.resize(numStates);
	for (int i=0; i<numStates; i++) {
		inputFile >> PI(i);
	}

	A.resize(numStates,numStates);
	for (int i=0; i<numStates; i++) {
		for (int j=0; j<numStates; j++) {
			inputFile >> A(i,j);
		}
	}

	B.resize(numStates,numObservation);
	for (int i=0; i<numStates; i++) {
		for (int j=0; j<numObservation; j++) {
			inputFile >> B(i,j);
		}
	}
	inputFile.close();
}

HMM::~HMM()
{
	PI.clear();
	A.clear();
	B.clear();
}

vector<int> HMM::viterbi ( vector<int> Y )
{
	int T = Y.size();
	vector<int> X(T);

	// step 1

	matrix<double> T1(numStates, T);
	matrix<int> T2(numStates, T);


	for (int i=0; i<numStates; i++) {
		T1(i,0) = PI(i) * B(i,Y(0));
		T2(i,0) = i;
	}

	// step 2

	for (int i=1; i<T; i++) {
		for (int j=0; j<numStates; j++) {
			double max = 0;
			int argmax = -1;

			for (int k=0; k<numStates; k++)	{
				double tmp = T1(k,i-1) * A(k,j) * B(j, Y(i));
				if (tmp >= max) {
					max = tmp;
					argmax = k;
				}
			}

			T1(j,i) = max;
			T2(j,i) = argmax;
		}
	}

	// step 3

	double max = 0;
	int zt = -1;
	for (int k=0; k<numStates; k++) {
		double tmp = T1(k,T-1);
		if (tmp >= max) {
			max = tmp;
			zt = k;
		}
	}

	//backtrace
	int index = zt;

	for (int i=T-1; i>=0; i--) {
		X(i) = index;
		index = T2(index, i);
	}

	return X;
}

vector<std::string> HMM::viterbi( vector<char> Y )
{
	vector<int> intY; 
	intY = getIndexFromEmission(Y);

	vector<int> X;
	X = viterbi(intY);

	vector<std::string>res;
	res = getNameOfStateFromIndex(X);

	return res;
}

vector<int> HMM::forwardBackward( vector<int> Y )
{
	int T = Y.size();
	vector<int> X(T);

	//Forward

	matrix<double> a(numStates, T);

	for (int i=0; i<numStates; i++) {
		a(i,0) = PI(i) * B(i,Y(0));
	}

	for (int i=1; i<T; i++) {
		for (int j=0; j<numStates; j++) {
			a(j,i) = 0;

			for (int k=0; k<numStates; k++) {
				a(j,i) += a(k,i-1) * A(k,j) * B(j, Y(i));
			}
		}
	}

	//Backward

	matrix<double> b(numStates, T);

	for (int i=0; i<numStates; i++) {
		b(i,T-1) = 1;
	}

	for (int i=T-2; i>=0; i--) {
		for (int j=0; j<numStates; j++) {
			b(j,i) = 0;

			for (int k=0; k<numStates; k++) {
				b(j, i) += b(k,i+1) * A(j,k) * B(k,Y(i+1));
			}
		}
	}

	// forward-backward

	matrix<double> c(numStates, T);

	for (int i=0; i<T; i++) {
		for (int j=0; j<numStates; j++) {
			c(j,i) = a(j,i)*b(j,i);
		}
	}

	// creating result

	for (int i=0; i<T; i++) {
		double max = 0;
		int index = -1;
		for (int j=0; j<numStates; j++) {
			if (c(j,i) >= max) {
				max = c(j,i);
				index = j;
			}
		}
		X(i) = index;
	}
	return X;
}

vector<std::string> HMM::forwardBackward( vector<char> Y )
{
	vector<int> intY; 
	intY = getIndexFromEmission(Y);

	vector<int> X;
	X = forwardBackward(intY);

	vector<std::string>res;
	res = getNameOfStateFromIndex(X);

	return res;
}

void HMM::print()
{
	std::cout << numStates << ' ' << numObservation;
	std::cout << std::endl;
	std::cout << std::endl;

	for (int i=0; i<numStates; i++) {
		std::cout << PI(i) << '\t';
	}
	std::cout << std::endl;

	std::cout << std::endl;
	for (int i=0; i<numStates; i++) {
		for (int j=0; j<numStates; j++) {
			std::cout << A(i,j) << '\t';
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	for (int i=0; i<numStates; i++) {
		for (int j=0; j<numObservation; j++) {
			std::cout << B(i,j) << '\t';
		}
		std::cout << std::endl;
	}
}

vector<int> HMM::getIndexFromEmission( vector<char> Y )
{
	int T = Y.size();
	vector<int> intY(T); 
	for (int i=0; i<T; i++)
	{
		intY(i) = Y(i) - 97;
	}

	return intY;
}

vector<std::string> HMM::getNameOfStateFromIndex( vector<int> X )
{
	int T = X.size();
	vector<std::string> res(T); 
	for (int i=0; i<T; i++)
	{
		res(i) = namesOfStates(X(i));
	}

	return res;
}
