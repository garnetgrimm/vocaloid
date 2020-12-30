#include "FormantIIRFilter.h"

FormantIIRFilter::FormantIIRFilter()
{
}

FormantIIRFilter::FormantIIRFilter(const FormantIIRFilter & filter)
{
	setCoeffs(filter.aCs, filter.bCs);
}

void FormantIIRFilter::setCoeffs(const double* aCofs, const  double* bCofs)
{
	//b coefficent zero is assumed to be zero
	memcpy(aCs, aCofs, sizeof(double) * COEFF_COUNT);
	memcpy(bCs, bCofs, sizeof(double) * COEFF_COUNT);
}

void FormantIIRFilter::generateCoeffs(double sampleRate, double cornerFreq, double bandwidth) noexcept
{
		cornerFreq /= sampleRate;
		bandwidth /= sampleRate;
		double R = 1 - 3 * bandwidth;
		double cos2pif = cos(2 * MathConstants<double>::pi*cornerFreq);
		double Knum = 1 - (2 * R*cos2pif) + R * R;
		double Kden = 2 - 2 * cos2pif;
		double K = Knum / Kden;

		aCs[0] = 1 - K;
		aCs[1] = 2 * (K - R)*cos2pif;
		aCs[2] = R*R - K;

		bCs[0] = 0;
		bCs[1] = 2 * R*cos2pif;
		bCs[2] = -R * R;
}

void FormantIIRFilter::processSamples(float* inputData, int inputLen)
{
	try {

		//create outputData array
		float* outputData = new float[inputLen];

		//initalize first few outputs
		for (int i = 0; i < COEFF_COUNT; i++) {
			outputData[i] = 0;
		}

		//process rest of data
		for (size_t i = COEFF_COUNT; i < inputLen; i++) {
			double sum = 0;
			for (int j = 0; j < COEFF_COUNT; j++) {
				sum += aCs[j] * inputData[i - j];
				sum += bCs[j] * outputData[i - j];
			}
			outputData[i] = (float)sum;
		}

		//copy all back to input
		memcpy(inputData, outputData, inputLen * sizeof(float));
		delete[] outputData;
	} catch (int e) {
		cout << "ERROR: " << e << endl;
	}
}
