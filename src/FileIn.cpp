/////////////////////////////////////////////////////////////////////
// FileIn.cpp: Implementation of the FileIn class.
// 
// MU620A[A] — Music Systems Programming 2 - Final project
// Maynooth University
// 
// 2024 Albert Madrenys
//
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sndfile.h>
#include "FileIn.h"

FileIn::FileIn(const char* filename, size_t channelNumber, size_t vsiz, double sr) :
	m_sfp(nullptr), m_sfinfo(), m_samplesRead(0), m_chn(channelNumber), m_fileBuffer(), m_error(false),
	KiwiWaves::UGen(vsiz, sr)
{
	if ((m_sfp = sf_open(filename, SFM_READ, &m_sfinfo)) == nullptr)
	{
		std::cout << "Error: Not able to open the input file " << filename << "." << std::endl;
		m_error = true;
		return;
	}

	m_sr = m_sfinfo.samplerate;

	if (m_chn > m_sfinfo.channels)
	{
		std::cout << "Error: Not able to extract channel " << (int)m_chn << " from a file with "
			<< m_sfinfo.channels << " channels." << std::endl;
		m_error = true;
		return;
	}

	m_fileBuffer.resize(m_s.size() * m_sfinfo.channels);
}

FileIn::~FileIn()
{
	if (m_sfp) sf_close(m_sfp);
}

void FileIn::setChannel(size_t chn)
{
	m_chn = chn;
	if (m_chn > m_sfinfo.channels)
	{
		std::cout << "Error: Not able to extract channel " << (int)m_chn << " from a file with "
			<< m_sfinfo.channels << " channels." << std::endl;
		m_error = true;
		return;
	}
}

void FileIn::dsp()
{
    if (m_error) return;

    m_samplesRead = sf_readf_double(m_sfp, m_fileBuffer.data(), m_s.size());

	// Extract the correct channel from the multichannel file buffer
    for (size_t i = 0, j = m_chn - 1; i < m_samplesRead; i++, j += m_sfinfo.channels)
        m_s[i] = m_fileBuffer[j];

	// Fill the process vector with zeroes if the end of the file has been reached
    if (m_samplesRead < m_s.size())
        std::fill(m_s.begin() + m_samplesRead, m_s.end(), 0.);
}
