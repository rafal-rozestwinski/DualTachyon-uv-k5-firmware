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
#if defined(ENABLE_FMRADIO)
#include "app/fm.h"
#endif
#include "bitmaps.h"
#include "driver/keyboard.h"
#include "driver/st7565.h"
#include "functions.h"
#include "helper/battery.h"
#include "misc.h"
#include "settings.h"
#include "ui/status.h"
#include "ui/theme.h"

void UI_DisplayStatus(void)
{
	memset(gStatusLine, 0, sizeof(gStatusLine));
	if (gCurrentFunction == FUNCTION_POWER_SAVE) {
		memcpy(gStatusLine + gUI_Theme.Status.PowerSave, BITMAP_PowerSave, sizeof(BITMAP_PowerSave));
	}
	if (gBatteryDisplayLevel < 2) {
		if (gLowBatteryBlink == 1) {
			memcpy(gStatusLine + gUI_Theme.Battery.X, BITMAP_BatteryLevel1, sizeof(BITMAP_BatteryLevel1));
		}
	} else {
		if (gBatteryDisplayLevel == 2) {
			memcpy(gStatusLine + gUI_Theme.Battery.X, BITMAP_BatteryLevel2, sizeof(BITMAP_BatteryLevel2));
		} else if (gBatteryDisplayLevel == 3) {
			memcpy(gStatusLine + gUI_Theme.Battery.X, BITMAP_BatteryLevel3, sizeof(BITMAP_BatteryLevel3));
		} else if (gBatteryDisplayLevel == 4) {
			memcpy(gStatusLine + gUI_Theme.Battery.X, BITMAP_BatteryLevel4, sizeof(BITMAP_BatteryLevel4));
		} else {
			memcpy(gStatusLine + gUI_Theme.Battery.X, BITMAP_BatteryLevel5, sizeof(BITMAP_BatteryLevel5));
		}
	}
	if (gChargingWithTypeC) {
		memcpy(gStatusLine + gUI_Theme.Status.Charging, BITMAP_USB_C, sizeof(BITMAP_USB_C));
	}
	if (gEeprom.KEY_LOCK) {
		memcpy(gStatusLine + gUI_Theme.Status.KeyLock, BITMAP_KeyLock, sizeof(BITMAP_KeyLock));
	} else if (gWasFKeyPressed) {
		memcpy(gStatusLine + gUI_Theme.Status.FKey, BITMAP_F_Key, sizeof(BITMAP_F_Key));
	}

	if (gEeprom.VOX_SWITCH) {
		memcpy(gStatusLine + gUI_Theme.Status.Vox, BITMAP_VOX, sizeof(BITMAP_VOX));
	}
	if (gEeprom.CROSS_BAND_RX_TX != CROSS_BAND_OFF) {
		memcpy(gStatusLine + gUI_Theme.Status.CrossBand, BITMAP_WX, sizeof(BITMAP_WX));
	}
	if (gEeprom.DUAL_WATCH != DUAL_WATCH_OFF) {
		memcpy(gStatusLine + gUI_Theme.Status.DualWatch, BITMAP_TDR, sizeof(BITMAP_TDR));
	}
	if (gEeprom.VOICE_PROMPT != VOICE_PROMPT_OFF) {
		memcpy(gStatusLine + gUI_Theme.Status.Prompt, BITMAP_VoicePrompt, sizeof(BITMAP_VoicePrompt));
	}
	if (gSetting_KILLED) {
		memset(gStatusLine + gUI_Theme.Status.Killed, 0xFF, 10);
	}
#if defined(ENABLE_FMRADIO)
	else if (gFmRadioMode) {
		memcpy(gStatusLine + gUI_Theme.Status.FM, BITMAP_FM, sizeof(BITMAP_FM));
	}
#endif
#if defined(ENABLE_NOAA)
	if (gIsNoaaMode) {
		memcpy(gStatusLine + gUI_Theme.Status.NOAA, BITMAP_NOAA, sizeof(BITMAP_NOAA));
	}
#endif
	ST7565_BlitStatusLine();
}

