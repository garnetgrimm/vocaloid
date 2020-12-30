#pragma once
#include <JuceHeader.h>

using namespace juce;

class SawToothGeneratorAudioSource : public ToneGeneratorAudioSource {
public:
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
};