/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#ifndef MIXER_H
#define MIXER_H

#include "Types.h"
#include "../Common.h"
#include "../Blip_Buffer/Blip_Buffer.h"

enum chip_level_t {
	CHIP_LEVEL_APU1,
	CHIP_LEVEL_APU2,
	CHIP_LEVEL_VRC6,
	CHIP_LEVEL_VRC7,
	CHIP_LEVEL_MMC5,
	CHIP_LEVEL_FDS,
	CHIP_LEVEL_N163,
	CHIP_LEVEL_S5B
};

class CMixer
{
public:
	CMixer();
	~CMixer();

	void	ExternalSound(int Chip);
	void	AddValue(int ChanID, int Chip, int Value, int AbsValue, int FrameCycles);
	void	UpdateSettings(int LowCut,	int HighCut, int HighDamp, float OverallVol);

	bool	AllocateBuffer(unsigned int Size, uint32 SampleRate, uint8 NrChannels);
	void	SetClockRate(uint32 Rate);
	void	ClearBuffer();
	int		FinishBuffer(int t);
	int		SamplesAvail() const;
	void	MixSamples(blip_sample_t *pBuffer, uint32 Count);
	uint32	GetMixSampleCount(int t) const;

	void	AddSample(int ChanID, int Value);
	int		ReadBuffer(int Size, void *Buffer, bool Stereo);

	int32	GetChanOutput(uint8 Chan) const;
	void	SetChipLevel(chip_level_t Chip, float Level);
	uint32	ResampleDuration(uint32 Time) const;
	void	SetNamcoVolume(float fVol);

private:
	inline double CalcPin1(double Val1, double Val2);
	inline double CalcPin2(double Val1, double Val2, double Val3);

	void MixInternal1(int Time);
	void MixInternal2(int Time);
	void MixN163(int Value, int Time);
	void MixFDS(int Value, int Time);
	void MixVRC6(int Value, int Time);
	void MixMMC5(int Value, int Time);
	void MixS5B(int Value, int Time);

	void StoreChannelLevel(int Channel, int Value);
	void ClearChannelLevels();

	float GetAttenuation() const;

private:
	// Blip buffer synths
	Blip_Synth<blip_good_quality, -500>		Synth2A03SS;
	Blip_Synth<blip_good_quality, -500>		Synth2A03TND;
	Blip_Synth<blip_good_quality, -500>		SynthVRC6;
	Blip_Synth<blip_good_quality, -130>		SynthMMC5;	
	Blip_Synth<blip_good_quality, -1600>	SynthN163;
	Blip_Synth<blip_good_quality, -3500>	SynthFDS;
	Blip_Synth<blip_good_quality, -2000>	SynthS5B;
	
	// Blip buffer object
	Blip_Buffer	BlipBuffer;

	double		m_dSumSS;
	double		m_dSumTND;

	int32		m_iChannels[CHANNELS];
	uint8		m_iExternalChip;
	uint32		m_iSampleRate;

	float		m_fChannelLevels[CHANNELS];
	uint32		m_iChanLevelFallOff[CHANNELS];

	int			m_iLowCut;
	int			m_iHighCut;
	int			m_iHighDamp;
	float		m_fOverallVol;

	float		m_fLevelAPU1;
	float		m_fLevelAPU2;
	float		m_fLevelVRC6;
	float		m_fLevelMMC5;
	float		m_fLevelFDS;
	float		m_fLevelN163;
};

#endif /* MIXER_H */
