/////////////////////////////////////////////////////////////////////
// FlangerTone.h: Extension of the KiwiWaves library to create the flanger effect.
// The delay time is modulated with an LFO, and a low-pass filter is added on the feedback loop.
// 
// MU620A[A] — Music Systems Programming 2 - Final project
// Maynooth University
// 
// 2024 Albert Madrenys
//
/////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <Delay.h>
#include <Osc.h>
#include <FuncTab.h>
#include <Tone.h>
#include <ExternalUGen.h>

class FlangerTone : public KiwiWaves::Delay
{
public:
	/** FlangerTone constructor. \n
		signalIn - input audio signal. \n
		oscFreq - frequency of the delay time LFO sine modulator. \n
		minDel - min value of delay. \n
		maxDel - max value of delay. \n
		depth - depth of the effect [0.0, 1.0]. \n
		feedback - amount of feedback [0.0, 1.0). \n
		cutFreq - cuttof frequency of the feedback low-pass filter. \n
		tsiz - size of the reading LFO cosine table. \n
		vsiz - number of frames in vector. \n
		sr - sampling rate.
	*/
	FlangerTone(UGen& signalIn, double oscFreq, double minDel, double maxDel, double depth = 1.0,
		double feedback = 0.9, double cutFreq = 5000.0, size_t tsiz = def_tsize, size_t vsiz = def_vsize, double sr = def_sr) :
		m_tab(tsiz, 1., 0.25), // phase added to be cosine
		m_delOsc((maxDel - std::max(minDel, 1 / sr)) * 0.5 * depth, oscFreq, m_tab, 0., (maxDel + std::max(minDel, 1 / sr)) * 0.5, vsiz, sr),
			// minDel cannot be smaller than 1/sr
		m_toneBuffer(1, sr), m_tone(m_toneBuffer, cutFreq, 1, sr), // only 1 sample buffer
		KiwiWaves::Delay(signalIn, maxDel, m_delOsc, feedback, true, vsiz, sr) { };

protected:
	void dsp() override
	{
		m_delOsc.process();
		KiwiWaves::Delay::dsp();
	}

private:
	KiwiWaves::SinTab m_tab;
	KiwiWaves::Osc m_delOsc;
	KiwiWaves::ToneLP m_tone;
	KiwiWaves::ExternalUGen m_toneBuffer;
		// Buffer that will be filled with the write position value, for then be sended to the tone filter

	double getFb(size_t pos) override
	{
		// To calculate the final feedback for this position, the original sample has to filtered.
		// The tone filter buffer has to contain only 1 sample, since the feedback needs to be processed sample by sample
		// This is because the delay line and the Ugen vector don't have the same size, and they are dependant on each other.
		m_toneBuffer.setData(m_s[pos], 0);
		return *(m_tone.process()) * m_fb[pos];
	}
};