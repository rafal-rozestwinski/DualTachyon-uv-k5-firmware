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

#include "ui/theme.h"

UI_Theme_t gUI_Theme;

#define LINE0 (0 * 128)
#define LINE1 (1 * 128)
#define LINE2 (2 * 128)
#define LINE3 (3 * 128)
#define LINE4 (4 * 128)
#define LINE5 (5 * 128)
#define LINE6 (6 * 128)

const UI_Theme_t gUI_ThemeDefault = {
	.Aircopy = {
		.Status = { 2, 0, 127, 8 },
		.Freq = { 16, 2 },
		.Digits = { 97, 3, 2 },
		.Mode = { 2, 4, 127, 8 },
	},
	.Battery = {
		.X = 110,
		.Y = 0,
		.Size = 18,
	},
	.FM = {
		.Title = { 0, 0, 127, 12 },
		.Status = { 0, 2, 127, 10 },
		.Freq = { 23, 4 },
		.Channel = { 0, 4, 127, 10 },
	},
	.Lock = {
		.Title = { 0, 1, 127, 10 },
		.Code = { 0, 3, 127, 12 },
	},
	.Main = {
		.Lock0 = { 0, 1, 127, 8 },
		.Lock1 = { 0, 3, 127, 8 },

		.DtmfState   = { { 2, 0, 127, 8 }, { 2, 3, 127, 8 } },
		.DtmfContact = { { 2, 2, 127, 8 }, { 2, 5, 127, 8 } },

		.VfoSelect = { LINE0 +  2, LINE4 +  2 },
		.RX_TX     = { LINE0 + 14, LINE4 + 14 },
		.IconM     = { LINE1 +  2, LINE5 +  2 },
		.IconF     = { LINE1 + 14, LINE5 + 14 },
		.IconN     = { LINE1 +  7, LINE5 +  7 },
		.IconSL1   = { LINE0 + 113, LINE1 + 113 },
		.IconSL2   = { LINE0 + 120, LINE1 + 120 },

		.ChannelMR   = { { 10, 1, 3 }, { 10, 5, 3 } },
		.ChannelFreq = { { 22, 1, 1 }, { 22, 5, 1 } },
		.ChannelNOAA = { { 15, 1, 2 }, { 15, 5, 2 } },
		.VfoStatus = { { 31, 0, 111, 0 }, { 31, 4, 111, 0 } }, // W is not used
		.Freq = { { 31, 0 }, { 31, 4 } },
		.FreqSmall = { { 112, 1, 2 }, { 112, 5, 2 } },
		.ChannelNum = { { 31, 0, 112, 8 }, { 31, 4, 112, 8 } },
		.ChannelName = { { 31, 0, 112, 8 }, { 31, 4, 112, 8 } },
		.BM = {
			.Antenna = {
				{ LINE2 +  0, LINE2 +  5, LINE2 +  8, LINE2 + 11, LINE2 + 14, LINE2 + 17, LINE2 + 20 },
				{ LINE6 +  0, LINE6 +  5, LINE6 +  8, LINE6 + 11, LINE6 + 14, LINE6 + 17, LINE6 + 20 },
			},
			.AM       = { LINE2 +  27, LINE6 +  27 },
			.CT       = { LINE2 +  27, LINE6 +  27 },
			.DCS      = { LINE2 +  24, LINE6 +  24 },
			.Power    = { LINE2 +  44, LINE6 +  44 },
			.Offset   = { LINE2 +  54, LINE6 +  54 },
			.Reverse  = { LINE2 +  64, LINE6 +  64 },
			.Narrow   = { LINE2 +  74, LINE6 +  74 },
			.DTMF     = { LINE2 +  84, LINE6 +  84 },
			.Scramble = { LINE2 + 110, LINE6 + 110 },
		},
	},
	.Scanner = {
		.Freq = { 2, 1, 127, 8 },
		.Css = { 2, 3, 127, 8 },
		.Status = { 0, 5, 127, 8 },
	}, 
	.Status = { 
		.PowerSave = 0,
		.Charging = 100,
		.KeyLock = 90,
		.FKey = 90,
		.Vox = 71,
		.CrossBand = 58,
		.DualWatch = 45,
		.Prompt = 34,
		.Killed = 21,
		.FM = 21,
		.NOAA = 7,
	},
	.Welcome = {
		.Line0 = { 0, 1, 127, 10 },
		.Line1 = { 0, 3, 127, 10 },
	},
};

