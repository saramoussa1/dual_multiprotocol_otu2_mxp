/*
 * kurs_helper.c
 *
 *  Created on: June 26, 2018
 *      Author: Clara Zaiter
 */
#include <stddef.h>
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include <inttypes.h>
#include "../inc/kurs_helper.h"
#include "../inc/kurs.h"

/**List of values used for CRC calculation.
 *
 */
const uint16_t crc_16_tab[] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 0xC601,
	0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 0xCC01, 0x0CC0,
	0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 0x0A00, 0xCAC1, 0xCB81,
	0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 0xD801, 0x18C0, 0x1980, 0xD941,
	0x1B00, 0xDBC1, 0xDA81, 0x1A40, 0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01,
	0x1DC0, 0x1C80, 0xDC41, 0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0,
	0x1680, 0xD641, 0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081,
	0x1040, 0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 0x3C00,
	0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 0xFA01, 0x3AC0,
	0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 0x2800, 0xE8C1, 0xE981,
	0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 0xEE01, 0x2EC0, 0x2F80, 0xEF41,
	0x2D00, 0xEDC1, 0xEC81, 0x2C40, 0xE401, 0x24C0, 0x2580, 0xE541, 0x2700,
	0xE7C1, 0xE681, 0x2640, 0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0,
	0x2080, 0xE041, 0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281,
	0x6240, 0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 0xAA01,
	0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 0x7800, 0xB8C1,
	0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 0xBE01, 0x7EC0, 0x7F80,
	0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 0xB401, 0x74C0, 0x7580, 0xB541,
	0x7700, 0xB7C1, 0xB681, 0x7640, 0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101,
	0x71C0, 0x7080, 0xB041, 0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0,
	0x5280, 0x9241, 0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481,
	0x5440, 0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 0x8801,
	0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 0x4E00, 0x8EC1,
	0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 0x4400, 0x84C1, 0x8581,
	0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 0x8201, 0x42C0, 0x4380, 0x8341,
	0x4100, 0x81C1, 0x8081, 0x4040
};

/**This function converts a signed 32-bit number into a string
 *
 *
 * @param s	pointer to the converted string
 * @param v	value to be converted
 * @return length of the converted string
 */
uint16_t sprintf_s32( char *s, long v ){
	char *s_start = s;
	char add = 0, nom = 0;
	if (v < 0)
	{
		*s++ = '-';
		v = -v;
	}
	while (v >= 1000000000)
	{
		nom++;
		v -= 1000000000;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 100000000)
	{
		nom++;
		v -= 100000000;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 10000000)
	{
		nom++;
		v -= 10000000;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 1000000)
	{
		nom++;
		v -= 1000000;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 100000)
	{
		nom++;
		v -= 100000;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 10000)
	{
		nom++;
		v -= 10000;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 1000)
	{
		nom++;
		v -= 1000;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 100)
	{
		nom++;
		v -= 100;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 10)
	{
		nom++;
		v -= 10;
	}
	if (nom) add = 1;
	if (add)
	{
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	*s++ = 0x30 + (v & 0x0F);
	*s = 0;
	return (s - s_start);
//	return false;
}

/**This function tries to convert a string into an unsigned 16-bit number.
 *
 * @param str the string to convert
 * @param rez the converted value
 * @return 1 for success and 0 for failure
 */
char StrToU16( char *str, uint16_t *rez )
{
	uint32_t tm32;
	char i;
	char detect = false;
	for (i = 0, tm32 = 0; (i < 10); str++)
	{
		if (tm32 > 0x0FFFF) break; //Ã�Ã¥Ã°Ã¥Ã¯Ã®Ã«Ã­Ã¥Ã­Ã¨Ã¥
		if ((detect == false) && (*str == ' ')) continue;
		else if (('0' <= *str) && (*str <= '9'))
		{
			tm32 = tm32 * 10 + (*str & 0x0F);
			detect = 1; //Ã�Ã®ÃªÃ°Ã Ã©Ã­Ã¥ Ã¬Ã¥Ã°Ã¥ Ã®Ã¤Ã¨Ã­Ã® Ã·Ã¨Ã±Ã¤Ã® Ã®Ã¯Ã°Ã¥Ã¤Ã¥Ã«Ã¨Ã«Ã¨
		}
		else if (detect)
		{
			*rez = tm32;
			return 1;
		}
		else break;
	}
	return 0;
}

/**This function converts a float into a string
 *
 * @param str		pointer to the converted string
 * @param f			float value to be converted
 * @param del_num
 * @return length of the converted string
 * @since 0.0.1
 */
uint16_t sprintf_float(char*str, float f, char del_num)
{
	union
	{
		float f;
		uint32_t itm;
		struct
		{
			uint32_t m :23;
			uint32_t e :8;
			uint32_t s :1;
		} f_s;

	} float_r;
	char*str_start = str;
	signed long long s64man;
	int32_t exp;

	uint32_t rez_int;
	char i = 0;
	float_r.f = f;
	int exp10 = 1;
	for (i = 0; i < del_num; i++, exp10 *= 10)
		;
	float_r.f *= exp10;
	s64man = float_r.f_s.m;
	exp = float_r.f_s.e;
	s64man |= (1 << 23);
	exp = (exp - 127) - 23 + 32;
	s64man = (exp > 0) ? (s64man << exp) : (s64man >> (-exp));
	if (((uint32_t) s64man) >= 0x7FFFFFFF)
		((uint32_t*) &s64man)[1]++;
	rez_int = ((uint32_t*) &s64man)[1];
	if ((float_r.f_s.s) && (rez_int))
		*str++ = '-';

	uint8_t nom = 0, add = 0;
	while (rez_int >= 1000000000)
	{
		if (nom < 9)
			nom++;
		rez_int -= 1000000000;
	}
	if (nom || (1000000000 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 10)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 100000000)
	{
		nom++;
		rez_int -= 100000000;
	}
	if (nom || (100000000 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 9)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 10000000)
	{
		nom++;
		rez_int -= 10000000;
	}
	if (nom || (10000000 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 8)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 1000000)
	{
		nom++;
		rez_int -= 1000000;
	}
	if (nom || (1000000 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 7)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 100000)
	{
		nom++;
		rez_int -= 100000;
	}
	if (nom || (100000 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 6)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 10000)
	{
		nom++;
		rez_int -= 10000;
	}
	if (nom || (10000 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 5)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 1000)
	{
		nom++;
		rez_int -= 1000;
	}
	if (nom || (1000 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 4)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 100)
	{
		nom++;
		rez_int -= 100;
	}
	if (nom || (100 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 3)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (rez_int >= 10)
	{
		nom++;
		rez_int -= 10;
	}
	if (nom || (10 <= exp10))
		add = 1;
	if (add)
	{
		if (del_num == 2)
			*str++ = '.';
		*str++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	if (del_num == 1)
		*str++ = '.';
	*str++ = 0x30 + (rez_int & 0x0F);
	*str = 0;
	return(str - str_start);
//	return false;
}

/**This function converts a string into an unsigned 32-bit number.
* @param[in] str the string to convert into a number
* @param[in] def
* @return the converted value if successful
* @since 0.0.1
*/
uint32_t StrToU32Def( char *str, uint32_t def )
{
	uint32_t tm32;
	char i;
	char detect = 0;
	for (i = 0, tm32 = 0; (i < 10); str++)
	{
		// this checks wether the value we are sending exceeds 32bits (429496729'6')
		// when we reach '429496729.' we look at the following digit, if it's greater than 6
		// then the string we sent exceeds a 32 bits interger
		if ((tm32 > 429496729) || ((tm32 == 429496729) && ('6' <= *str))) break;
		if ((detect == false) && (*str == ' ')) continue;
		else if (('0' <= *str) && (*str <= '9'))
		{
			// turn the string, digit by digit, to a u32 integer
			tm32 = tm32 * 10 + (*str & 0x0F);
			detect = true;
		}
		else if (detect) return tm32;
		else break;
	}
	return def;
}


/**This function converts a 4-digit hex string into an unsigned 16-bit number.
* @param[in] str	pointer to the string
* @param[out] rez 	pointer to the converted value
* @return 1 on success
* @since 0.0.7
*/
char Str4HexToU16( char *str, uint16_t *rez ){
#define MAX_U16_DIGIT_CNT	5
	uint16_t tm16 = 0;
	char digit_cnt = 0;
	char detect = false;
	do
	{
		if((tm16 == 6553) && ('6' <= *str)) //at the edge and the next character will be greater than 16 bits
		{
			return false;
		}
		if ((detect == false) && (*str == ' '))
		{
		}
		else if (('0' <= *str) && (*str <= '9'))// we always get here as long as we have hex digits (0 -> 9)
		{
			digit_cnt++;
			if(digit_cnt > MAX_U16_DIGIT_CNT) // the number we received exceeds the maximum of 16-bits values
			{
				return false;
			}
			tm16 = tm16 * 16 + (*str - '0');
			detect = true;
		}

		else if (('a' <= *str) && (*str <= 'f'))// we always get here as long as we have hex digits (A -> F)
		{
			digit_cnt++;
			if(digit_cnt > MAX_U16_DIGIT_CNT) // the number we received exceeds the maximum of 16-bits values
			{
				return false;
			}
			tm16 = tm16 * 16 + (*str - 'a' + 10);
			detect = true;
		}

		else if (('A' <= *str) && (*str <= 'F'))// we always get here as long as we have hex digits (A -> F)
		{
			digit_cnt++;
			if(digit_cnt > MAX_U16_DIGIT_CNT) // the number we received exceeds the maximum of 16-bits values
			{
				return false;
			}
			tm16 = tm16 * 16 + (*str - 'A' + 10);
			detect = true;
		}

		else if (detect)// the number we received as string has ended
		{
			*rez = tm16;
			return true;
		}
		str++;
	}while(1);
	return false;
}

/**This function converts an unsigned integer to a string in hexadecimal format.
 * @param[out] str pointer to the converted string
 * @param[in] dwVal the number to convert
 * @return pointer to the end of the string
 * @since 0.0.7
 */
char * UintToHexStr( char * str, uint32_t dwVal ){
	if( !str ) return str;
	char * pch = &str[ 7 ];
	uint8_t by;
	while( pch >= str )
	{
		by = dwVal & 0x0F;
		dwVal >>= 4;
		if( by > 9 ) *pch-- = ( 'A' - 10 ) + by;
		else *pch-- = '0' + by;
	}
	pch += 9;
	*pch = 0;
	return pch;
//	return false;
}

/**This function an unsigned 16-bit number into a string in hexadecimal format.
 * @param[out] 	str	 	pointer to the converted string
 * @param[in] 	wVal	the number to convert
 * @return pointer to the end of the string
 * @since 0.0.7
 */
char * WordToHexStr( char * str, uint16_t wVal ){
	if( !str ) return str;
	char * pch = &str[ 3 ];
	uint8_t by;
	while( pch >= str ){
		by = wVal & 0x0F;
		wVal >>= 4;
		if( by > 9 ) *pch-- = ( 'A' - 10 ) + by;
		else *pch-- = '0' + by;
	}
	pch += 5;
	*pch = 0;
	return pch;
//	return false;
}

/**This function converts a 2-digit hexadecimal string into an unsigned byte number.
 * @param[in] 	str 	pointer to the string to be converted
 * @param[out] 	pResult	pointer to the converted number
 * @return true on success, false on failure
 * @since 0.0.7
 */
char Str2HexToU8( char * str, uint8_t * pResult ){
	*pResult = 0;
	if( !str ) return false;
	uint32_t dwCnt = 2;
	while( dwCnt-- ){
		char ch = *str++;
		*pResult <<= 4;
		if( ch >= '0' && ch <= '9' ) *pResult += ch - '0';
		else if( ch >= 'A' && ch <= 'F' ) *pResult += ch - 'A' + 10;
		else if( ch >= 'a' && ch <= 'f' ) *pResult += ch - 'a' + 10;
		else return false;
	}
	return true;
}

/**This function converts an 8-digit hexadecimal string into an unsigned 32-bit number.
 * @param[in] 	str		pointer to the string to be converted
 * @param[out] 	pResult pointer to the converted number
 * @return true on success, false on failure
 * @since 0.0.7
 */
char Str8HexToU32( char* str, uint32_t * pResult ){
	*pResult = 0;
	if( !str ) return false;
	uint32_t dwCnt = 8;
	while( dwCnt-- ){
		char ch = *str++;
		*pResult <<= 4;
		if( ch >= '0' && ch <= '9' ) *pResult += ch - '0';
		else if( ch >= 'A' && ch <= 'F' ) *pResult += ch - 'A' + 10;
		else if( ch >= 'a' && ch <= 'f' ) *pResult += ch - 'a' + 10;
		else return false;
	}
	return true;
}

/**This function tries to convert a string to a float number.
 *
 * @param[in] 	str		pointer to the string to convert
 * @param[out] 	pResult	pointer to the converted number
 * @param[in] 	cDelim	delimiter character
 * @return true on success
 * @since 0.0.1
 */
char StrToFloat( char *str, float * pResult, char cDelim ){
	if( !str ) return false;
	while( *str == ' ' ) str++;
	char * pStart = str;
	if( *str == '+' || *str == '-' ) str++;
	uint32_t dwCnt = 0;
	while( *str >= '0' && *str <= '9' )
	{
		str++;
		dwCnt++;
	}
	if( !dwCnt ) return false;
	if( *str == '.' ) str++;
	while( *str >= '0' && *str <= '9' )	str++;
	while( *str == ' ' ) str++;
	if( *str != cDelim ) return false;

	*pResult = atof( pStart );
	return true;
}

/**This function converts an unsigned 32-bit number into a string.
 *
 * @param[out] s the converted string
 * @param[in]  v the number to convert
 * @return length of the string
 * @since 0.0.1
 */
uint16_t sprintf_u32(char *s, uint32_t v){
	char *s_start = s;
	char add = 0, nom = 0;
	while (v >= 1000000000){
		nom++;
		v -= 1000000000;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 100000000){
		nom++;
		v -= 100000000;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 10000000){
		nom++;
		v -= 10000000;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 1000000){
		nom++;
		v -= 1000000;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 100000){
		nom++;
		v -= 100000;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 10000){
		nom++;
		v -= 10000;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 1000){
		nom++;
		v -= 1000;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 100){
		nom++;
		v -= 100;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	while (v >= 10)	{
		nom++;
		v -= 10;
	}
	if (nom) add = 1;
	if (add){
		*s++ = 0x30 + (nom & 0x0F);
	}
	nom = 0;
	*s++ = 0x30 + (v & 0x0F);
	*s = 0;
	return (s - s_start);
//	return false;
}

/**This function tries to convert a string into an unsigned 32-bit number.
 *
 * @param[in] str 	pointer to the string to convert
 * @param[out] rez	the converted number
 * @return 1 on success
 * @since 0.0.1
 */
char StrToU32( char *str, uint32_t *rez ){
#define MAX_U32_DIGIT_CNT 10	// sizeof(uint32_t) is 4294967295 -> 10 digits long
	uint32_t tm32 = 0;
	char digit_cnt = 0;
	char detect = false;
	//for (i = 0, tm32 = 0; (i < 10); str++)
	do{
		if((tm32 == 429496729) && ('6' <= *str)){ //at the edge and the next character will be greater than 32 bits
			return false;
		}
		if ((detect == false) && (*str == ' ')){
		}else if (('0' <= *str) && (*str <= '9')){// we always get here as long as we have digits
			digit_cnt++;
			if(digit_cnt > MAX_U32_DIGIT_CNT){ // the number we received exceeds the maximum number of digits needed to represent 32-bits values
				return false;
			}
			tm32 = tm32 * 10 + (*str & 0x0F);
			detect = true;
		}else if (detect){// the number we received as string has ended
			*rez = tm32;
			return true;
		}
		str++;
	}while(1);
	return false;
}

/**This function converts a hexadecimal string representation into an unsigned 32-bit number.
 *
 * @param[in] str
 * @param[out] rez
 * @return 1 on success
 * @since 0.0.1
 */
char StrHexToU32( char *str, uint32_t *rez ){
	uint32_t item = 0;
	*rez = 0;
	while (1){
		char tmp = *str++;
		*rez <<= 4;
		if (tmp >= '0' && tmp <= '9') *rez += (tmp - '0');
		else if (tmp >= 'A' && tmp <= 'F') *rez += (tmp - 'A' + 10);
		else if (tmp >= 'a' && tmp <= 'f') *rez += (tmp - 'a' + 10);
		else return false;
		if ((++item) >= 8) break;
	}
	return true;
}


////////////////////////////////////////////////////////////////////////////////
/**This function calculates the CRC of a string.
* @param[in] buf string for which the CRC has to be calculated
* @param[in] size length of the string
* @return the calculated CRC value
* @since 0.0.1
*/
uint16_t get_crc16_ascii( char *buf, uint16_t size ){
	unsigned int i;
	unsigned int crc = 0;

	for (i = 0; *buf && (i < size); i++, buf++) crc = calc_crc_value(*buf, crc);

	return crc;
}

////////////////////////////////////////////////////////
/**This function generates a CRC.
 *
 * @param[in] m		value used in CRC generation
 * @param[in] crc	old value of CRC used in CRC recalculation
 * @return the new calculated CRC
 * @since 0.0.1
 */
uint16_t calc_crc_value( char m, uint16_t crc ){
	return ((crc >> 8) ^ crc_16_tab[(crc ^ m) & 0xFF]);
}

uint16_t calc_crc16( char * buf, uint32_t dwSize )
{
	alt_u32 crc = 0;
	while( dwSize-- )
	{
		crc = ( crc >> 8 ) ^ crc_16_tab[ ( crc ^ *buf++ ) & 0xFF ];
	}
	return crc;
}
