/*
 * kurs_core.h
 *
 *  Created on: June 26, 2018
 *      Author: Clara Zaiter
 */

#ifndef KURS_CORE_H_
#define KURS_CORE_H_

#include "kurs_helper.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))		//!<returns the minimum of two values.

/*****************************************************************************/

void HandleKursRdDeviceClass(char *str_rx, char *str_tx, int *reply_len);
void HandleKursEnableSetParams(char *str_rx, char *str_tx, int *reply_len);
void HandleKursRdProfile( char *str_rx, char *str_tx, int *reply_len );
void HandleKursRdStrValueAndColor( char *str_rx, char *str_tx, int *reply_len );
void HandleKursSetParamValue( char *str_rx, char *str_tx, int *reply_len );
void HandleKursSetColorDegradeAndFailure( char *str_rx, char *str_tx, int *reply_len );
void HandleKursGetColorDegradeAndFailure( char *str_rx, char *str_tx, int *reply_len );
void HandleKursReadLangPackCommand(char *str_rx, char *str_tx, int *reply_len);
void HandleKursWriteLangPackCommand(char *str_rx, char *str_tx, int *reply_len);
void HandleKursUnknownCommand(char *str_rx, char *str_tx, int *reply_len);
void HandleKursExtendedRdDeviceClass(char *str_rx, char *str_tx, int *reply_len);
void HandleKursExtendedEnableSetParams(char *str_rx, char *str_tx, int *reply_len);
void HandleKursExtendedConfigRdWr(char *str_rx, char *str_tx, int *reply_len);
void ColorDynamicParameters(void);

#endif /* KURS_CORE_H_ */
