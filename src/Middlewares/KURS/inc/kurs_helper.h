/*
 * kurs_helper.h
 *
 *  Created on: June 26, 2018
 *      Author: Clara Zaiter
 */

#ifndef KURS_HELPER_H_
#define KURS_HELPER_H_

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

/** KURS commands
 * \cond
 *
 */
// KURS commands
#define KURS_RD_DEVICE_CLASS                    0x01
#define KURS_SAFE_REPROGRAMMING   				0x0F
#define KURS_EN_SET_PARAMS                      0x49
#define KURS_RD_PROFILE_PARAMS                  0x71
#define KURS_SET_PROFILE_PARAMS                 0x72
#define KURS_RD_PROFILE_LINE                    0x73
#define KURS_SET_PROFILE_CALIB_PARAMS			0x76
#define KURS_RD_PROFILE_PARAMS_WITH_COLORS		0x77
#define KURS_WRITE_LANGUAGE_PACK				0x7A
#define KURS_READ_LANGUAGE_PACK					0x7B
#define KURS_SET_COLOR_DEGRADE_AND_FAILURE		0x7C
#define KURS_GET_COLOR_DEGRADE_AND_FAILURE		0x7D
//#ifdef PROF_V5
#define KURS_EXTENDED_DEVICE_CLASS				0x61
#define KURS_EXTENDED_EN_SET_PARAMS				0x62
#define KURS_EXTENDED_CONFIG_RD_WR				0x63
//#endif


// KURS param types
#define DC_NA                 	0        //!<value for unsupported type parameters
#define DC_BOOL         		1        //!<value for boolean type parameters
#define DC_S32                 	2        //!<value for S32 type parameters
#define DC_U32                 	4        //!<value for U32 type parameters
#define DC_FLOAT				8        //!<value for float type parameters
#define DC_HEX                	16       //!<value for hex type parameters
#define DC_STR                	32       //!<value for string type parameters
#define DC_ENUM                	64       //!<value for ENUM type parameters
#define DC_OID                	128      //!<value for OID type parameters
#define DC_IP                 	256      //!<value for IP type parameters
#define DC_INC                	512      //!<value for Self-reset counter parameters
#define DC_U64                	1024	 //!<value for U64 type parameters
#define DC_BITMSK               2048	 //!<value for Bitmask (unsigned 32bit int) parameters

// KURS param colors
#define COLOR_NA                0  //!<unavailable color
#define COLOR_NO                1  //!<color for parameters that should not be colored
#define COLOR_Info              2  //!<color for information parameters
#define COLOR_Normal			3  //!<color for parameters in normal state
#define COLOR_Warning			4  //!<color for parameters in warning state
#define COLOR_Minor				5  //!<color for parameters in minor alarm state
#define COLOR_Major				6  //!<color for parameters in major alarm state
#define COLOR_Critical			7  //!<color for parameters in critical alarm state
#define COLOR_TIP_POROG			8  //!<
#define COLOR_TIP_GET			9  //!<color for parameters that have to be custom colored

// KURS param status
#ifdef PROF_V3
#define STATUS_CMax_HMax        2
#define STATUS_WMax_CMax        1
#define STATUS_WMin_WMax        0
#define STATUS_CMin_WMin        -1
#define STATUS_HMin_CMin        -2
#define STATUS_HMax             3
#define STATUS_HMin             -3
#else
#define STATUS_CMax             2
#define STATUS_WMax             1
#define STATUS_NORM             0
#define STATUS_WMin             -11
#define STATUS_CMin             -2
#endif

/**
 * \endcond
 */

char Str8HexToU32(char* str, uint32_t * pResult);
uint16_t sprintf_s32(char *s, long v);
char * WordToHexStr(char * str, uint16_t wVal);
char Str2HexToU8(char * str, uint8_t * pResult);
char StrToU32(char *str, uint32_t *rez);
char StrHexToU32(char *str, uint32_t *rez);
uint16_t get_crc16_ascii(char *buf, uint16_t size);
uint16_t calc_crc_value(char m, uint16_t crc);
char Str4HexToU16(char *str, uint16_t *rez);
char Str4HexToU16(char *str, uint16_t *rez);
char * UintToHexStr(char * str, uint32_t dwVal);
char StrToU32( char *str, uint32_t *rez );
char StrHexToU32( char *str, uint32_t *rez );
char StrToFloat( char *str, float * pResult, char cDelim );
uint32_t StrToU32Def( char *str, uint32_t def );
char StrToU16( char *str, uint16_t *rez );
uint16_t sprintf_u32( char *s, uint32_t v );
uint16_t sprintf_float(char*str, float f, char del_num);
uint16_t sprintf_s32( char *s, long v );
uint16_t calc_crc16( char * buf, uint32_t dwSize );

#endif /* KURS_HELPER_H_ */
