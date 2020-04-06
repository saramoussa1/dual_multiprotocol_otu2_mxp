/*
 * ipg_utils.h
 *
 *  Created on: Mar 24, 2016
 *      Author: root
 */

#ifndef IPG_UTILS_H_
#define IPG_UTILS_H_

#include "string.h"
#include "HAL/inc/io.h"
#include "ipg_types.h"
#include "ctype.h"
#include "float.h"
#include "math.h"
#include "ipg_bits.h"
#include "ipglb_transceiver.h"

typedef enum{
	DOUBLE_EQUAL = 0,
	DOUBLE_LT = 1,
	DOUBLE_GT = 2,
}DOUBLE_CMP_RESULT_t;

static DOUBLE_CMP_RESULT_t double_compare(double a, double b, double resolution){
	DOUBLE_CMP_RESULT_t res = DOUBLE_EQUAL;
	double delta = a-b;
	if(fabs(delta) < resolution){
		res = DOUBLE_EQUAL;
	}
	if(delta > resolution){
		res = DOUBLE_LT;
	}
	if((-delta)>resolution){
		res = DOUBLE_GT;
	}
	return (res);
}

static int double_compare_lt(double a, double b, double resolution){
	if((b-a)> resolution){
		return 1;
	}
	return 0;
}

static int double_compare_gt(double a, double b, double resolution){
	if((a-b)> resolution){
		return 1;
	}
	return 0;
}

static int double_compare_equal(double a, double b, double resolution){
	if(fabs(a-b) < resolution){
		return 1;
	}
	return 0;
}

#define UW_TO_DBM(val) (10 * log10(val * 0.001))

/** Functions that removes trailing spaces from the end of a string
 * string has to end with a null character
* @param  str pointer to the string that has to be trimmed
* @retval None
*/
static void rtrim(char *str){
	int n;
	n = strlen(str);
	while (n > 0 && isspace((unsigned char)str[n - 1])) {
		n--;
	}
	str[n] = '\0';
}

/**This function converts a value in uW to dBm.
 * @param[in] uW value to be converted in uW
 * @return	the converted value in dBm
 */
static double uW_to_dBm(double uW){
	double converted = 0;
	// make it into mW
	converted = uW * 0.001;
	// Optical power in is in steps of 0.01 uW, it needs to be transformed into
	return 	(10 * log10(converted));
}

/**This function converts an optical power raw value (two bytes) into dBm.
 * The raw value is a two bytes array with MSB first.
 * @param[in] val pointer to the raw optical power value
 * @return the converted value in dBm
 */
static double raw_to_dBm(alt_u8* val){
	return 	(uW_to_dBm(((val[0] << 8)|val[1]) * 0.1));
}

static ret_code_t channel_to_freq(tuning_info_t tuning_info, alt_u32 channel, double* freq){
	ret_code_t rc = RET_FAIL;
	double _freq = 0;
	_freq = (channel - 1) * tuning_info.grid_spacing + tuning_info.first_freq;
	if(double_compare_lt(_freq, tuning_info.last_freq, tuning_info.grid_spacing*0.1) == 1){
		rc = RET_SUCCESS;
		*freq = _freq;
	}
	return 	(rc);
}

static ret_code_t freq_to_channel(tuning_info_t tuning_info, double freq, alt_u32* channel){
	ret_code_t rc = RET_SUCCESS;
	double f_channel = 0.0;
	if(double_compare_lt(freq, tuning_info.first_freq, tuning_info.grid_spacing*0.1) || (double_compare_gt(freq, tuning_info.last_freq, tuning_info.grid_spacing*0.1))){
		rc = RET_FAIL;
	}
	if(rc == RET_SUCCESS){
		f_channel = 1 + (freq - tuning_info.first_freq) / tuning_info.grid_spacing;
		*channel = f_channel;
		if(double_compare_lt((f_channel - *channel), 0.5, tuning_info.grid_spacing*0.1)){
			f_channel = floor(f_channel);
		}else{
			f_channel = ceil(f_channel);
		}
		*channel = f_channel;
	}
	return 	(rc);
}

typedef enum{
	FLOAT_INV = -1,
	FLOAT_EQUAL = 0,
	FLOAT_LT = 1,
	FLOAT_GT = 2,
}FLOAT_CMP_RESULT_t;

static FLOAT_CMP_RESULT_t float_compare(float a, float b, float resolution){
	FLOAT_CMP_RESULT_t res = FLOAT_INV;
	float delta = a-b;
	if(fabs(delta) < resolution){
		res = FLOAT_EQUAL;
	}
	if(delta > resolution){
		res = FLOAT_LT;
	}
	if((-delta)>resolution){
		res = FLOAT_GT;
	}
	return 	(res);
}

static int float_compare_lt(float a, float b, float resolution){
	uint32_t res = 0;
	if((b-a)> resolution){
		res = 1;
	}
	return (res);
}

static int float_compare_gt(float a, float b, float resolution){
	uint32_t res = 0;
	if((a-b)> resolution){
		res = 1;
	}
	return (res);
}

static int float_compare_equal(float a, float b, float resolution){
	uint32_t res = 0;
	if(fabs(a-b) < resolution){
		res = 1;
	}
	return (res);
}
#endif /* IPG_UTILS_H_ */
