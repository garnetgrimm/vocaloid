#pragma once

#include <JuceHeader.h>
#include "Formant.h"
#include "FormantIIRFilter.h"

using namespace juce;

class FormantAudioSource : public AudioSource {
private:
	vector<FormantIIRFilter> filter;
	AudioSource* input;
	double sampleRate;
	int cascadeCount;
	int formantFreqs;
public:
	FormantAudioSource(AudioSource* inputSrc, double sampleRate, int cascadeCount);
	void changeFormantFreqs(Formant formant);
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void releaseResources() override;
};