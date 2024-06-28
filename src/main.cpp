/////////////////////////////////////////////////////////////////////
// KiwiFlanger: Implementation of the flanger effect using the KiwiWaves library.
// main.cpp: main file of the program.
// 
// MU620A[A] — Music Systems Programming 2 - Final project
// Maynooth University
// 
// 2024 Albert Madrenys
//
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <sndfile.h>
#include <Balance.h>
#include "FileIn.h"
#include "FlangerTone.h"

int main(int argc, char** argv)
{
	SF_INFO sfinfo_out;
	SNDFILE* sfp_out;

	const double minDel = 0.0005, maxDel = 0.002;

	std::string filename_in, filename_out;
	size_t chnNum = 1;
	double oscFreq = 0.5; // positive
	double depth = 1.0; // from 0 to 1
	double feedback = 0.9; // from 0 to 0.999
	double cutFreq = 5000.0; // positive

	std::cout << "---Flanger using KiwiWaves---" << std::endl << "  *  Enter the input audio file: ";
	std::cin >> filename_in;

	FileIn fileIn(filename_in.c_str(), chnNum, def_vsize);
	if (fileIn.error()) return 1;

	if (fileIn.getSFInfo()->channels > 1)
	{
		std::cout << "  *  " << fileIn.getSFInfo()->channels << " channels detected. Enter the channel number [1, "
			<< fileIn.getSFInfo()->channels << "]: ";
		std::cin >> chnNum;
		fileIn.setChannel(chnNum);
		if (fileIn.error()) return 2;
	}

	std::cout << "  *  Enter the audio output file: ";
	std::cin >> filename_out;

	sfinfo_out.channels = 1;
	sfinfo_out.samplerate = fileIn.getSFInfo()->samplerate;
	sfinfo_out.format = fileIn.getSFInfo()->format;
	if ((sfp_out = sf_open(filename_out.c_str(), SFM_WRITE, &sfinfo_out)) == NULL)
	{
		std::cout << "Error while opening the output file " << filename_out << "." << std::endl;
		return 3;
	}

	std::cout << "  *  Enter the frequency of the flanger LFO: ";
	std::cin >> oscFreq;

	if (oscFreq < 0.)
	{
		std::cout << "Only positive frequencies allowed." << std::endl;
		sf_close(sfp_out);
		return 4;
	}

	std::cout << "  *  Enter the depth of the flanger effect [0.0, 1.0]: ";
	std::cin >> depth;

	if (depth < 0. || depth > 1.)
	{
		std::cout << "Error: The value of the depth is out of bounds." << std::endl;
		sf_close(sfp_out);
		return 5;
	}

	std::cout << "  *  Enter amount of feedback [0.0, 1.0): ";
	std::cin >> feedback;

	if (feedback < 0. || feedback >= 1.)
	{
		std::cout << "Error: The value of the feedback is out of bounds." << std::endl;
		sf_close(sfp_out);
		return 6;
	}

	std::cout << "  *  Enter cutoff frequency of the feedback low-pass filter (around 5000 Hz recommended): ";
	std::cin >> cutFreq;

	if (cutFreq < 0.)
	{
		std::cout << "Error: Only positive frequencies allowed." << std::endl;
		sf_close(sfp_out);
		return 7;
	}
	
	FlangerTone flan(fileIn, oscFreq, minDel, maxDel, depth, feedback, cutFreq, def_tsize, def_vsize, sfinfo_out.samplerate);
	KiwiWaves::Balance bal(flan, fileIn, 10., addSmallNumber, def_vsize, sfinfo_out.samplerate);
	
	std::cout << "  -  Processing the effect..." << std::endl;
	// Overall output file duration will be the original one + the delay caused by the flanger
	int64_t framesToWrite = fileIn.getSFInfo()->frames + flan.getDelayline().size();
	do {
		fileIn.process();
		flan.process();
		sf_write_double(sfp_out, bal.process(), std::min((int64_t)bal.vsize(), framesToWrite));
		framesToWrite -= bal.vsize();
	} while (framesToWrite > 0);
	sf_close(sfp_out);

	std::cout << "  -  Effect successfully processed. Closing program." << std::endl;
	return 0;
}