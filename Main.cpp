/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SawToothGeneratorAudioSource.h"
#include "WhiteNoiseAudioSource.h"
#include "FormantAudioSource.h"

using namespace juce;
using namespace std;

vector<Formant> generateFormantData() {
	vector<Formant> result;

	//DATASET 1
	result.push_back(Formant({280, 2230}, "i"));
	result.push_back(Formant({370, 2090}, "I"));
	result.push_back(Formant({405, 2080}, "e"));
	result.push_back(Formant({600, 1930}, "E"));
	result.push_back(Formant({860, 1550}, "ae"));
	result.push_back(Formant({830, 1170}, "a"));
	result.push_back(Formant({560, 820}, "c"));
	result.push_back(Formant({430, 980}, "o"));
	result.push_back(Formant({400, 1100}, "U"));
	result.push_back(Formant({330, 1260}, "u"));
	result.push_back(Formant({680, 1310}, "^"));

	//DATASET 2
	result.push_back(Formant({ 270, 2300, 3000 }, "ee"));
	result.push_back(Formant({ 300, 870,  2250 }, "oo"));
	result.push_back(Formant({ 400, 2000, 2550 }, "i"));
	result.push_back(Formant({ 530, 1850, 2500 }, "e"));
	result.push_back(Formant({ 640, 1200, 2400 }, "u"));
	result.push_back(Formant({ 660, 1700, 2400 }, "a"));
	return result;
}

//==============================================================================
int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	ScopedJuceInitialiser_GUI gui_init;
	AudioDeviceManager aman;
	String err = aman.initialiseWithDefaultDevices(0, 2);
	double sampleRate = aman.getAudioDeviceSetup().sampleRate;
	if (err.isEmpty())
	{
		std::cout << "device opened : " << aman.getCurrentAudioDevice()->getName() << "\n";

		vector<Formant> formantList = generateFormantData();

		float targetFreq = 500;
		float BW = 300;
		WhiteNoiseAudioSource noise(5);

		//filter out low noise
		IIRFilterAudioSource lowPass(&noise, false);
		lowPass.setCoefficients(IIRCoefficients::makeLowPass(sampleRate, targetFreq + BW));

		//filter out high noise
		IIRFilterAudioSource highPass(&lowPass, false);
		highPass.setCoefficients(IIRCoefficients::makeHighPass(sampleRate, targetFreq - BW));
		
		FormantAudioSource formant = FormantAudioSource(&highPass, sampleRate, 1);
		formant.changeFormantFreqs(formantList.at(0));

		AudioSourcePlayer asplayer;
		asplayer.setSource(&formant);
		aman.addAudioCallback(&asplayer);

		int i = 0;
		srand(time(NULL));
		while(true) {
			cout << i << ": " << formantList.at(i).sound << endl;
			getchar();
			i++;
			i %= formantList.size();
			formant.changeFormantFreqs(formantList.at(i));
		}
		std::cout << "closing device...\n" << endl;
		aman.closeAudioDevice();
		std::cout << "device closed\n" << endl;
	} else {
		std::cout << "could not open device : " << err << "\n";
	}

	std::cout << "program exiting" << std::endl;
	return 0;
}
