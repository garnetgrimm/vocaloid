#pragma once
#include <JuceHeader.h>

using namespace juce;
using namespace std;

#define COEFF_COUNT 3

class FormantIIRFilter {
private:
	double aCs[COEFF_COUNT];
	double bCs[COEFF_COUNT];
public:
	FormantIIRFilter();
	FormantIIRFilter(const FormantIIRFilter &filter);
	void setCoeffs(const double*, const double*);
	void generateCoeffs(double sampleRate, double cornerFreq, double bandwidth) noexcept;
	void processSamples(float* inputData, int inputLen);
};