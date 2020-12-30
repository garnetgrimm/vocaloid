#include "SawToothGeneratorAudioSource.h"

//note - juce_audio_basics/sources/juce_ToneGeneratorAudioSource.h had to change all private to protected
void SawToothGeneratorAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& info)
{
	if (phasePerSample == 0.0)
		phasePerSample = (2 * amplitude*frequency) / (sampleRate);

	for (int i = 0; i < info.numSamples; ++i)
	{
		const float sample = fmod(currentPhase + amplitude, 2 * amplitude) - amplitude;
		currentPhase += phasePerSample;

		for (int j = info.buffer->getNumChannels(); --j >= 0;)
			info.buffer->setSample(j, info.startSample + i, sample);
	}
}