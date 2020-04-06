/****************************************************************************
 * IPG Photonics Corporation
 * COPYRIGHT (C) IPG Photonics Corporation ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 ****************************************************************************/

/**
 * @file kurs_core.c
 * @author c_zaayter
 * @date June 26, 2018
 * @brief This file contains all functions needed to handle KURS communication.
 *
 */

#include <stddef.h>
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <inttypes.h>
#include "../inc/kurs_core.h"
#include "../inc/kurs_helper.h"
#include "../inc/kurs_user_prefs.h"
#include "../inc/kurs_remote_update.h"
#include "../inc/kurs.h"

#ifdef PROF_V5
#define NBRE_SECTIONS_LESS_THAN_100		8
#else
#define NBRE_SECTIONS_LESS_THAN_100		7
#endif
typedef enum{
	FLOAT_INV = -1,
	FLOAT_EQUAL = 0,
	FLOAT_LT = 1,
	FLOAT_GT = 2,
}FLOAT_CMP_RESULT_t;

static FLOAT_CMP_RESULT_t float_compare(float a, float b, float resolution){
	float delta = a-b;
	if(fabs(delta) < resolution){
		return FLOAT_EQUAL;
	}
	if(delta > resolution){
		return FLOAT_LT;
	}
	if((-delta)>resolution){
		return FLOAT_GT;
	}
	return FLOAT_INV;
}

static int float_compare_lt(float a, float b, float resolution){
	if((b-a)> resolution){
		return 1;
	}
	return 0;
}

static int float_compare_gt(float a, float b, float resolution){
	if((a-b)> resolution){
		return 1;
	}
	return 0;
}

static int float_compare_equal(float a, float b, float resolution){
	if(fabs(a-b) < resolution){
		return 1;
	}
	return 0;
}

/*
 *  local prototypes
 */
 /* profile related functions*/
void profile_init();
void profile_set_default(uint32_t selected_sections);
void profile_save_prefs( );
/* profile parameters related functions*/
char SetStrValueItem( KURS_ProfileTypeDef *PR, profile_param_t *T, char *str, uint16_t size );
uint16_t GetStrParamValueItem( KURS_ProfileTypeDef *prof, profile_param_t *param, char *str );
uint16_t profile_userget_value(KURS_ProfileTypeDef *pr, profile_param_t *section);
char profile_userset_value( KURS_ProfileTypeDef *PR, profile_param_t *t, void *value );
/* profile helper functions*/
uint8_t get_block_color(KURS_ProfileTypeDef *PR);
/* profile coloring functions*/
char TestProg( const profile_param_t *P );
uint16_t user_get_color( profile_param_t *t, params_t2_add_t *T2_ADD );
profile_param_t* get_param_by_id(KURS_ProfileTypeDef *pr, uint32_t param_id);

/*
 * local variables
 */
uint32_t expected_flash_data_count = 0;	//!<expected count of user data in external flash
uint32_t ems_update_period_min;			//!<time interval in min used to update EMS parameters
uint32_t block_color = 0;				//!<value of the Blockcolor parameter

KURS_ProfileTypeDef profile=
{
		DEF_KURS_ADDRESS,
#ifdef PROF_V5
		"5.0",
#else
		"3.0",
#endif
		DEF_DEV_CLASS,
		DEF_DEV_PREFIX,
		2,                // number for English language
		5,
		{
			NULL,//section_1,
			NULL,//section_2,
			NULL,//section_3,
			NULL,//section_4,
			NULL,//section_5,
			NULL,//NULL,//6
			NULL,//NULL,//7
#ifdef PROF_V5
			NULL,//section_8,
#endif
			NULL,//100
			NULL,//101
			NULL,//102
			NULL,//section_103,
		},
		{
			0,//sizeof(section_1)/sizeof(profile_param_t),
			0,//sizeof(section_2)/sizeof(profile_param_t),
			0,//sizeof(section_3)/sizeof(profile_param_t),
			0,//sizeof(section_4)/sizeof(profile_param_t),
			0,//sizeof(section_5)/sizeof(profile_param_t),
			0,//6
			0,//7
#ifdef PROF_V5
			0,//sizeof(section_8)/sizeof(profile_param_t),
#endif
			0,//100
			0,//101
			0,//102
			0,//sizeof(section_103)/sizeof(profile_param_t),
		},
		1,//set_param_enabled
#ifdef PROF_V5
		{
				1,
				1,
				1,
				1,
				1,
				0,
				0,
				1,
				0,
				0,
				0,
				1,
		},
		1,//save_param_enabled
#endif
		profile_set_default,
		profile_init, // pointer to profile init function
		profile_save_prefs, // pointer to profile save function
//		NULL, // user data
//		0, // user data size
		NULL,//
		0,//PROFILE_LINES_COUNT,//PROF_LINE_COUNT,//
		NULL,
//		NULL,//LANG_PACK_OFFSET_IN_FLASH,//NULL,//(void*)(0x08060000), // pointer to flash where language packs will be stored
	};

void profile_set_default(uint32_t selected_sections){
	KURS_profile_set_default(selected_sections);
}

__weak void KURS_profile_set_default(uint32_t selected_sections){
//	UNUSED(selected_sections);
	/**
	 * DO NOT MODIFY
	 * If needed define and implement this function in your main file
	 *
	 */
}

void profile_init(){
	KURS_profile_init();
}

__weak void KURS_profile_init(){
//	UNUSED(_profile);
	/**
	 * DO NOT MODIFY
	 * If needed define and implement this function in your main file
	 *
	 */
}

__weak void commit_user_settings(void)
{

}

/**This function saves all parameters with write permission to EEPROM.
 * Section 2 colors are also saved to flash.
 *
 * @param[in,out] prof pointer to the profile instance
 * @return None
 * @since 1.0.0
 */
void profile_save_prefs( ){
	save = 1;
//	commit_user_settings();
}



/**This function handles the KURS_RD_DEVICE_CLASS command (0x01).
* @param[in] request pointer to the request string
* @param[out] reply pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursRdDeviceClass(char *str_rx, char *str_tx, int *reply_len)
{
	char *str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2; //skip the address and command locations

	str += sprintf(str, "%s\r", profile.class_);
	*reply_len = (str - str_tx);
}

/**This function handles the KURS_EN_SET_PARAMS command (0x49).
* The correct password is provided ("IPG")
* @param[in] request pointer to the request string
* @param[out] reply pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursEnableSetParams(char *str_rx, char *str_tx, int *reply_len)
{
	char *str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2; //skip the address and command locations
	str_rx += 2; // move the reply pointer 2 positions to skip address and command
				  // check whether the password is IPG or not
#ifdef PROF_V5
	profile.set_param_enabled = 1;
	str += sprintf(str, "OK\r");
#else
	if (strncmp(str_rx, "IPG", 3) != 0) // strings do not match
	{
		profile.set_param_enabled = 0;
		str += sprintf(str, "NA\r");
	}
	else
	{
		profile.set_param_enabled = 1;
		str += sprintf(str, "OK\r");
	}
#endif
	*reply_len = (str - str_tx);
}

/**This function handles the KURS KURS_RD_PROFILE_LINE command (0x73).
* @param[in] str_rx length of the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursRdProfile(char *str_rx, char *str_tx, int *reply_len){
	int line_index;
	char *str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2; //skip the address and command locations
	line_index = StrToU32Def(str_rx + 2, 0xFFFFFFFF); // get the index of the line requested,
	if (line_index == 0) // line_index 0 if EMS wants to know how many lines are available in the profile
	{
		str += sprintf(str, "%d;%d", line_index, profile.line_count);
	}else if (line_index <= profile.line_count){
		str += sprintf(str, "%d;%s", line_index, profile.text[line_index - 1]);
//		str += sprintf(str, "%d;%s", line_index, Profile_Text[line_index - 1]);
	}
	else
	{
		strcpy(str, "0;NA");
		str += strlen(str);
	}
	strcpy(str++, ";");
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}

/**This function handles the KURS_RD_PROFILE_PARAMS (0x71) and KURS_RD_PROFILE_PARAMS_WITH_COLORS (0x77) commands.
* @param[in] str_rx pointer to the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursRdStrValueAndColor( char *str_rx, char *str_tx, int *reply_len )
{
	char en_color;
	int section_number = 0, section_index = 0;
	profile_param_t *current_param = NULL;
	uint16_t param_count = 0;
	char *str = str_tx;
	uint16_t param_index = 0;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2;

	en_color = (str_rx[1] == KURS_RD_PROFILE_PARAMS_WITH_COLORS) ? 1 : 0;
	section_number = StrToU32Def(str_rx + 2, 0xFFFFFFFF);
	if(section_number == -1){ // packet with missing characters probably
		// section not found or empty
//		str+=sprintf(str, "%d;NA", section_number);
		*reply_len = 0;
		return; // return without sending any reply
	}else{
		section_index = 0;
		// validate the section number, if valid, get the corresponding section and params
		// check for sections between 1 and NBRE_SECTIONS_LESS_THAN_100 inclusive
		if ((section_number != 0) && (section_number <= NBRE_SECTIONS_LESS_THAN_100)
			&& (profile.parameters[section_number - 1] != NULL)
			&& (profile.section_param_count[section_number - 1] > 0))
		{
			section_index = section_number - 1;
		}

		// now check for sections 100 and up
		if ((section_number >= 100) && (section_number <= 103)
			&& (profile.parameters[section_number - 100 + NBRE_SECTIONS_LESS_THAN_100] != NULL)
			&& (profile.section_param_count[section_number - 100 + NBRE_SECTIONS_LESS_THAN_100] > 0))
		{
			section_index = section_number - 100 + NBRE_SECTIONS_LESS_THAN_100;
		}
		current_param = (profile_param_t *)profile.parameters[section_index];
		param_count = profile.section_param_count[section_index];
		sprintf(str, "%d", section_number);
		str += strlen(str);
		if ((current_param == NULL) || (param_count <= 0))
		{
			// section not found or empty
			strcpy(str, ";NA");
			str += strlen(str);
		}
		else
		{
			// read parameters with no color
			if ((section_number != 2) || (!en_color))
			{
				param_index = 0;
				// iterate through all the parameters and get their values
				for (param_index = 0; param_index < param_count; param_index++, current_param++)
				{
					strcpy(str++, ";");
					if ((current_param->user_get) && (profile_userget_value(&profile, current_param) > 0))
					{
						// I don't think we will ever get here because profile_userget_value always return 0
					}
					str += GetStrParamValueItem(&profile, current_param, str);
	//				else
	//				{
	//
	//				}
				}
			}
			else // section 2 or colored params
			{
				params_t2_add_t *param_t2;
				param_index = 0;
				// iterate through all the parameters and get their values
				for (param_index = 0; param_index < param_count; param_index++, current_param++)
				{
					param_t2 = (params_t2_add_t *)current_param->d_add;
					strcpy(str++, ";");
					if ((current_param->user_get) && (profile_userget_value(&profile, current_param) > 0))
					{
					}
					str += GetStrParamValueItem(&profile, current_param, str);
					if ((param_t2 != NULL) && param_t2->COLOR != COLOR_NA)
					{
						//str += sprintf(str, "@%d", TestProg(current_param));
						if (param_t2->COLOR_Tip == COLOR_TIP_POROG)
						{
							str += sprintf(str, "@%d", TestProg(current_param));
						}
						else if (param_t2->COLOR_Tip == COLOR_TIP_GET)
						{
							str += sprintf(str, "@%d", user_get_color(current_param, param_t2));
						}
						else
						{
							str += sprintf(str, "@%d", param_t2->COLOR);
						}
					}
				}
			}
		}
	}
	strcpy(str++, ";");
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}

/**This function handles the KURS_RD_PROFILE_PARAMS (0x72) and KURS_RD_PROFILE_PARAMS_WITH_COLORS (0x76) commands.
* @param[in] str_rx pointer to the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursSetParamValue(char *str_rx, char *str_tx, int *reply_len){
	//printf("Set param request %s\n", str_rx);
	uint32_t crc_match = 0, crc_read = 0;
	uint16_t table = 0;
	uint16_t item = 0;
	uint16_t size_data = 0;
	char *data_ = NULL;
	char *uk = NULL;
	profile_param_t *T_UK = NULL;
	uint16_t T_COUNT = 0;
	char data_error = 0;
	char *str = str_tx;
	char *uk_crc = strrchr((char *)str_rx, ';'); // get the last ";" (followed by the CRC byte)
	if ((uk_crc != 0))
	{
		uk_crc++; // move to the crc location
		crc_read = StrToU32Def(uk_crc, 0xFFFFFFFF); /*this is the CRC that was received*/
		crc_match = get_crc16_ascii((char *)str_rx, uk_crc - str_rx); /* this is the CRC we calculated*/
		if (crc_read != crc_match)
		{
			data_error = 1;
		} /*they should match*/
	}
	else // no ";" found
	{
		data_error = 1;
	}
	if ((data_error)
		|| (StrToU16((char *)str_rx + 2, &table) == 0)) // try to validate the table index we received
	{
		data_error = 1;
	}
	if ((data_error)
		|| ((uk = (char *)strchr((char *)str_rx, ';')) == NULL) // move to the next field (fields are seperated by ";")
		|| (StrToU16((char *)uk + 1, &item) == 0)) // try to parse the parameter index we recieved
	{
		data_error = 1;
	}
	if ((data_error)
		|| (uk = (char *)strchr((char *)uk + 1, ';')) == NULL) // move to the next field
	{
		data_error = 1;
	}
	if (data_error == 0)
	{
		//printf("data error is false\n");
		data_ = (char *)uk + 1; // move to the location where the data is in the string
		if ((table) && (table != 2) && (table <= NBRE_SECTIONS_LESS_THAN_100) // make sure the table index is valid
			&& (profile.parameters[table - 1] != NULL) // make sure the section has an entry in the profile structure
			&& (profile.section_param_count[table - 1] > 0)) // and that this table has parameters
		{
			T_UK = (profile_param_t *)profile.parameters[table - 1]; // get a pointer to the parameters of this table
			T_COUNT = profile.section_param_count[table - 1];                // and the number of parameters
		}
		else if ((100 <= table) && (table <= 103)                                                                // in case the table index is between 100 and 103 inclusive
				 && (profile.parameters[table - 100 + NBRE_SECTIONS_LESS_THAN_100] != NULL)                 // make sure the section has an entry in the profile structure
				 && (profile.section_param_count[table - 100 + NBRE_SECTIONS_LESS_THAN_100] > 0))         // and that this table has parameters
		{
			T_UK = (profile_param_t *)profile.parameters[table - 100 + NBRE_SECTIONS_LESS_THAN_100]; // get a pointer to the parameters of this table
			T_COUNT = profile.section_param_count[table - 100 + NBRE_SECTIONS_LESS_THAN_100];                  // and the number of parameters
		}

		if ((T_UK != NULL)
			&& ((T_UK += item - 1) != NULL)) // move the parameter pointer to the item we want
		{

			if (T_UK->type == DC_STR || T_UK->type == DC_OID)
			{

				if ((uk = (char *)strchr((char *)uk + 1, ';')) == NULL) // move to the last ";" if it exists
				{
					data_error = 1;
				}
				// the value (which is a string in this case) is size_data long
				size_data = (char *)uk - data_;
			}
			else
			{
				size_data = T_UK->d_size;
			}
		}
	}
	if ((data_error == 0)
		&& (profile.set_param_enabled) //make sure the EMS has enabled paramter writing
		&& (T_UK != NULL)
		&& (item)
		&& (item <= T_COUNT)
		&& (((str_rx[1] == KURS_SET_PROFILE_PARAMS) && (table != 6))                        // section 6 params have to be written using 0x76
			|| ((str_rx[1] == KURS_SET_PROFILE_CALIB_PARAMS) && (table == 6)))
		&& ((T_UK->access & 0xAA) != 0)        // make sure the param is not read only
		&& (SetStrValueItem(&profile, T_UK, data_, size_data)))
	{
		// copy the first two bytes (address and command) from the request to the reply string
		*str = *str_rx;
		*(str + 1) = *(str_rx + 1);
		str += 2; //keep the address and command

		str += sprintf((char *)str, "%d;%d;", table, item); // copy the table index and parameter index
		if ((T_UK->user_get) && (profile_userget_value(&profile, T_UK) > 0))
		{
//			str += size_data;
		}
//		else
		{
			str += GetStrParamValueItem(&profile, T_UK, str);
		}
		*str++ = ';';
		*str = 0;
		profile.Save(&profile);
	}
	else
	{
		if ((str_rx[1] == KURS_SET_PROFILE_CALIB_PARAMS) && (table == 6))
		{}
		// copy the first two bytes (address and command) from the request to the reply string
		*str = *str_rx;
		*(str + 1) = *(str_rx + 1);
		str += 2;
		str += sprintf((char *)str, "%d;%d;NA;", table, item);
	}
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));

	*reply_len = (str - str_tx);
}

/**This function handles the KURS_SET_COLOR_DEGRADE_AND_FAILURE command (0x7C).
* @param[in] str_rx pointer to the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursSetColorDegradeAndFailure( char *str_rx, char *str_tx, int *reply_len )
{
	uint16_t table = 0;
	uint16_t item = 0;
	uint16_t colo1; //Degrade
	uint16_t colo2; //Failure
	char *uk = NULL;
	profile_param_t *T_UK = (profile_param_t *)profile.parameters[1]; //params of section 2
	uint16_t T_COUNT = profile.section_param_count[1];
	char data_error = 0;
	char *str = str_tx;
	char *uk_crc = strrchr(str_rx, ';'); // look for the last occurrence of ";"
	uint32_t crc_read = 0, crc_match = 0;
	if ((uk_crc != 0))
	{
		uk_crc++; // move one position to get to the CRC
		crc_read = StrToU32Def(uk_crc, 0xFFFFFFFF); // CRC that we received
		crc_match = get_crc16_ascii((char *)str_rx, uk_crc - str_rx); // CRC that we calculated
		if (crc_read != crc_match) // they should match
		{
			data_error = 1;
		}
	}
	else // ";" last occurrence not found
	{
		data_error = 1;
	}
	if (StrToU16((char *)str_rx + 2, &table) == false) // try to find the table index
	{
		data_error = 1;
	}
	if ((data_error)
		|| ((uk = (char *)strchr((char *)str_rx, ';')) == NULL) // find the next ";"
		|| (StrToU16((char *)uk + 1, &item) == 0)) //move one position and try to parse the parameter number
	{
		data_error = 1;
	}
	if ((data_error)
		|| ((uk = (char *)strchr((char *)uk + 1, ';')) == NULL) // find the next ";"
		|| (StrToU16((char *)uk + 1, &colo1) == 0))
	{
		data_error = 1; // colo1 - Degrade
	}
	if ((data_error)
		|| ((uk = (char *)strchr((char *)uk + 1, ';')) == NULL) // find the next ";"
		|| (StrToU16((char *)uk + 1, &colo2) == 0)) data_error = 1; // colo2 - Failure
	if ((data_error == 0)
		&& (profile.set_param_enabled)
		&& (table == 2)
		&& (item)
		&& (item <= T_COUNT)
		&& ((T_UK += item - 1) != NULL)) // move the params pointer to the item we need
	{
		if ((((params_t2_add_t *)T_UK->d_add)->COLOR_Degrade != COLOR_NA)
			&& (colo1 != COLOR_NA)
			&& (colo1 <= COLOR_Critical)) ((params_t2_add_t *)T_UK->d_add)->COLOR_Degrade = colo1;
		if ((((params_t2_add_t *)T_UK->d_add)->COLOR_Failure != COLOR_NA)
			&& (colo2 != COLOR_NA) && (colo2 <= COLOR_Critical)) ((params_t2_add_t *)T_UK->d_add)->COLOR_Failure = colo2;
		*(uint16_t *)str = *(uint16_t *)str_rx;
		str += 2;
		str += sprintf((char *)str, "%d;%d;%d;%d;", table, item, ((params_t2_add_t *)T_UK->d_add)->COLOR_Degrade, ((params_t2_add_t *)T_UK->d_add)->COLOR_Failure);
		profile.Save(&profile);
	}
	else
	{
		// copy the first two bytes (address and command) from the request to the reply string
		*str = *str_rx;
		*(str + 1) = *(str_rx + 1);
		str += 2;
		str += sprintf((char *)str, "%d;%d;NA;", table, item);
	}
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}

/**This function handles the KURS_GET_COLOR_DEGRADE_AND_FAILURE command (0x7D).
* @param[in] str_rx pointer to the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursGetColorDegradeAndFailure(char *str_rx, char *str_tx, int *reply_len){
	uint16_t parameter_count = 0;
	int i = 0;
	uint32_t table = 0;
	char *str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2;

	table = StrToU32Def(str_rx + 2, 0xFFFFFFFF);

	str += sprintf(str, "%d", (int)table);
	if (table == 2)
	{
		profile_param_t *T_UK = (profile_param_t *)profile.parameters[1]; //pointer to table 2 params
		parameter_count = profile.section_param_count[1];

		for (i = 0; i < parameter_count; i++, T_UK++)
		{
			if((params_t2_add_t *)T_UK->d_add == NULL)
			{
				str += sprintf(str, ";0@0");
			}
			else
			{
				str += sprintf(str, ";%d@%d", ((params_t2_add_t *)T_UK->d_add)->COLOR_Degrade, ((params_t2_add_t *)T_UK->d_add)->COLOR_Failure);
			}
		}
	}
	else
	{
		strcpy(str, ";NA");
		str += strlen(str);
	}
	strcpy(str++, ";");
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}

/**This function handles the KURS_WRITE_LANGUAGE_PACK command (0x7A).
 *
* @param[in] str_rx pointer to the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
 */
void HandleKursWriteLangPackCommand(char *str_rx, char *str_tx, int *reply_len){
	char* str = str_tx;
	char*   block_data;
	uint32_t  block_index;
	uint32_t  block_length;
	static uint32_t expected_blck_cnt = 0;

	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2;

	block_index = StrToU32Def(str_rx + 2, 0xFFFFFFFF);   // extract the block index
	block_data = strstr(str_rx, ";");       // get a pointer to the language data

	if( 	(block_index < LANG_BLOCK_MAX_CNT) &&
			(profile.set_param_enabled) &&
			(block_data != NULL)){
		block_data++; // move past the ';'
		//   block_length = strstr(block_data, '\0') - block_data; // get the length of the language data
		block_length = strlen(block_data);// get the length of the language data block
		if(block_length == LANG_BLOCK_SIZE){
			if(block_index == 0){
				expected_blck_cnt = StrToU32Def(block_data, 0xFFFFFFFF);
				memset(profile.lngpack, 0xFF, sizeof(LANG_BLOCK_SIZE * LANG_BLOCK_MAX_CNT));
			}else if(block_index == expected_blck_cnt){
				KURS_NewLngpackCallback();
			}

			memcpy(&profile.lngpack[block_index][0], block_data, LANG_BLOCK_SIZE);
			str += sprintf(str, "%d;OK;", (int)block_index);
		}else{
			str += sprintf(str, "%d;NA;", (int)block_index);
		}
	}else{
		if(block_index == 0xFFFFFFFF)block_index = 0;
		str += sprintf(str, "%d;NA;", (int)block_index);
	}
	str += sprintf(str, "%d\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len =  str - str_tx;
}

__weak void KURS_NewLngpackCallback(void){
	/*
	 * DO NOT MODIFY
	 * Implemen in main application
	 */
}

/**This function handles the KURS_READ_LANGUAGE_PACK command (0x7B).
 *
* @param[in] str_rx pointer to the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
 */
void HandleKursReadLangPackCommand(char *str_rx, char *str_tx, int *reply_len){
	char* str = str_tx;
	unsigned int block_index;

	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2;

	block_index = StrToU32Def(str_rx + 2, 0xFFFFFFFF);			// extract the block index

	str+= sprintf(str, "%u;", block_index);
	if(block_index < LANG_BLOCK_MAX_CNT)
	{
		char* flash_src = &profile.lngpack[block_index][0];
//		char* flash_src = ponder_profile.lang + (block_index * LANG_BLOCK_SIZE);
		int data_cnt = 0;
		for(data_cnt = 0; data_cnt < LANG_BLOCK_SIZE; data_cnt++, flash_src++, str++)
		{
			if(block_index && (*flash_src == ';'))
			{
				*str = 0x02;
			}
			else if (*flash_src == '\r')
			{
				*str = 0x01;
			}
			else if(*flash_src == 0)
			{
				*str = 0x03;
			}
			else
			{
				*str = *flash_src;
			}
		}
		memset(str, 0x03, (256-data_cnt));
		str += (256 - data_cnt);
	}
	else
	{
		strcpy(str, "NA");
		str += strlen(str);
	}
	strcpy(str++, ";");
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}

/**This function handles an unknown KURS command.
* @param[in] request pointer to the request string
* @param[out] reply pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursUnknownCommand(char *str_rx, char *str_tx, int *reply_len)
{
	char *str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2; //skip the address and command locations

	str += sprintf(str, "NA\r");
	*reply_len = (str - str_tx);
}

#ifdef PROF_V5
/**This function handles the KURS_EXTENDED_DEVICE_CLASS command (0x61).
* @param[in] request pointer to the request string
* @param[out] reply pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
*/
void HandleKursExtendedRdDeviceClass(char *str_rx, char *str_tx, int *reply_len)
{
	char *str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2; //skip the address and command locations
	str += sprintf(str, "%s;", profile.class_);
	//If the values of the parameters of the sections have changed,
	//the reply must be <0x80+Address><0x61>DevClass;Set;Save;Table1;....;TableN;CRC<0x0D>
	//If the values of the parameters of the sections have not changed,
	//the reply must be <0x80+Address><0x61>DevClass;Set;Save;CRC<0x0D>
	if(profile.set_param_enabled)
	{
		str += sprintf(str, "OK;");
	}
	else
	{
		str += sprintf(str, "NA;");
	}
	if(profile.save_param_enabled)
	{
		str += sprintf(str, "OK;");
	}
	else
	{
		str += sprintf(str, "NA;");
	}
	//TODO: Where to use this change flag?
	int table_index = 0;
	for(table_index = 0; table_index < PROFILE_SECTION_COUNT; table_index++)
	{
		if(profile.section_param_changed[table_index])
		{
			if(table_index <= 7)
			{
				str += sprintf(str, "%d;", (table_index+1));
			}
			else
			{
				str += sprintf(str, "%d;", (91 + table_index + 1)); //index 8 => section 100; 9 => 101; 10 => 102; 11 => 103
			}
		}
	}
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}

/**This function handles the KURS_EXTENDED_EN_SET_PARAMS command (0x62).
* @param[in] request pointer to the request string
* @param[out] reply pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursExtendedEnableSetParams(char *str_rx, char *str_tx, int *reply_len)
{
	char* str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2; //skip the address and command locations
	//the reply must be <0x80+Address><0x62>"OK";CRC<0x0D>
	//and in case we did not get the correct command, it must be <0x80+Address><0x62>"NA";CRC<0x0D>
	str_rx += 2; // move the reply pointer 2 positions to skip address and command
	//Parse the request to update set and save param enabled flags
//	if (strncmp(str_rx, "OK", 2) != 0) // strings do not match
//	{
//		profile.set_param_enabled = 0;
//	}
//	str_rx += 3; // move the reply pointer 3 positions to skip "Set" flag
//	if (strncmp(str_rx, "OK", 2) != 0) // strings do not match
//	{
//		profile.save_param_enabled = 0;
//	}
	str += sprintf(str, "OK;");
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}

/**This function handles the KURS_EXTENDED_CONFIG_RD_WR command (0x63).
* @param[in] request pointer to the request string
* @param[out] reply pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.1
*/
void HandleKursExtendedConfigRdWr(char *str_rx, char *str_tx, int *reply_len)
{
	char* str = str_tx;
	// copy the first two bytes (address and command) from the request to the reply string
	*str = *str_rx;
	*(str + 1) = *(str_rx + 1);
	str += 2; //skip the address and command locations
	//the reply must be <0x80+Address><0x62>"OK";CRC<0x0D>
	//and in case we did not get the correct command, it must be <0x80+Address><0x62>"NA";CRC<0x0D>
	str_rx += 2; // move the reply pointer 2 positions to skip address and command
	//Parse the request to update set and save param enabled flags
	if (strncmp(str_rx, "STARTREAD", 9) == 0) // strings match
	{
		strcpy( str, "OK;STARTREAD;0;");
		str += strlen( "OK;STARTREAD;0;" );
	}
	if (strncmp(str_rx, "READBLOCK", 9) == 0) // strings match
	{
		int block_index;
		block_index = StrToU32Def(str_rx + 10, 0xFFFFFFFF); // get the index of the line requested,
		strcpy( str, "NA;READBLOCK;");
		str += strlen( "NA;READBLOCK;" );
		str += sprintf(str, "%d;", block_index);

	}
	if (strncmp(str_rx, "FINISHREAD", 10) == 0) // strings match
	{
			strcpy( str, "NA;FINISHREAD;");
		str += strlen( "NA;FINISHREAD;" );
	}
	if (strncmp(str_rx, "STARTWRITE", 10) == 0) // strings match
	{
		strcpy( str, "OK;STARTWRITE;0;");
		str += strlen( "OK;STARTWRITE;0;" );
	}
	if (strncmp(str_rx, "WRITEBLOCK", 11) == 0) // strings match
	{
		int block_index;
		block_index = StrToU32Def(str_rx + 10, 0xFFFFFFFF); // get the index of the line requested,
		strcpy( str, "NA;WRITEBLOCK;");
		str += strlen( "NA;WRITEBLOCK;" );
		str += sprintf(str, "%d;", block_index);

	}
	if (strncmp(str_rx, "FINISHWRITE", 11) == 0) // strings match
	{
		strcpy( str, "NA;FINISHWRITE;1;");
		str += strlen( "NA;FINISHWRITE;1;" );
	}
	str += sprintf(str, "%u\r", get_crc16_ascii(str_tx, str - str_tx));
	*reply_len = (str - str_tx);
}
#endif

/**This function handles the KURS_SAFE_REPROGRAMMING command (0x0F).
* @param[in] str_rx pointer to the request string
* @param[out] str_tx pointer to the reply string
* @param[out] reply_len length of the reply string
* @return None
* @since 0.0.7
*/


__weak void KURS_RebootDelayed(){
	/*
	 * DO NOT MODIFY
	 * Implement in main application
	 */
}

/**This function converts the value of a parameter into an ASCII string.
 * This function uses the \c type of the \c profile_param_t argument
 * to choose which conversion function to call.
 * @param prof pointer to the profile instance
 * @param param pointer to the parameter to be converted
 * @param str the resulting string
 * @return length of the converted string
 * @since 0.0.1
*/
uint16_t GetStrParamValueItem( KURS_ProfileTypeDef *prof, profile_param_t *param, char *str )
{
	char *format_in = (char *)param->format;
	void *data_ = (void *)param->data;
	uint16_t str_len = 0;

	if ((str == NULL) || (data_ == NULL)) return str_len;
	switch (param->type)
	{
		case DC_U64:
//		   str_len += sprintf(str, "%llu" ,*(uint64_t *)data_);
		   str_len += sprintf(str, "%"PRIu64 ,*(uint64_t *)data_);
		   break;
	    case DC_BOOL:
	    case DC_U32:
	    case DC_ENUM:
	    case DC_BITMSK:
		   if (format_in != NULL)
		   {
			   str_len += sprintf(str, format_in, *(uint32_t *)data_);
		   }
		   else
		   {
			   str_len += sprintf_u32(str, *(uint32_t *)data_);
		   }
		   break;
	   case DC_S32:
		   {
			   str_len += sprintf_s32(str, *(uint32_t *)data_);
		   }
		   break;
	   case DC_HEX:
		   {
			   str_len += sprintf(str, (format_in != NULL) ? format_in : "%.8x",*(int *)data_);
		   }
		   break;
	   case DC_OID:
	   case DC_STR:
		   {
			   uint16_t size = param->d_size;
			   strncpy(str, (char *)data_, size);
			   str[size - 1] = 0;
			   str_len += strlen(str);
		   }
		   break;
	   case DC_FLOAT:
		   if (format_in != NULL)
		   {
			   str_len += sprintf(str, format_in, *(float*) data_);
		   }
		   else
		   {
			   str_len += sprintf(str, "%f", *(float*) data_);
		   }
		   break;
	   case DC_IP:
		   {
			   // IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3
			   str_len += sprintf(str, (format_in != NULL) ? format_in : "%u.%u.%u.%u",
									  ((uint8_t*) data_)[3], ((uint8_t*) data_)[2], ((uint8_t*) data_)[1],
									  ((uint8_t*) data_)[0]);
		   }
		   break;
	   case DC_INC:
		   break;
	}
	return str_len;
}

/**This function gets the color of a parameter.
 *
 * @param[in,out] ptrP	pointer to the parameter to color
 * @return the color of the parameter
 * @since 0.0.1
 */
char TestProg(const profile_param_t* ptrP){
#ifdef PROF_V5
	profile_param_t *P = (profile_param_t*) ptrP;
	int status;
	params_t2_add_t *P_ADD;
	char COLOR = COLOR_Normal;
	if (P == NULL) return 0;
	P_ADD = (params_t2_add_t *)P->d_add;
	status = STATUS_NORM;
	if(*(P_ADD->monitor_on) == 0){
		P_ADD->COLOR = COLOR_NO;
		return P_ADD->COLOR;
	}else if(P_ADD->COLOR_Tip == COLOR_TIP_GET){
		user_get_color(P, P_ADD);
		return P_ADD->COLOR;
	}
	switch (P->type)
	{
	   case DC_ENUM:
	   case DC_BITMSK:
		   // enums should be handled outside manually
		   break;
	   case DC_BOOL:
		   {
			   uint32_t data_ = *(uint32_t *)P->data;
			   if (data_ == 0) {
				   status = STATUS_NORM;
			   }else{
				   /* in case of booleans, one of degrade or failure should be COLOR_NA*/
				   // is it a degrade?
				   if (P_ADD->COLOR_Degrade != COLOR_NA){
					   status = STATUS_WMax; //consider it as a warning
				   }else if (P_ADD->COLOR_Failure != COLOR_NA){ // or a failure?
					   status = STATUS_CMax; //consider it as a critical error
				   }else{
					   status = STATUS_NORM;
				   }
			   }
		   }
		   break;
	   case DC_U32:
	   case DC_HEX:
	   case DC_INC:
		   {
			   uint32_t data_ = *(uint32_t *)P->data;
			   if ((P_ADD->CMax != NULL) && (data_ >= (*(uint32_t *)P_ADD->CMax))) status = STATUS_CMax;
			   else if ((P_ADD->CMin != NULL) && (data_ <= (*(uint32_t *)P_ADD->CMin))) status = STATUS_CMin;
			   else if ((P_ADD->WMax != NULL) && (data_ >= (*(uint32_t *)P_ADD->WMax))) status = STATUS_WMax;
			   else if ((P_ADD->WMin != NULL) && (data_ <= (*(uint32_t *)P_ADD->WMin))) status = STATUS_WMin;
			   else status = STATUS_NORM;
		   }
		   break;
	   case DC_S32:
		   {
			   int32_t data_ = *(int32_t *)P->data;
			   if ((P_ADD->CMax != NULL) && (data_ >= (*(int32_t *)P_ADD->CMax))) status = STATUS_CMax;
			   else if ((P_ADD->CMin != NULL) && (data_ <= (*(int32_t *)P_ADD->CMin))) status = STATUS_CMin;
			   else if ((P_ADD->WMax != NULL) && (data_ >= (*(int32_t *)P_ADD->WMax))) status = STATUS_WMax;
			   else if ((P_ADD->WMin != NULL) && (data_ <= (*(int32_t *)P_ADD->WMin))) status = STATUS_WMin;
			   else status = STATUS_NORM;
		   }
		   break;
	   case DC_FLOAT:
		   {
			   float fdata = *(float*) P->data;
			   //It is not correct to compare float values, that's why we are using float_compare functions instead
			   if ((P_ADD->CMax != NULL) && (float_compare_gt(fdata, (*(float*) P_ADD->CMax), 0.00001)))
				   status = STATUS_CMax;
			   else if ((P_ADD->CMin != NULL) && (float_compare_lt(fdata, (*(float*) P_ADD->CMin), 0.00001)))
				   status = STATUS_CMin;
			   else if ((P_ADD->WMax != NULL) && (float_compare_gt(fdata, (*(float*) P_ADD->WMax), 0.00001)))
				   status = STATUS_WMax;
			   else if ((P_ADD->WMin != NULL) && (float_compare_lt(fdata, (*(float*) P_ADD->WMin), 0.00001)))
				   status = STATUS_WMin;
			   else
				   status = STATUS_NORM;
//			   if ((P_ADD->CMax != NULL) && (fdata >= (*(float*) P_ADD->CMax)))
//				   status = STATUS_CMax;
//			   else if ((P_ADD->CMin != NULL) && (fdata <= (*(float*) P_ADD->CMin)))
//				   status = STATUS_CMin;
//			   else if ((P_ADD->WMax != NULL) && (fdata >= (*(float*) P_ADD->WMax)))
//				   status = STATUS_WMax;
//			   else if ((P_ADD->WMin != NULL) && (fdata <= (*(float*) P_ADD->WMin)))
//				   status = STATUS_WMin;
//			   else
//				   status = STATUS_NORM;
		   }
		   break;
	}
	if(P_ADD->COLOR == COLOR_NA){
		return COLOR_NA;
	}
	switch (status){
	   case STATUS_CMax:
	   case STATUS_CMin:
		   COLOR = P_ADD->COLOR_Failure;
		   break;
	   case STATUS_WMax:
	   case STATUS_WMin:
		   COLOR = P_ADD->COLOR_Degrade;
		   break;
	}
	P_ADD->COLOR = COLOR;
	if (P_ADD->status != status){
		P_ADD->status = status;
	}
	return COLOR;
#else
	profile_param_t *P = (profile_param_t*) ptrP;
	int status;
	params_t2_add_t *P_ADD;
	char COLOR = COLOR_Normal;
	if (P == NULL) return 0;
	P_ADD = (params_t2_add_t *)P->d_add;
	status = STATUS_NORM;
	if(*(P_ADD->monitor_on) == 0){
		P_ADD->COLOR = COLOR_NO;
		return P_ADD->COLOR;
	}else if(P_ADD->COLOR_Tip == COLOR_TIP_GET){
		user_get_color(P, P_ADD);
		return P_ADD->COLOR;
	}
	switch (P->type)
	{
	   case DC_ENUM:
	   case DC_BITMSK:
		   // enums should be handled outside manually
		   break;
	   case DC_BOOL:
		   {
			   uint32_t data_ = *(uint32_t *)P->data;
			   if (data_ == 0) {
				   status = STATUS_NORM;
			   }else{
				   /* in case of booleans, one of degrade or failure should be COLOR_NA*/
				   // is it a degrade?
				   if (P_ADD->COLOR_Degrade != COLOR_NA){
					   status = STATUS_WMax_CMax; //consider it as a warning
				   }else if (P_ADD->COLOR_Failure != COLOR_NA){ // or a failure?
					   status = STATUS_CMax_HMax; //consider it as a critical error
				   }else{
					   status = STATUS_NORM;
				   }
			   }
		   }
		   break;
	   case DC_U32:
	   case DC_HEX:
	   case DC_INC:
		   {
			   uint32_t data_ = *(uint32_t *)P->data;
			   if ((P_ADD->HMax != NULL) && (data_ >= (*(uint32_t *)P_ADD->HMax))) status = STATUS_HMax;
			   else if ((P_ADD->HMin != NULL) && (data_ <= (*(uint32_t *)P_ADD->HMin))) status = STATUS_HMin;
			   else if ((P_ADD->CMax != NULL) && (data_ >= (*(uint32_t *)P_ADD->CMax))) status = STATUS_CMax_HMax;
			   else if ((P_ADD->CMin != NULL) && (data_ <= (*(uint32_t *)P_ADD->CMin))) status = STATUS_HMin_CMin;
			   else if ((P_ADD->WMax != NULL) && (data_ >= (*(uint32_t *)P_ADD->WMax))) status = STATUS_WMax_CMax;
			   else if ((P_ADD->WMin != NULL) && (data_ <= (*(uint32_t *)P_ADD->WMin))) status = STATUS_CMin_WMin;
			   else status = STATUS_NORM;
		   }
		   break;
	   case DC_S32:
		   {
			   int32_t data_ = *(int32_t *)P->data;
			   if ((P_ADD->HMax != NULL) && (data_ >= (*(int32_t *)P_ADD->HMax))) status = STATUS_HMax;
			   else if ((P_ADD->HMin != NULL) && (data_ <= (*(int32_t *)P_ADD->HMin))) status = STATUS_HMin;
			   else if ((P_ADD->CMax != NULL) && (data_ >= (*(int32_t *)P_ADD->CMax))) status = STATUS_CMax_HMax;
			   else if ((P_ADD->CMin != NULL) && (data_ <= (*(int32_t *)P_ADD->CMin))) status = STATUS_HMin_CMin;
			   else if ((P_ADD->WMax != NULL) && (data_ >= (*(int32_t *)P_ADD->WMax))) status = STATUS_WMax_CMax;
			   else if ((P_ADD->WMin != NULL) && (data_ <= (*(int32_t *)P_ADD->WMin))) status = STATUS_CMin_WMin;
			   else status = STATUS_NORM;
		   }
		   break;
	   case DC_FLOAT:
		   {
			   float fdata = *(float*) P->data;
//			   sprintf_float(ctm, *(float*) P->data, 10);
//			   sscanf(ctm, "%f", &fdata);

			   if ((P_ADD->HMax != NULL) && (fdata >= (*(float*) P_ADD->HMax)))
				   status = STATUS_HMax;
			   else if ((P_ADD->HMin != NULL) && (fdata <= (*(float*) P_ADD->HMin)))
				   status = STATUS_HMin;
			   else if ((P_ADD->CMax != NULL) && (fdata >= (*(float*) P_ADD->CMax)))
				   status = STATUS_CMax_HMax;
			   else if ((P_ADD->CMin != NULL) && (fdata <= (*(float*) P_ADD->CMin)))
				   status = STATUS_HMin_CMin;
			   else if ((P_ADD->WMax != NULL) && (fdata >= (*(float*) P_ADD->WMax)))
				   status = STATUS_WMax_CMax;
			   else if ((P_ADD->WMin != NULL) && (fdata <= (*(float*) P_ADD->WMin)))
				   status = STATUS_CMin_WMin;
			   else
				   status = STATUS_NORM;
		   }
		   break;
	}
	if(P_ADD->COLOR == COLOR_NA){
		return COLOR_NA;
	}
	switch (status){
	   case STATUS_HMax:
	   case STATUS_CMax_HMax:
	   case STATUS_HMin_CMin:
	   case STATUS_HMin:
		   COLOR = P_ADD->COLOR_Failure;
		   break;
	   case STATUS_WMax_CMax:
	   case STATUS_CMin_WMin:
		   COLOR = P_ADD->COLOR_Degrade;
		   break;
	}
	P_ADD->COLOR = COLOR;
	if (P_ADD->status != status){
		P_ADD->status = status;
	}
	return COLOR;
#endif
}

/**This function sets the value of a parameter.
 * The value is extracted and converted from a string.
 *
 * @param[in] PR	pointer to the profile instance
 * @param[in,out] T	pointer to the parameter
 * @param str		pointer to the string to convert
 * @param size		length of the string
 * @return 1 on sucess
 * @since 0.0.1
 */
char SetStrValueItem(KURS_ProfileTypeDef *PR, profile_param_t *T, char *str, uint16_t size){
	if((str == NULL) || (T == NULL)){
		return 0;
	}
	switch (T->type){
	   case DC_U32:
		   {
			   uint32_t tm;
			   if (StrToU32(str, &tm) == 0) return 0;
			   if (((T->d_min != NULL) && (tm < (*(uint32_t *)(T->d_min))))
				   || ((T->d_max != NULL) && (*(uint32_t *)(T->d_max) < tm))
				   || ((T->user_set) && (profile_userset_value(PR, T, &tm) == 0))) return 0;
			   *(uint32_t *)T->data = tm;
		   }
		   break;
	   case DC_BOOL:
	   case DC_ENUM:
		   {
			   uint32_t tm;
			   if (StrToU32(str, &tm) == 0) return 0;
			   if (((T->d_min != NULL) && (tm < (*(uint32_t *)(T->d_min))))
				   || ((T->d_max != NULL) && (*(uint32_t *)(T->d_max) < tm))
				   || ((T->user_set) && (profile_userset_value(PR, T, &tm) == 0))) return 0;
			   *(uint32_t *)T->data = tm;
		   }
		   break;
	   case DC_S32:
		   {
			   long tm = atol(str);
			   if (((T->d_min != NULL) && (tm < (*(long *)(T->d_min))))
				   || ((T->d_max != NULL) && (*(long *)(T->d_max) < tm))
				   || ((T->user_set) && (profile_userset_value(PR, T, &tm) == 0))) return 0;
			   *(long *)T->data = tm;
		   }
		   break;
	   case DC_HEX:
		   {
			   uint32_t tm;
			   if (StrHexToU32(str, &tm) == 0) return 0;
			   if (((T->d_min != NULL) && (tm < (*(uint32_t *)(T->d_min))))
				   || ((T->d_max != NULL) && (*(uint32_t *)(T->d_max) < tm))
				   || ((T->user_set) && (profile_userset_value(PR, T, &tm) == 0))) return false;
			   *(uint32_t *)T->data = tm;
		   }
		   break;
	   case DC_OID:
	   case DC_STR:
		   {
			   //printf("copy %d bytes", min(T->d_size-1,size));
			   memset(T->data, 0, T->d_size);
			   strncpy((char *)T->data, str, min(T->d_size - 1, size));
			   ((char *)T->data)[min(T->d_size - 1, size)] = 0; // terminate the string with a 0
			   	  if((T->user_set) && (profile_userset_value(PR, T, T->data) == 0)){
			   		  return false;
			   	  }
		   }
		   break;
	   case DC_FLOAT:
		   {
			   float tm;
			   if (StrToFloat(str, &tm, ';') == false)
			 		return false;

			   if (((T->d_min != NULL) && float_compare_lt(tm,(*(float*) (T->d_min)), 0.00001))//(tm < (*(float*) (T->d_min))))
				   || ((T->d_max != NULL) && float_compare_lt(*(float*) (T->d_max) , tm, 0.00001))//(*(float*) (T->d_max) < tm))
				   || ((T->user_set) && (profile_userset_value(PR, T, &tm) == false)))
				   return false;
			   *(float*) T->data = tm;
		   }
		   break;
	   case DC_IP:
		   {
			   unsigned int tmi[4];
			   //TODO: return those lines!!
//				   if ((sscanf(str, "%u.%u.%u.%u;", tmi + 0, tmi + 1, tmi + 2, tmi + 3)
//						== 4) && (tmi[0] <= 255) && (tmi[1] <= 255) && (tmi[2] <= 255)
//					   && (tmi[3] <= 255))
			   	   if(1)
				   {
					   tmi[0] = ((*(tmi + 0) & 0x0FF) << 24) | ((*(tmi + 1) & 0x0FF) << 16)
								| ((*(tmi + 2) & 0x0FF) << 8) | (*(tmi + 3) & 0x0FF);
					   if ((T->user_set) && (profile_userset_value(PR, T, &tmi[0]) == false))
						   return false;
					   *(uint32_t*) T->data = tmi[0];
				   }
				   else
					   return false;
		   }
		   break;
	   case DC_INC:
		   break;
	}
	return 1;
}

/**This function colors all dynamic parameters.
 * @since 0.0.1
 */
void ColorDynamicParameters(void){
	block_color = get_block_color(&profile);
}

/**This function gets a parameter by its ID
 * @since 0.0.1
 */
profile_param_t* get_param_by_id(KURS_ProfileTypeDef *pr, uint32_t param_id){
	int table_index = 0;
//	profile_t *pr = &qsfp_cfp_profile;

	// iterate through each table to find a match
	for (table_index = 0; table_index <= TABLE_103_INDEX; table_index++){
		if ((pr->section_param_count[table_index] == 0) || (pr->parameters[table_index] == NULL)){
			continue;
		}
		uint32_t param_cnt = pr->section_param_count[table_index];
		profile_param_t* T = pr->parameters[table_index];
		uint16_t param_index = 0;
		// iterate through all the parameters of a table to find a match
		for (param_index = 0; param_index < param_cnt; param_index++, T++){
			if(T != NULL){
				if(T->param_id == param_id){
					return T;
				}
			}
		}
	}
	return NULL;
}

/**This function recalculates the EMS block_color parameter.
 * @param[in] PR pointer to the profile instance
 * @return blockcolor value between 0 and 7
 * @since 0.0.1
 */
uint8_t get_block_color(KURS_ProfileTypeDef *PR){
#ifndef	TABLE_2_INDEX
#define TABLE_2_INDEX	1
#endif
	uint8_t color = COLOR_Normal;
	profile_param_t *P = PR->parameters[TABLE_2_INDEX];
	uint16_t T_Count = PR->section_param_count[TABLE_2_INDEX];
	uint16_t i = 0;
	for (i = 0; i < T_Count; i++, P++)
		if ((((params_t2_add_t*) P->d_add) != NULL) &&
		(color < ((params_t2_add_t*) P->d_add)->COLOR))
			color = ((params_t2_add_t*) P->d_add)->COLOR;
	return color;
}

/**This function reads the language file.
 * The language file location is defined
 * in the \c lang field of the KURS_ProfileTypeDef structure
 * (normally sector 7 of the internal flash 0x8060000).
 * @return None
 * @since 0.0.1
 */
__weak void KURS_ReadLngFile(KURS_ProfileTypeDef* _profile){
//	UNUSED(_profile);
	/*
	 * DO NOT MODIFY here
	 * Modify and implement in the application main file
	 */
}

/**This function writes the language file to flash.
 * @return None
 * @since 0.0.1
 */
__weak void KURS_WriteLngFileToFlash(KURS_ProfileTypeDef* _profile){
//	UNUSED(_profile);
	/*
	 * DO NOT MODIFY here
	 * Modify and implement in the application main file
	 */
}

/**This function gets a fresh copy of a parameter value.
 *
 * @param pr	pointer to the profile instance
 * @param section		pointer to the parameter
 * @return 1 on success 0 on failure
 * @since 0.0.1
 */
uint16_t profile_userget_value(KURS_ProfileTypeDef *pr, profile_param_t *section){
	if ((section->param_id == 0) && (section->user_get == 0)){
		return 0;
	}
	return KURS_userget_value(pr, section);
	/*
	 * DO NOT MODIFY here
	 * Modify and implement in the application main file
	 */
}

__weak char KURS_userget_value( KURS_ProfileTypeDef *pr, profile_param_t *section ){
//	UNUSED(pr);
//	UNUSED(section);
	return 0;
	/*
	 * DO NOT MODIFY here
	 * Modify and implement in the application main file
	 */
}

/**This function changes a parameter to a new value based on user selection.
 *
 * @param PR	pointer to the profile instance
 * @param t		pointer to the parameter
 * @param value	pointer to the new value
 * @return 1 on success
 * @since 0.0.1
 */
char profile_userset_value( KURS_ProfileTypeDef *PR, profile_param_t *t, void *value ){
	if ((t->param_id == 0) && (t->user_set == 0)){
		return 0;
	}
	return KURS_userset_value(PR, t, value);
}

__weak char KURS_userset_value( KURS_ProfileTypeDef *PR, profile_param_t *t, void *value ){
//	UNUSED(PR);
//	UNUSED(t);
//	UNUSED(value);
	return 0;
	/*
	 * DO NOT MODIFY here
	 * Modify and implement in the application main file
	 */
}

/**This function uses custom logic to calculate the color of a parameter.
 *
 * @param t			pointer to the parameter to color
 * @param T2_ADD	pointer to the additional data of a parameter
 * @return the color of the parameter
 * @since 0.0.1
 */
uint16_t user_get_color(profile_param_t *t, params_t2_add_t *T2_ADD){
//	profile_param_t *T = t;
//	uint32_t value = *(uint32_t*)T->data;
	// if monitoring is OFF, do not color.
	if (*T2_ADD->monitor_on == 0) {
		T2_ADD->COLOR = COLOR_NO;
		return T2_ADD->COLOR;
	}
	return KURS_userget_color(t, T2_ADD);
}

__weak char KURS_userget_color( profile_param_t *t, params_t2_add_t *T2_ADD ){
	return T2_ADD->COLOR;
	/*
	 * DO NOT MODIFY here
	 * Modify and implement in the application main file
	 */
}

/**Helper function that handles KURS requests.
 *
 * @param[in] _req		pointer to the request that is received
 * @param[in,out] _rep		pointer to the reply that will be sent back
 * @param[out] _rep_len	length of the generated reply
 */
void KURS_HandleRequests(char* _req, char* _rep, int* _rep_len){
	switch (_req[1]){
	   case KURS_RD_DEVICE_CLASS:
		   HandleKursRdDeviceClass(_req, _rep, _rep_len);
		   break;
	   case KURS_EN_SET_PARAMS:
		   HandleKursEnableSetParams(_req, _rep, _rep_len);
		   break;
	   case KURS_SAFE_REPROGRAMMING://Launch the processor reprogramming new version (safe)
			HandleKursSafeReprogramming(_req, _rep, _rep_len);
			break;
	   case KURS_RD_PROFILE_LINE: /*<addr><cmd><line_index><0x0D>*/
		   HandleKursRdProfile(_req, _rep, _rep_len);
		   break;
	   case KURS_RD_PROFILE_PARAMS:
	   case KURS_RD_PROFILE_PARAMS_WITH_COLORS:
		   HandleKursRdStrValueAndColor(_req, _rep, _rep_len);
		   break;
	   case KURS_SET_PROFILE_PARAMS:
	   case KURS_SET_PROFILE_CALIB_PARAMS:
		   HandleKursSetParamValue(_req, _rep, _rep_len);
		   break;
	   case KURS_SET_COLOR_DEGRADE_AND_FAILURE:
		   HandleKursSetColorDegradeAndFailure(_req, _rep, _rep_len);
		   break;
	   case KURS_WRITE_LANGUAGE_PACK:
		   HandleKursWriteLangPackCommand(_req, _rep, _rep_len);
		   break;
	   case KURS_READ_LANGUAGE_PACK:
		   HandleKursReadLangPackCommand(_req, _rep, _rep_len);
		   break;
	   case KURS_GET_COLOR_DEGRADE_AND_FAILURE:
		   HandleKursGetColorDegradeAndFailure(_req, _rep, _rep_len);
		   break;
#ifdef PROF_V5
	   case KURS_EXTENDED_DEVICE_CLASS:
		   HandleKursExtendedRdDeviceClass(_req, _rep, _rep_len);
		   break;
	   case KURS_EXTENDED_EN_SET_PARAMS:
		   HandleKursExtendedEnableSetParams(_req, _rep, _rep_len);
		   break;
	   case KURS_EXTENDED_CONFIG_RD_WR:
		   HandleKursExtendedConfigRdWr(_req, _rep, _rep_len);
		   break;
#endif
	   default:
		   HandleKursUnknownCommand(_req, _rep, _rep_len);
		   break;
	}
}
