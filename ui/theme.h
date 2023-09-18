/* Copyright 2023 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#ifndef UI_THEME_H
#define UI_THEME_H

#include <stdint.h>

typedef struct {
	uint8_t X0, Y;
	uint8_t X1, W;
} UI_ThemePrint_t;

typedef struct {
	uint8_t X, Y;
} UI_ThemeXY_t;

typedef struct {
	uint8_t X, Y;
	uint8_t Count;
} UI_ThemeDigits_t;

typedef struct {
	uint8_t X, Y;
	uint16_t Size;
} UI_ThemeLine_t;

typedef struct {
	struct {
		UI_ThemePrint_t Status;
		UI_ThemeXY_t Freq;
		UI_ThemeDigits_t Digits;
		UI_ThemePrint_t Mode;
	} Aircopy;
	UI_ThemeLine_t Battery;
	struct {
		UI_ThemePrint_t Title;
		UI_ThemePrint_t Status;
		UI_ThemeXY_t Freq;
		UI_ThemePrint_t Channel;
	} FM;
	struct {
		UI_ThemePrint_t Title;
		UI_ThemePrint_t Code;
	} Lock;
	struct {
		UI_ThemePrint_t Lock0;
		UI_ThemePrint_t Lock1;
		uint8_t VfoLine0[2];
		uint8_t VfoLine1[2];
		UI_ThemePrint_t DtmfState[2];
		UI_ThemePrint_t DtmfContact[2];
		uint16_t VfoSelect[2];
		uint16_t RX_TX[2];
		uint16_t IconM[2];
		uint16_t IconF[2];
		uint16_t IconN[2];
		uint16_t IconSL1[2];
		uint16_t IconSL2[2];
		UI_ThemeDigits_t ChannelMR[2];
		UI_ThemeDigits_t ChannelFreq[2];
		UI_ThemeDigits_t ChannelNOAA[2];
		UI_ThemePrint_t VfoStatus[2];
		UI_ThemeXY_t Freq[2];
		UI_ThemeDigits_t FreqSmall[2];
		UI_ThemePrint_t ChannelNum[2];
		UI_ThemePrint_t ChannelName[2];
		struct {
			uint16_t Antenna[2][7];
			uint16_t AM[2];
			uint16_t CT[2];
			uint16_t DCS[2];
			uint16_t Power[2];
			uint16_t Offset[2];
			uint16_t Reverse[2];
			uint16_t Narrow[2];
			uint16_t DTMF[2];
			uint16_t Scramble[2];
		} BM;
	} Main;
	struct {
		UI_ThemePrint_t Freq;
		UI_ThemePrint_t Css;
		UI_ThemePrint_t Status;
	} Scanner;
	struct { 
		uint8_t PowerSave;
		uint8_t Charging;
		uint8_t KeyLock;
		uint8_t FKey;
		uint8_t Vox;
		uint8_t CrossBand;
		uint8_t DualWatch;
		uint8_t Prompt;
		uint8_t Killed;
		uint8_t FM;
		uint8_t NOAA;
	} Status;
	struct {
		UI_ThemePrint_t Line0;
		UI_ThemePrint_t Line1;
	} Welcome;
} UI_Theme_t;

extern UI_Theme_t gUI_Theme;
extern const UI_Theme_t gUI_ThemeDefault;

#endif

