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
#include "app/aircopy.h"
#include "driver/st7565.h"
#include "external/printf/printf.h"
#include "misc.h"
#include "radio.h"
#include "ui/aircopy.h"
#include "ui/helper.h"
#include "ui/inputbox.h"
#include "ui/theme.h"

void UI_DisplayAircopy(void)
{
	char String[16];

	memset(gFrameBuffer, 0, sizeof(gFrameBuffer));

	if (gAircopyState == AIRCOPY_READY) {
		strcpy(String, "AIR COPY(RDY)");
	} else if (gAircopyState == AIRCOPY_TRANSFER) {
		strcpy(String, "AIR COPY");
	} else {
		strcpy(String, "AIR COPY(CMP)");
	}
	UI_PrintString(String, gUI_Theme.Aircopy.Status.X0, gUI_Theme.Aircopy.Status.X1, gUI_Theme.Aircopy.Status.Y, gUI_Theme.Aircopy.Status.W, true);

	if (gInputBoxIndex == 0) {
		NUMBER_ToDigits(gRxVfo->ConfigRX.Frequency, String);
		UI_DisplayFrequency(String, gUI_Theme.Aircopy.Freq.X, gUI_Theme.Aircopy.Freq.Y, false, false);
		UI_DisplaySmallDigits(gUI_Theme.Aircopy.Digits.Count, String + 6, gUI_Theme.Aircopy.Digits.X, gUI_Theme.Aircopy.Digits.Y);
	} else {
		UI_DisplayFrequency(gInputBox, gUI_Theme.Aircopy.Freq.X, gUI_Theme.Aircopy.Freq.Y, true, false);
	}

	memset(String, 0, sizeof(String));

	if (gAirCopyIsSendMode == 0) {
		sprintf(String, "RCV:%d E:%d", gAirCopyBlockNumber, gErrorsDuringAirCopy);
	} else if (gAirCopyIsSendMode == 1) {
		sprintf(String, "SND:%d", gAirCopyBlockNumber);
	}
	UI_PrintString(String, gUI_Theme.Aircopy.Mode.X0, gUI_Theme.Aircopy.Mode.X1, gUI_Theme.Aircopy.Mode.Y, gUI_Theme.Aircopy.Mode.W, true);
	ST7565_BlitFullScreen();
}

