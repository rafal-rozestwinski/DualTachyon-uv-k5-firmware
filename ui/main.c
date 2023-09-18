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

#include <string.h>
#include "app/dtmf.h"
#include "bitmaps.h"
#include "driver/st7565.h"
#include "external/printf/printf.h"
#include "functions.h"
#include "misc.h"
#include "radio.h"
#include "settings.h"
#include "ui/helper.h"
#include "ui/inputbox.h"
#include "ui/main.h"
#include "ui/theme.h"

#define GET_PTR(x) ((uint8_t *)gFrameBuffer + (x))

void UI_DisplayMain(void)
{
	char String[16];
	uint8_t i;

	memset(gFrameBuffer, 0, sizeof(gFrameBuffer));
	if (gEeprom.KEY_LOCK && gKeypadLocked) {
		UI_PrintString("Long Press #", gUI_Theme.Main.Lock0.X0, gUI_Theme.Main.Lock0.X1, gUI_Theme.Main.Lock0.Y, gUI_Theme.Main.Lock0.W, true);
		UI_PrintString("To Unlock", gUI_Theme.Main.Lock1.X0, gUI_Theme.Main.Lock1.X1, gUI_Theme.Main.Lock1.Y, gUI_Theme.Main.Lock1.W, true);
		ST7565_BlitFullScreen();
		return;
	}

	for (i = 0; i < 2; i++) {
		uint8_t Channel;
		bool bIsSameVfo;

		Channel = gEeprom.TX_CHANNEL;
		bIsSameVfo = !!(Channel == i);

		if (gEeprom.DUAL_WATCH != DUAL_WATCH_OFF && gRxVfoIsActive) {
			Channel = gEeprom.RX_CHANNEL;
		}

		if (Channel != i) {
			if (gDTMF_CallState != DTMF_CALL_STATE_NONE || gDTMF_IsTx || gDTMF_InputMode) {
				char Contact[16];

				if (!gDTMF_InputMode) {
					if (gDTMF_CallState == DTMF_CALL_STATE_CALL_OUT) {
						if (gDTMF_State == DTMF_STATE_CALL_OUT_RSP) {
							strcpy(String, "CALL OUT(RSP)");
						} else {
							strcpy(String, "CALL OUT");
						}
					} else if (gDTMF_CallState == DTMF_CALL_STATE_RECEIVED) {
						if (DTMF_FindContact(gDTMF_Caller, Contact)) {
							sprintf(String, "CALL:%s", Contact);
						} else {
							sprintf(String, "CALL:%s", gDTMF_Caller);
						}
					} else if (gDTMF_IsTx) {
						if (gDTMF_State == DTMF_STATE_TX_SUCC) {
							strcpy(String, "DTMF TX(SUCC)");
						} else {
							strcpy(String, "DTMF TX");
						}
					}
				} else {
					sprintf(String, ">%s", gDTMF_InputBox);
				}
				UI_PrintString(String, gUI_Theme.Main.DtmfState[i].X0, gUI_Theme.Main.DtmfState[i].X1, gUI_Theme.Main.DtmfState[i].Y, gUI_Theme.Main.DtmfState[i].W, false);

				memset(String, 0, sizeof(String));
				memset(Contact, 0, sizeof(Contact));

				if (!gDTMF_InputMode) {
					if (gDTMF_CallState == DTMF_CALL_STATE_CALL_OUT) {
						if (DTMF_FindContact(gDTMF_String, Contact)) {
							sprintf(String, ">%s", Contact);
						} else {
							sprintf(String, ">%s", gDTMF_String);
						}
					} else if (gDTMF_CallState == DTMF_CALL_STATE_RECEIVED) {
						if (DTMF_FindContact(gDTMF_Callee, Contact)) {
							sprintf(String, ">%s", Contact);
						} else {
							sprintf(String, ">%s", gDTMF_Callee);
						}
					} else if (gDTMF_IsTx) {
						sprintf(String, ">%s", gDTMF_String);
					}
				}
				UI_PrintString(String, gUI_Theme.Main.DtmfContact[i].X0, gUI_Theme.Main.DtmfContact[i].X1, gUI_Theme.Main.DtmfContact[i].Y, gUI_Theme.Main.DtmfContact[i].W, false);
				continue;
			} else if (bIsSameVfo) {
				memcpy(GET_PTR(gUI_Theme.Main.VfoSelect[i]), BITMAP_VFO_Default, sizeof(BITMAP_VFO_Default));
			}
		} else {
			if (bIsSameVfo) {
				memcpy(GET_PTR(gUI_Theme.Main.VfoSelect[i]), BITMAP_VFO_Default, sizeof(BITMAP_VFO_Default));
			} else {
				memcpy(GET_PTR(gUI_Theme.Main.VfoSelect[i]), BITMAP_VFO_NotDefault, sizeof(BITMAP_VFO_NotDefault));
			}
		}

		// 0x8EE2
		uint32_t SomeValue = 0;

		if (gCurrentFunction == FUNCTION_TRANSMIT) {
#if defined(ENABLE_ALARM)
			if (gAlarmState == ALARM_STATE_ALARM) {
				SomeValue = 2;
			} else {
#else
			if (1) {
#endif
				if (gEeprom.CROSS_BAND_RX_TX == CROSS_BAND_OFF) {
					Channel = gEeprom.RX_CHANNEL;
				} else {
					Channel = gEeprom.TX_CHANNEL;
				}
				if (Channel == i) {
					SomeValue = 1;
					memcpy(GET_PTR(gUI_Theme.Main.RX_TX[i]), BITMAP_TX, sizeof(BITMAP_TX));
				}
			}
		} else {
			SomeValue = 2;
			if ((gCurrentFunction == FUNCTION_RECEIVE || gCurrentFunction == FUNCTION_MONITOR) && gEeprom.RX_CHANNEL == i) {
				memcpy(GET_PTR(gUI_Theme.Main.RX_TX[i]), BITMAP_RX, sizeof(BITMAP_RX));
			}
		}

		// 0x8F3C
		if (IS_MR_CHANNEL(gEeprom.ScreenChannel[i])) {
			memcpy(GET_PTR(gUI_Theme.Main.IconM[i]), BITMAP_M, sizeof(BITMAP_M));
			if (gInputBoxIndex == 0 || gEeprom.TX_CHANNEL != i) {
				NUMBER_ToDigits(gEeprom.ScreenChannel[i] + 1, String);
			} else {
				memcpy(String + 5, gInputBox, 3);
			}
			UI_DisplaySmallDigits(gUI_Theme.Main.ChannelMR[i].Count, String + 5, gUI_Theme.Main.ChannelMR[i].X, gUI_Theme.Main.ChannelMR[i].Y);
		} else if (IS_FREQ_CHANNEL(gEeprom.ScreenChannel[i])) {
			char c;

			memcpy(GET_PTR(gUI_Theme.Main.IconF[i]), BITMAP_F, sizeof(BITMAP_F));
			c = (gEeprom.ScreenChannel[i] - FREQ_CHANNEL_FIRST) + 1;
			UI_DisplaySmallDigits(gUI_Theme.Main.ChannelFreq[i].Count, &c, gUI_Theme.Main.ChannelFreq[i].X, gUI_Theme.Main.ChannelFreq[i].Y);
		} else {
#if defined(ENABLE_NOAA)
			memcpy(GET_PTR(gUI_Theme.Main.IconN[i]), BITMAP_NarrowBand, sizeof(BITMAP_NarrowBand));
			if (gInputBoxIndex == 0 || gEeprom.TX_CHANNEL != i) {
				NUMBER_ToDigits((gEeprom.ScreenChannel[i] - NOAA_CHANNEL_FIRST) + 1, String);
			} else {
				String[6] = gInputBox[0];
				String[7] = gInputBox[1];
			}
			UI_DisplaySmallDigits(gUI_Theme.Main.ChannelNOAA[i].Count, String + 6, gUI_Theme.Main.ChannelNOAA[i].X, gUI_Theme.Main.ChannelNOAA[i].Y);
#endif
		}

		// 0x8FEC

		uint8_t State = VfoState[i];
#if defined(ENABLE_ALARM)
		if (gCurrentFunction == FUNCTION_TRANSMIT && gAlarmState == ALARM_STATE_ALARM) {
			if (gEeprom.CROSS_BAND_RX_TX == CROSS_BAND_OFF) {
				Channel = gEeprom.RX_CHANNEL;
			} else {
				Channel = gEeprom.TX_CHANNEL;
			}
			if (Channel == i) {
				State = VFO_STATE_ALARM;
			}
		}
#endif
		if (State) {
			uint8_t Width = 10;

			memset(String, 0, sizeof(String));
			switch (State) {
			case 1:
				strcpy(String, "BUSY");
				Width = 15;
				break;
			case 2:
				strcpy(String, "BAT LOW");
				break;
			case 3:
				strcpy(String, "DISABLE");
				break;
			case 4:
				strcpy(String, "TIMEOUT");
				break;
#if defined(ENABLE_ALARM)
			case 5:
				strcpy(String, "ALARM");
				break;
#endif
			case 6:
				sprintf(String, "VOL HIGH");
				Width = 8;
				break;
			}
			UI_PrintString(String, gUI_Theme.Main.VfoStatus[i].X0, gUI_Theme.Main.VfoStatus[i].X1, gUI_Theme.Main.VfoStatus[i].Y, Width, true);
		} else {
			if (gInputBoxIndex && IS_FREQ_CHANNEL(gEeprom.ScreenChannel[i]) && gEeprom.TX_CHANNEL == i) {
				UI_DisplayFrequency(gInputBox, gUI_Theme.Main.Freq[i].X, gUI_Theme.Main.Freq[i].Y, true, false);
			} else {
				if (!IS_MR_CHANNEL(gEeprom.ScreenChannel[i]) || gEeprom.CHANNEL_DISPLAY_MODE == MDF_FREQUENCY) {
					if (gCurrentFunction == FUNCTION_TRANSMIT) {
						if (gEeprom.CROSS_BAND_RX_TX == CROSS_BAND_OFF) {
							Channel = gEeprom.RX_CHANNEL;
						} else {
							Channel = gEeprom.TX_CHANNEL;
						}
						if (Channel == i) {
							NUMBER_ToDigits(gEeprom.VfoInfo[i].pTX->Frequency, String);
						} else {
							NUMBER_ToDigits(gEeprom.VfoInfo[i].pRX->Frequency, String);
						}
					} else {
						NUMBER_ToDigits(gEeprom.VfoInfo[i].pRX->Frequency, String);
					}
					UI_DisplayFrequency(String, gUI_Theme.Main.Freq[i].X, gUI_Theme.Main.Freq[i].Y, false, false);
					if (IS_MR_CHANNEL(gEeprom.ScreenChannel[i])) {
						const uint8_t Attributes = gMR_ChannelAttributes[gEeprom.ScreenChannel[i]];
						if (Attributes & MR_CH_SCANLIST1) {
							memcpy(GET_PTR(gUI_Theme.Main.IconSL1[i]), BITMAP_ScanList, sizeof(BITMAP_ScanList));
						}
						if (Attributes & MR_CH_SCANLIST2) {
							memcpy(GET_PTR(gUI_Theme.Main.IconSL2[i]), BITMAP_ScanList, sizeof(BITMAP_ScanList));
						}
					}
					UI_DisplaySmallDigits(gUI_Theme.Main.FreqSmall[i].Count, String + 6, gUI_Theme.Main.FreqSmall[i].X, gUI_Theme.Main.FreqSmall[i].Y);
				} else if (gEeprom.CHANNEL_DISPLAY_MODE == MDF_CHANNEL) {
					sprintf(String, "CH-%03d", gEeprom.ScreenChannel[i] + 1);
					UI_PrintString(String, gUI_Theme.Main.ChannelNum[i].X0, gUI_Theme.Main.ChannelNum[i].X1, gUI_Theme.Main.ChannelNum[i].Y, gUI_Theme.Main.ChannelNum[i].W, true);
				} else if (gEeprom.CHANNEL_DISPLAY_MODE == MDF_NAME) {
					if(gEeprom.VfoInfo[i].Name[0] == 0 || gEeprom.VfoInfo[i].Name[0] == 0xFF) {
						sprintf(String, "CH-%03d", gEeprom.ScreenChannel[i] + 1);
						UI_PrintString(String, gUI_Theme.Main.ChannelNum[i].X0, gUI_Theme.Main.ChannelNum[i].X1, gUI_Theme.Main.ChannelNum[i].Y, gUI_Theme.Main.ChannelNum[i].W, true);
					} else {
						UI_PrintString(gEeprom.VfoInfo[i].Name, gUI_Theme.Main.ChannelName[i].X0, gUI_Theme.Main.ChannelName[i].X1, gUI_Theme.Main.ChannelName[i].Y, gUI_Theme.Main.ChannelName[i].W, true);
					}
				}
			}
		}

		// 0x926E
		uint8_t Level = 0;

		if (SomeValue == 1) {
				if (gRxVfo->OUTPUT_POWER == OUTPUT_POWER_LOW) {
					Level = 2;
				} else if (gRxVfo->OUTPUT_POWER == OUTPUT_POWER_MID) {
					Level = 4;
				} else {
					Level = 6;
				}		
		} else if (SomeValue == 2) {
			if (gVFO_RSSI_Level[i]) {
				Level = gVFO_RSSI_Level[i];
			}
		}

		// TODO: not quite how the original does it, but it's quite entangled in Ghidra.
		if (Level) {
			memcpy(GET_PTR(gUI_Theme.Main.BM.Antenna[i][0]), BITMAP_Antenna, sizeof(BITMAP_Antenna));
			memcpy(GET_PTR(gUI_Theme.Main.BM.Antenna[i][1]), BITMAP_AntennaLevel1, sizeof(BITMAP_AntennaLevel1));
			if (Level >= 2) {
				memcpy(GET_PTR(gUI_Theme.Main.BM.Antenna[i][2]), BITMAP_AntennaLevel2, sizeof(BITMAP_AntennaLevel2));
			}
			if (Level >= 3) {
				memcpy(GET_PTR(gUI_Theme.Main.BM.Antenna[i][3]), BITMAP_AntennaLevel3, sizeof(BITMAP_AntennaLevel3));
			}
			if (Level >= 4) {
				memcpy(GET_PTR(gUI_Theme.Main.BM.Antenna[i][4]), BITMAP_AntennaLevel4, sizeof(BITMAP_AntennaLevel4));
			}
			if (Level >= 5) {
				memcpy(GET_PTR(gUI_Theme.Main.BM.Antenna[i][5]), BITMAP_AntennaLevel5, sizeof(BITMAP_AntennaLevel5));
			}
			if (Level >= 6) {
				memcpy(GET_PTR(gUI_Theme.Main.BM.Antenna[i][6]), BITMAP_AntennaLevel6, sizeof(BITMAP_AntennaLevel6));
			}
		}

		// 0x931E
		if (gEeprom.VfoInfo[i].IsAM) {
			memcpy(GET_PTR(gUI_Theme.Main.BM.AM[i]), BITMAP_AM, sizeof(BITMAP_AM));
		} else {
			const FREQ_Config_t *pConfig;

			if (SomeValue == 1) {
				pConfig = gEeprom.VfoInfo[i].pTX;
			} else {
				pConfig = gEeprom.VfoInfo[i].pRX;
			}
			switch (pConfig->CodeType) {
			case CODE_TYPE_CONTINUOUS_TONE:
				memcpy(GET_PTR(gUI_Theme.Main.BM.CT[i]), BITMAP_CT, sizeof(BITMAP_CT));
				break;
			case CODE_TYPE_DIGITAL:
			case CODE_TYPE_REVERSE_DIGITAL:
				memcpy(GET_PTR(gUI_Theme.Main.BM.DCS[i]), BITMAP_DCS, sizeof(BITMAP_DCS));
				break;
			default:
				break;
			}
		}

		// 0x936C
		switch (gEeprom.VfoInfo[i].OUTPUT_POWER) {
		case OUTPUT_POWER_LOW:
			memcpy(GET_PTR(gUI_Theme.Main.BM.Power[i]), BITMAP_PowerLow, sizeof(BITMAP_PowerLow));
			break;
		case OUTPUT_POWER_MID:
			memcpy(GET_PTR(gUI_Theme.Main.BM.Power[i]), BITMAP_PowerMid, sizeof(BITMAP_PowerMid));
			break;
		case OUTPUT_POWER_HIGH:
			memcpy(GET_PTR(gUI_Theme.Main.BM.Power[i]), BITMAP_PowerHigh, sizeof(BITMAP_PowerHigh));
			break;
		}

		if (gEeprom.VfoInfo[i].ConfigRX.Frequency != gEeprom.VfoInfo[i].ConfigTX.Frequency) {
			if (gEeprom.VfoInfo[i].FREQUENCY_DEVIATION_SETTING == FREQUENCY_DEVIATION_ADD) {
				memcpy(GET_PTR(gUI_Theme.Main.BM.Offset[i]), BITMAP_Add, sizeof(BITMAP_Add));
			}
			if (gEeprom.VfoInfo[i].FREQUENCY_DEVIATION_SETTING == FREQUENCY_DEVIATION_SUB) {
				memcpy(GET_PTR(gUI_Theme.Main.BM.Offset[i]), BITMAP_Sub, sizeof(BITMAP_Sub));
			}

		}

		if (gEeprom.VfoInfo[i].FrequencyReverse) {
			memcpy(GET_PTR(gUI_Theme.Main.BM.Reverse[i]), BITMAP_ReverseMode, sizeof(BITMAP_ReverseMode));
		}
		if (gEeprom.VfoInfo[i].CHANNEL_BANDWIDTH == BANDWIDTH_NARROW) {
			memcpy(GET_PTR(gUI_Theme.Main.BM.Narrow[i]), BITMAP_NarrowBand, sizeof(BITMAP_NarrowBand));
		}
		if (gEeprom.VfoInfo[i].DTMF_DECODING_ENABLE || gSetting_KILLED) {
			memcpy(GET_PTR(gUI_Theme.Main.BM.DTMF[i]), BITMAP_DTMF, sizeof(BITMAP_DTMF));
		}
		if (gEeprom.VfoInfo[i].SCRAMBLING_TYPE && gSetting_ScrambleEnable) {
			memcpy(GET_PTR(gUI_Theme.Main.BM.Scramble[i]), BITMAP_Scramble, sizeof(BITMAP_Scramble));
		}
	}

	ST7565_BlitFullScreen();
}

