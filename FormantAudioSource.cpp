#include "FormantAudioSource.h"

FormantAudioSource::FormantAudioSource(AudioSource * inputSrc, double sampleRate, int cascadeCount)
{
	input = inputSrc;
	this->sampleRate = sampleRate;
	this->cascadeCount = cascadeCount;
	changeFormantFreqs(Formant({}, "-"));
}

void FormantAudioSource::changeFormantFreqs(Formant formant)
{
	int formantCountDiff = formant.freqs.size() - formantFreqs;
	if (formantCountDiff > 0) {
		for (int i = 0; i < formantCountDiff; i++) {
			filter.push_back(FormantIIRFilter());
		}
	}
	else if (formantCountDiff < 0) {
		for (int i = 0; i < formantCountDiff; i++) {
			filter.pop_back();
		}
	}
	formantFreqs = formant.freqs.size();
	for (int i = 0; i < formantFreqs; i++) {
		filter.at(i).generateCoeffs(sampleRate, formant.freqs.at(i), 20);
	}
}

void FormantAudioSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	input->getNextAudioBlock(bufferToFill);

	const int numChannels = bufferToFill.buffer->getNumChannels();
	const int numSamples = bufferToFill.numSamples;

	for (int channel = 0; channel < numChannels; channel++) {
		float* inputSamplesPtr = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
		float* outputSamplesPtr = new float[numSamples];
		memset(outputSamplesPtr, 0, sizeof(float) * numSamples);
		if (!cascadeCount || !formantFreqs) {
			memcpy(outputSamplesPtr, inputSamplesPtr, sizeof(float) * numSamples);
		}
		for (int order = 0; order < cascadeCount; order++) {
			for (int formantIdx = 0; formantIdx < formantFreqs; formantIdx++) {
				float* filterSamplesPtr = new float[numSamples];
				memcpy(filterSamplesPtr, inputSamplesPtr, sizeof(float) * numSamples);
				filter.at(formantIdx).processSamples(filterSamplesPtr, numSamples);
				for (int i = 0; i < numSamples; i++) {
					double sample = 0;
					sample = filterSamplesPtr[i];
					sample /= formantFreqs;
					outputSamplesPtr[i] += sample;
				}
				delete[] filterSamplesPtr;
			}
		}
		memcpy(inputSamplesPtr, outputSamplesPtr, sizeof(float) * numSamples);
		delete[] outputSamplesPtr;
	}
}

void FormantAudioSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	input->prepareToPlay(samplesPerBlockExpected, sampleRate);
	for (int formantIdx = 0; formantIdx < formantFreqs; formantIdx++) {
		//filter[formantIdx].reset();
	}
}

void FormantAudioSource::releaseResources()
{
	input->releaseResources();
}