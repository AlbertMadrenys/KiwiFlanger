/////////////////////////////////////////////////////////////////////
// FileIn.h: Extension of the library to create a new UGen that, on the process call, reads an input file.
// Uses the SndFile library.
// 
// MU620A[A] — Music Systems Programming 2 - Final project
// Maynooth University
// 
// 2024 Albert Madrenys
//
/////////////////////////////////////////////////////////////////////
#include <sndfile.h>
#include <UGen.h>

class FileIn : public KiwiWaves::UGen
{
public:
	/** FileIn constructor. \n
		filename - name of the read file. \n
		channelNumber - channel of the file to be extracted. \n
		vsiz - number of frames in vector. \n
		sr - sampling rate.
	*/
	FileIn(const char* filename, size_t channelNumber = 1, size_t vsiz = def_vsize, double sr = def_sr);

	~FileIn() override;

	/** Sets the channel to extract. Cannot exceed the number of channels of the input file.
	*/
	void setChannel(size_t chn);

	const SF_INFO* getSFInfo() const { return &m_sfinfo; }
	size_t samplesRead() const { return m_samplesRead; }
	bool ended() const  { return m_samplesRead < m_s.size(); }
	bool error() const { return m_error; }

protected:
	void dsp() override;

private:
	size_t m_samplesRead;
	SF_INFO m_sfinfo;
	SNDFILE* m_sfp;

	size_t m_chn;
	std::vector<double> m_fileBuffer; // Size will be numFrames * numChanels
	bool m_error;
};