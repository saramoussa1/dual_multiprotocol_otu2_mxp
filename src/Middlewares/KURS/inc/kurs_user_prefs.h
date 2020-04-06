/*
 * user_settings.h
 *
 *  Created on: Mar 29, 2018
 *      Author: dcham
 */

#ifndef USER_SETTINGS_H_
#define USER_SETTINGS_H_
#include "../../../common/inc/ipg_types.h"
extern char save;

void build_preferences_list(void);
void IPGLB_WriteUserPrefsCallback(void * pSrc, size_t dwSize);
ret_code_t IPGLB_ReadUserPrefsCallback(void* pDst, size_t dwSize);
ret_code_t IPGLB_ReadLngFileCallback(void* pDst, size_t dwSize);
void IPGLB_WriteLngFileCallback(void * pSrc, size_t dwSize);
int append_preference(void* data, size_t size);
void commit_user_settings(void);
ret_code_t read_user_settings(void);

#endif /* USER_SETTINGS_H_ */
