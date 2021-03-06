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

#pragma once

#include "src/polyfill.h"

// CSettings command target

enum EDIT_STYLES {
	EDIT_STYLE1 = 0,		// FT2
	EDIT_STYLE2 = 1,		// ModPlug
	EDIT_STYLE3 = 2			// IT
};

enum WIN_STATES {
	STATE_NORMAL,
	STATE_MAXIMIZED
};

enum PATHS {
	PATH_FTM,
	PATH_FTI,
	PATH_NSF,
	PATH_DMC,
	PATH_WAV,

	PATH_COUNT
};

// Base class for settings, pure virtual
class CSettingBase {
public:
	CSettingBase(LPCTSTR pSection, LPCTSTR pEntry) : m_pSection(pSection), m_pEntry(pEntry) {};
	virtual ~CSettingBase() {}
	virtual void Load() = 0;
	virtual void Save() = 0;
	virtual void Default() = 0;
	LPCTSTR GetSection() const { return m_pSection; };
protected:
	LPCTSTR m_pSection;
	LPCTSTR m_pEntry;
};

// Templated setting class
template <class T>
class CSettingType : public CSettingBase {
public:
	CSettingType(LPCTSTR pSection, LPCTSTR pEntry, T defaultVal, T *pVar) : CSettingBase(pSection, pEntry), m_tDefaultValue(defaultVal), m_pVariable(pVar) {};
	virtual void Load();
	virtual void Save();
	virtual void Default();
protected:
	T *m_pVariable;
	T m_tDefaultValue;
};

// Settings collection
class CSettings : public CObject
{
private:
	CSettings();

public:
	virtual ~CSettings();

	void	LoadSettings();
	void	SaveSettings();
	void	DefaultSettings();
	void	DeleteSettings();
	void	SetWindowPos(int Left, int Top, int Right, int Bottom, int State);

//	void	StoreSetting(CString Section, CString Name, int Value) const;
//	int		LoadSetting(CString Section, CString Name, int Default) const;

	CString GetPath(unsigned int PathType) const;
	void	SetPath(CString PathName, unsigned int PathType);

public:
	static CSettings* GetObject();

public:
	// Local cache of all settings (all public)

	struct {
		bool	bWrapCursor;
		bool	bWrapFrames;
		bool	bFreeCursorEdit;
		bool	bWavePreview;
		bool	bKeyRepeat;
		bool	bRowInHex;
		bool	bFramePreview;
		int		iEditStyle;
		bool	bNoDPCMReset;
		bool	bNoStepMove;
		int		iPageStepSize;
		CString	strFont;
		bool	bPatternColor;
		bool	bPullUpDelete;
		bool	bBackups;
		int		iFontSize;
		bool	bSingleInstance;
		bool	bPreviewFullRow;
		bool	bDisplayFlats;
		bool	bDblClickSelect;
	} General;

	struct {
		int		iDevice;
		int		iSampleRate;
		int		iSampleSize;
		int		iBufferLength;
		int		iBassFilter;
		int		iTrebleFilter;
		int		iTrebleDamping;
		int		iMixVolume;
	} Sound;

	struct {
		int		iMidiDevice;
		int		iMidiOutDevice;
		bool	bMidiMasterSync;
		bool	bMidiKeyRelease;
		bool	bMidiChannelMap;
		bool	bMidiVelocity;
		bool	bMidiArpeggio;
	} Midi;

	struct {
		int		iColBackground;
		int		iColBackgroundHilite;
		int		iColBackgroundHilite2;
		int		iColPatternText;
		int		iColPatternTextHilite;
		int		iColPatternTextHilite2;
		int		iColPatternInstrument;
		int		iColPatternVolume;
		int		iColPatternEffect;
		int		iColSelection;
		int		iColCursor;
	} Appearance;

	struct {
		int		iLeft;
		int		iTop;
		int		iRight;
		int		iBottom;
		int		iState;
	} WindowPos;

	struct {
		int		iKeyNoteCut;
		int		iKeyNoteRelease;
		int		iKeyClear;
		int		iKeyRepeat;
	} Keys;

	// Other
	int SampleWinState;
	int FrameEditPos;
	bool FollowMode;

	struct {
		int		iLevelAPU1;
		int		iLevelAPU2;
		int		iLevelVRC6;
		int		iLevelVRC7;
		int		iLevelMMC5;
		int		iLevelFDS;
		int		iLevelN163;
		int		iLevelS5B;
	} ChipLevels;

	CString InstrumentMenuPath;

private:
	template<class T> void AddSetting(LPCTSTR pSection, LPCTSTR pEntry, T tDefault, T* pVariable);
	void AddSetting(CSettingBase *pSetting);
	void SetupSettings();

private:
	static const int MAX_SETTINGS = 128;

private:
	CSettingBase *m_pSettings[MAX_SETTINGS];
	int m_iAddedSettings;

private:
	// Paths
	CString Paths[PATH_COUNT];
};


