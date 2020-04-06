/*
 * kurs.h
 *
  *  Created on: June 26, 2018
 *      Author: Clara Zaiter
 */

#ifndef KURS_H_
#define KURS_H_

#include "alt_types.h"
#include "stdint.h"
//#define PROF_V3
#define PROF_V5

//#define PROF_MAX_LINE_LENGTH		200
//#ifdef PROF_V3
//#define PROF_LINE_COUNT		4//46//550//145//46//550
//#else
//#define PROF_LINE_COUNT		550 + 213
//#endif

#ifndef __weak
#define __weak __attribute__((weak))
#endif

#ifdef PROF_V3
#define PROFILE_SECTION_COUNT        11			//!<Number of sections that constitute the profile.
												// count of sections per profile excluding the header, color transaltor, groupmon ...
												// we have 7 basic sections in addition to section 100, 101, 102 and 103
												// this adds up to 11 sections
#elif defined PROF_V5
#define PROFILE_SECTION_COUNT        12			// We have section 8 as an extra section
#endif
												// we have 8 basic sections (for prof5) and 7 (for prof 3)
												// in addition to section 100, 101, 102 and 103 this adds up to 12 sections

#define LANG_BLOCK_SIZE				256					//!<Size of a packet used when uploading a new language file
#define LANG_BLOCK_MAX_CNT			150					//!<maximum number of blocks

#define DEF_KURS_ADDRESS        	0x80	//!<fixed part of a KURS address
#define TABLE_1_INDEX        		0		//!<index of table 2 in profile
#define TABLE_2_INDEX        		1		//!<index of table 2 in profile
#define TABLE_3_INDEX        		2		//!<index of table 3 in profile
#define TABLE_4_INDEX        		3		//!<index of table 3 in profile
#define TABLE_5_INDEX        		4		//!<index of table 3 in profile
#define TABLE_6_INDEX        		5		//!<index of table 3 in profile
#define TABLE_7_INDEX        		6		//!<index of table 3 in profile
#ifdef PROF_V5
#define TABLE_8_INDEX        		7		//!<index of table 3 in profile
#define TABLE_100_INDEX         	8 		//!<index of table 103 in profile
#define TABLE_101_INDEX         	9 		//!<index of table 103 in profile
#define TABLE_102_INDEX         	10 		//!<index of table 103 in profile
#define TABLE_103_INDEX         	11 		//!<index of table 103 in profile
#else
#define TABLE_100_INDEX         	7			//!<index of table 103 in profile
#define TABLE_101_INDEX         	8 		//!<index of table 103 in profile
#define TABLE_102_INDEX         	9  		//!<index of table 103 in profile
#define TABLE_103_INDEX         	10 		//!<index of table 103 in profile
#endif
#define DEF_DEV_CLASS				"tbd"
#define DEF_DEV_PREFIX				"tbd"

typedef void (*profile_set_default_callback)(uint32_t );		//!<callback function to set parameters to default values
typedef void (*profile_init_callback)();                      //!<callback function to initialize the profile instance and parameters
typedef void (*profile_save_callback)();                      //!<callback function to save new values for user data

typedef struct params_t2_add{
	uint16_t COLOR;                 //!< current color of the parameter
	uint16_t COLOR_Degrade;         //!<color of the parameter when in degrade state
	uint16_t COLOR_Failure;        	//!<color of the parameter when in failure state
	char COLOR_Tip;					//!<flag indicating whether the parameter should be custom colored
	void *HMax;						//!<value of HMax threshold
	void *CMax;						//!<value of CMax threshold
	void *WMax;						//!<value of WMax threshold
	void *WMin;						//!<value of WMin threshold
	void *CMin;						//!<value of CMin threshold
	void *HMin;						//!<value of HMin threshold
	char status;					//!<current status of the parameter
	uint32_t *monitor_on;			//!<flag indicating whether to turn on/off monitoring (coloring) for this parameter
} params_t2_add_t;

/**Custom defined type representing a parameter of the profile.
 *
 */
typedef struct profile_param{
	uint32_t param_id;	//!<unique ID of the parameter
	char user_get;		//!<flag indicating whether the parameter needs processing before it is sent back to EMS
	char user_set;		//!<flag indicating whether the EMS value needs processing before it is applied to parameter
	uint16_t type;		//!<KURS type of the parameter
	uint16_t access;	//!<KURS access level of the parameter
	void *data;			//!<pointer to the variable storing the current value of the parameter
	uint16_t d_size;	//!<size in bytes of the variable i.e. 4 bytes for u32
	void *d_min;		//!<minimum value of the parameter value
	void *d_max;		//!<maximum value of the parameter value
	void *d_add;		//!<pointer to the additional data structure of the parameter
	char *format;		//!<string format used for the parameter value i.e "%.2f" for float with two fractional digits
} profile_param_t;

/**Custom defined type representing the profile.
 *
 */
typedef struct{
	uint16_t addr_bus;									//!<KURS address for our device (read from HW pins)
	char ver_prof[8];									//!<profile version
	char class_[8];										//!<device class
	char prefix[8];										//!<device class prefix
	char num_lang;										//!<language index: 1 for Russian and 2 for English
	char num_bitmap;									//!<???
	profile_param_t *parameters[PROFILE_SECTION_COUNT];	//!<List of sections' parameters
	uint16_t section_param_count[PROFILE_SECTION_COUNT];//!<Number of parameters for each section
	char set_param_enabled;								//!<flag set by sending special command to allow parameter value modification
#ifdef PROF_V5
	char section_param_changed[PROFILE_SECTION_COUNT];  //!<Number of sections where parameters are changed
	char save_param_enabled;							//!<flag set by sending special command to allow parameter value modification saving
#endif
	profile_set_default_callback SetDefault;			//!<pointer to the function that sets parameters to their default values
	profile_init_callback Init;							//!<pointer to the function that initializes the profile instance
	profile_save_callback Save;							//!<pointer to the function that saves user data to flash
//	char* user_data;									//!<pointer to user data
//	uint32_t user_data_size;							//!<size of the user data
	char **text;										//!<pointer to the profile text
//	char text[PROF_PROF_LINE_COUNT][PROF_MAX_LINE_LENGTH];//char** text;										//!<pointer to the profile text
	uint16_t line_count;								//!<Length of the profile text lines
	char lngpack[LANG_BLOCK_MAX_CNT][LANG_BLOCK_SIZE];	//!<memory space reserved for a language file
//	char *lngpack_location;	//!<address where language pack is stored
} KURS_ProfileTypeDef;

/**
 * List of possible data protocol rates.
 */
typedef enum{
	KURS_PORT_RATE_OTU1 	= 0,  	/*!< KURS_PORT_RATE_OTU1 */
	KURS_PORT_RATE_OTU2 	= 1,  	/*!< KURS_PORT_RATE_OTU2 */
	KURS_PORT_RATE_STM1 	= 128,	/*!< KURS_PORT_RATE_STM1*/
	KURS_PORT_RATE_STM4 	= 130,	/*!< KURS_PORT_RATE_STM4*/
	KURS_PORT_RATE_STM16 	= 131,	/*!< KURS_PORT_RATE_STM16*/
	KURS_PORT_RATE_STM64 	= 132,	/*!< KURS_PORT_RATE_STM64*/
	KURS_PORT_RATE_FE 		= 256, 	/*!< KURS_PORT_RATE_FE  */
	KURS_PORT_RATE_GE 		= 257, 	/*!< KURS_PORT_RATE_GE  */
	KURS_PORT_RATE_XGE 		= 258, 	/*!< KURS_PORT_RATE_XGE  */
	KURS_PORT_RATE_FC1 		= 512, 	/*!< KURS_PORT_RATE_FC1  */
	KURS_PORT_RATE_FC2 		= 513, 	/*!< KURS_PORT_RATE_FC2  */
}KURS_port_rate_t;

typedef enum{
	BLOCK_STATE_OK = 1,
	BLOCK_STATE_NOT_READY = 2,
	BLOCK_STATE_FAIL = 3,
}KURS_block_state_t;

void KURS_HandleRequests(char* _req, char* _rep, int* _rep_len);
char KURS_userget_color( profile_param_t *t, params_t2_add_t *T2_ADD );
char KURS_userset_value( KURS_ProfileTypeDef *PR, profile_param_t *t, void *value );
char KURS_userget_value( KURS_ProfileTypeDef *pr, profile_param_t *section );
void KURS_profile_set_default(uint32_t selected_sections);
void KURS_RebootDelayed();
void KURS_NewLngpackCallback(void);
void KURS_WriteLngFileToFlash(KURS_ProfileTypeDef* _profile);
void KURS_ReadLngFile(KURS_ProfileTypeDef* _profile);
void commit_user_settings();
void KURS_profile_init();

#endif /* KURS_H_ */
