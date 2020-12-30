#include "WhiteNoiseAudioSource.h"

WhiteNoiseAudioSource::WhiteNoiseAudioSource(float amp)
{
	amplitude = amp;
}

//note - juce_audio_basics/sources/juce_ToneGeneratorAudioSource.h had to change all private to protected
void WhiteNoiseAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& data)
{

	const int numChannels = data.buffer->getNumChannels();
	const int numSamples = data.numSamples;

	for (int channel = 0; channel < numChannels; channel++) {
		for (int sampleIdx = 0; sampleIdx < numSamples; sampleIdx++)
		{
			float noise = ((float)rand() - (RAND_MAX/2)) / RAND_MAX;
			data.buffer->setSample(channel, data.startSample + sampleIdx, amplitude*noise);
		}
		float* inputSamplesPtr = data.buffer->getWritePointer(channel, data.startSample);
	}
}

void WhiteNoiseAudioSource::setAmplitude(float amp)
{
	amplitude = amp;
}

void WhiteNoiseAudioSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
}

void WhiteNoiseAudioSource::releaseResources()
{
}