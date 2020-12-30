#pragma once
#include <JuceHeader.h>

using namespace juce;

class WhiteNoiseAudioSource : public AudioSource{
private:
	float amplitude;
public:
	WhiteNoiseAudioSource(float amp);
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void setAmplitude(float amp);
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void releaseResources() override;
};