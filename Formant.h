#pragma once
#include <JuceHeader.h>

using namespace juce;
using namespace std;

class Formant {
private:
public:
	vector<float> freqs;
	String sound;
	Formant() {};
	Formant(vector<float> freqs, String sound) : freqs(freqs), sound(sound) {};
};