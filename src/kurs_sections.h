/*
 * kurs_sections.h
 *
 *  Created on: Nov 1, 2018
 *      Author: Clara
 */

#ifndef KURS_SECTIONS_H_
#define KURS_SECTIONS_H_

#include "Middlewares/KURS/inc/kurs_helper.h"
#include "common/inc/ipglb_card.h"
#include "main.h"
#include "kurs_constants.h"
#include "ipg_aps.h"

#define KURS_APS		0

extern ipglb_card_t card;
extern protocol_odux_alarms_t odux_alarms[INTERFACE_COUNT];

#ifdef KURS_APS
extern aps_module_t _aps;
#endif
/**
 * local variables
 */
static uint32_t dummy_u32 = 0;
static uint64_t dummy_u64 = 0;
static float dummy_float = 0;


/*We assign indexes from 0 to 15 for the 16 clients and 16 to 17 for line0 and line1*/
profile_param_t section_1[]=/* __attribute__((section(".prof"))) =*/
{
	/*General parameters */
	{ ID_1_PID, 			   		0, 0, DC_STR,   213, 	card.info.pid, 			       					            PID_LEN, 		    		NULL, NULL, NULL, NULL },
	{ ID_1_SERIAL_NUM, 				0, 0, DC_STR,   213, 	card.info.serial_number, 			       					SR_NUM_LEN, 	    		NULL, NULL, NULL, NULL },
	{ ID_1_HW_NUM, 		       		0, 0, DC_STR,   85, 	card.info.hw_number, 		   					            HW_NUM_LEN, 	    		NULL, NULL, NULL, NULL },
	{ ID_1_SW_NUM, 		        	0, 0, DC_STR,   85, 	card.info.sw_number, 		       				            SW_NUM_LEN, 	    		NULL, NULL, NULL, NULL },
	{ ID_1_SW_NUM_FPGA,         	0, 0, DC_STR,   85, 	card.info.fpga_sw_number, 	       				            SW_NUM_FPGA_LEN,    		NULL, NULL, NULL, NULL },
	/*Line 1 port parameters */
	{ ID_1_LN1_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[16].destination,       		                IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_LN1_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[16].transceiver.info.VN,               		VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_LN1_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[16].transceiver.info.HN,               		PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_LN1_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[16].transceiver.info.SN,               		SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Line 2 port parameters */
	{ ID_1_LN2_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[17].destination,       		                IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_LN2_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[17].transceiver.info.VN,         			VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_LN2_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[17].transceiver.info.HN,         			PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_LN2_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[17].transceiver.info.SN,                 	SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 1 port parameters */
	{ ID_1_CL1_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[0].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL1_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[0].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL1_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[0].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL1_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[0].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 2 port parameters */
	{ ID_1_CL2_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[1].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL2_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[1].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL2_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[1].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL2_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[1].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 3 port parameters */
	{ ID_1_CL3_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[2].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL3_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[2].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL3_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[2].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL3_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[2].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 4 port parameters */
	{ ID_1_CL4_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[3].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL4_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[3].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL4_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[3].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL4_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[3].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 5 port parameters */
	{ ID_1_CL5_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[4].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL5_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[4].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL5_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[4].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL5_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[4].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 6 port parameters */
	{ ID_1_CL6_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[5].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL6_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[5].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL6_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[5].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL6_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[5].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 7 port parameters */
	{ ID_1_CL7_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[6].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL7_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[6].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL7_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[6].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL7_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[6].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 8 port parameters */
	{ ID_1_CL8_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[7].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL8_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[7].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL8_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[7].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL8_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[7].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 9 port parameters */
	{ ID_1_CL9_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[8].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL9_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[8].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL9_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[8].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL9_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[8].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 10 port parameters */
	{ ID_1_CL10_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[9].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL10_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[9].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL10_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[9].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL10_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[9].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 11 port parameters */
	{ ID_1_CL11_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[10].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL11_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[10].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL11_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[10].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL11_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[10].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 12 port parameters */
	{ ID_1_CL12_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[11].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL12_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[11].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL12_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[11].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL12_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[11].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 13 port parameters */
	{ ID_1_CL13_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[12].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL13_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[12].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL13_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[12].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL13_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[12].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 14 port parameters */
	{ ID_1_CL14_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[13].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL14_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[13].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL14_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[13].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL14_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[13].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 15 port parameters */
	{ ID_1_CL15_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[14].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL15_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[14].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL15_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[14].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL15_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[14].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	/*Client 16 port parameters */
	{ ID_1_CL16_DESTINATION,     	0, 0, DC_STR,   245, 	card.interfaces[15].destination,       	         		    IF_DESTINATION_MAX_LEN, 	NULL, NULL, NULL, NULL },
	{ ID_1_CL16_SFP_VN,      		0, 0, DC_STR,   85, 	card.interfaces[15].transceiver.info.VN, 		            VN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL16_SFP_PN,      		0, 0, DC_STR,   85, 	card.interfaces[15].transceiver.info.HN, 		            PN_LENGTH, 	    			NULL, NULL, NULL, NULL },
	{ ID_1_CL16_SFP_SN,      		0, 0, DC_STR,   85, 	card.interfaces[15].transceiver.info.SN,   		            SN_LENGTH, 	    			NULL, NULL, NULL, NULL },
};

/* section 2 params*/
/*General parameters*/
static uint32_t monitor_always_on = 1;
extern diagnostics_t pin[INTERFACE_COUNT];
extern diagnostics_t pout[INTERFACE_COUNT];

params_t2_add_t table2_alarm1_add = 		{COLOR_Normal, COLOR_Minor, COLOR_NA, COLOR_TIP_POROG, NULL, NULL, NULL, NULL, NULL, NULL, '\0', &monitor_always_on,};
params_t2_add_t table2_alarm2_add = 		{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_POROG, NULL, NULL, NULL, NULL, NULL, NULL, '\0',&monitor_always_on,};
params_t2_add_t table2_tcase_add = 			{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.temperature.physical_max, &card.temperature.critical_max, &card.temperature.warning_max, &card.temperature.warning_min, &card.temperature.critical_min, &card.temperature.physical_min, '\0', &monitor_always_on,};

params_t2_add_t table2_ln1_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor,};
params_t2_add_t table2_ln1_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical,  COLOR_TIP_GET,NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor,};
params_t2_add_t table2_ln1_fecload_add =	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[16].port.monitor.fec_statistics.fec_load.physical_max, &card.interfaces[16].port.monitor.fec_statistics.fec_load.critical_max, &card.interfaces[16].port.monitor.fec_statistics.fec_load.warning_max, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor,};
params_t2_add_t table2_ln1_sfp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor};
params_t2_add_t table2_ln1_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[16].transceiver.monitor.temperature.physical_max, &card.interfaces[16].transceiver.monitor.temperature.critical_max, &card.interfaces[16].transceiver.monitor.temperature.warning_max, &card.interfaces[16].transceiver.monitor.temperature.warning_min, &card.interfaces[16].transceiver.monitor.temperature.critical_min, &card.interfaces[16].transceiver.monitor.temperature.physical_min, '\0', (uint32_t*)&card.interfaces[16].monitor,};
params_t2_add_t table2_ln1_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor,};
params_t2_add_t table2_ln1_odu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor,};
params_t2_add_t table2_ln1_prefecber_add =	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[16].port.monitor.fec_statistics.fec_load.physical_max, &card.interfaces[16].port.monitor.fec_statistics.fec_load.critical_max, &card.interfaces[16].port.monitor.fec_statistics.fec_load.warning_max, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor,};
params_t2_add_t table2_ln1_postfecber_add =	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[16].port.monitor.fec_statistics.fec_load.physical_max, &card.interfaces[16].port.monitor.fec_statistics.fec_load.critical_max, &card.interfaces[16].port.monitor.fec_statistics.fec_load.warning_max, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[16].monitor,};

params_t2_add_t table2_ln2_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor,};
params_t2_add_t table2_ln2_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor,};
params_t2_add_t table2_ln2_fecload_add =	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[17].port.monitor.fec_statistics.fec_load.physical_max, &card.interfaces[17].port.monitor.fec_statistics.fec_load.critical_max, &card.interfaces[17].port.monitor.fec_statistics.fec_load.warning_max, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor,};
params_t2_add_t table2_ln2_sfp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor};
params_t2_add_t table2_ln2_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[17].transceiver.monitor.temperature.physical_max, &card.interfaces[17].transceiver.monitor.temperature.critical_max, &card.interfaces[17].transceiver.monitor.temperature.warning_max, &card.interfaces[17].transceiver.monitor.temperature.warning_min, &card.interfaces[17].transceiver.monitor.temperature.critical_min, &card.interfaces[17].transceiver.monitor.temperature.physical_min, '\0', (uint32_t*)&card.interfaces[17].monitor,};
params_t2_add_t table2_ln2_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor,};
params_t2_add_t table2_ln2_odu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor,};
params_t2_add_t table2_ln2_prefecber_add =	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[17].port.monitor.fec_statistics.fec_load.physical_max, &card.interfaces[17].port.monitor.fec_statistics.fec_load.critical_max, &card.interfaces[17].port.monitor.fec_statistics.fec_load.warning_max, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor,};
params_t2_add_t table2_ln2_postfecber_add =	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_POROG, &card.interfaces[17].port.monitor.fec_statistics.fec_load.physical_max, &card.interfaces[17].port.monitor.fec_statistics.fec_load.critical_max, &card.interfaces[17].port.monitor.fec_statistics.fec_load.warning_max, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[17].monitor,};

params_t2_add_t table2_cl1_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[0].monitor,};
params_t2_add_t table2_cl1_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[0].monitor,};
params_t2_add_t table2_cl1_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[0].monitor};
params_t2_add_t table2_cl1_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[0].monitor,};
params_t2_add_t table2_cl1_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[0].monitor,};
params_t2_add_t table2_cl1_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[0].monitor,};
params_t2_add_t table2_cl1_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[0].monitor,};

params_t2_add_t table2_cl2_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[1].monitor,};
params_t2_add_t table2_cl2_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[1].monitor,};
params_t2_add_t table2_cl2_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[1].monitor};
params_t2_add_t table2_cl2_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[1].monitor,};
params_t2_add_t table2_cl2_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[1].monitor,};
params_t2_add_t table2_cl2_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[1].monitor,};
params_t2_add_t table2_cl2_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[1].monitor,};

params_t2_add_t table2_cl3_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[2].monitor,};
params_t2_add_t table2_cl3_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[2].monitor,};
params_t2_add_t table2_cl3_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[2].monitor};
params_t2_add_t table2_cl3_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[2].monitor,};
params_t2_add_t table2_cl3_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[2].monitor,};
params_t2_add_t table2_cl3_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[2].monitor,};
params_t2_add_t table2_cl3_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[2].monitor,};

params_t2_add_t table2_cl4_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[3].monitor,};
params_t2_add_t table2_cl4_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[3].monitor,};
params_t2_add_t table2_cl4_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[3].monitor};
params_t2_add_t table2_cl4_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[3].monitor,};
params_t2_add_t table2_cl4_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[3].monitor,};
params_t2_add_t table2_cl4_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[3].monitor,};
params_t2_add_t table2_cl4_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[3].monitor,};

params_t2_add_t table2_cl5_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[4].monitor,};
params_t2_add_t table2_cl5_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[4].monitor,};
params_t2_add_t table2_cl5_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[4].monitor};
params_t2_add_t table2_cl5_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[4].monitor,};
params_t2_add_t table2_cl5_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[4].monitor,};
params_t2_add_t table2_cl5_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[4].monitor,};
params_t2_add_t table2_cl5_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[4].monitor,};

params_t2_add_t table2_cl6_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[5].monitor,};
params_t2_add_t table2_cl6_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[5].monitor,};
params_t2_add_t table2_cl6_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[5].monitor};
params_t2_add_t table2_cl6_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[5].monitor,};
params_t2_add_t table2_cl6_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[5].monitor,};
params_t2_add_t table2_cl6_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[5].monitor,};
params_t2_add_t table2_cl6_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[5].monitor,};

params_t2_add_t table2_cl7_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[6].monitor,};
params_t2_add_t table2_cl7_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[6].monitor,};
params_t2_add_t table2_cl7_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[6].monitor};
params_t2_add_t table2_cl7_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[6].monitor,};
params_t2_add_t table2_cl7_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[6].monitor,};
params_t2_add_t table2_cl7_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[6].monitor,};
params_t2_add_t table2_cl7_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[6].monitor,};

params_t2_add_t table2_cl8_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[7].monitor,};
params_t2_add_t table2_cl8_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[7].monitor,};
params_t2_add_t table2_cl8_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[7].monitor};
params_t2_add_t table2_cl8_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[7].monitor,};
params_t2_add_t table2_cl8_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[7].monitor,};
params_t2_add_t table2_cl8_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[7].monitor,};
params_t2_add_t table2_cl8_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[7].monitor,};

params_t2_add_t table2_cl9_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[8].monitor,};
params_t2_add_t table2_cl9_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[8].monitor,};
params_t2_add_t table2_cl9_sfpp_pres = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[8].monitor};
params_t2_add_t table2_cl9_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[8].monitor,};
params_t2_add_t table2_cl9_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[8].monitor,};
params_t2_add_t table2_cl9_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[8].monitor,};
params_t2_add_t table2_cl9_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[8].monitor,};

params_t2_add_t table2_cl10_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[9].monitor,};
params_t2_add_t table2_cl10_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[9].monitor,};
params_t2_add_t table2_cl10_sfpp_pres = 	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[9].monitor};
params_t2_add_t table2_cl10_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[9].monitor,};
params_t2_add_t table2_cl10_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[9].monitor,};
params_t2_add_t table2_cl10_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[9].monitor,};
params_t2_add_t table2_cl10_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[9].monitor,};

params_t2_add_t table2_cl11_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[10].monitor,};
params_t2_add_t table2_cl11_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[10].monitor,};
params_t2_add_t table2_cl11_sfpp_pres = 	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[10].monitor};
params_t2_add_t table2_cl11_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[10].monitor,};
params_t2_add_t table2_cl11_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[10].monitor,};
params_t2_add_t table2_cl11_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[10].monitor,};
params_t2_add_t table2_cl11_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[10].monitor,};

params_t2_add_t table2_cl12_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[11].monitor,};
params_t2_add_t table2_cl12_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[11].monitor,};
params_t2_add_t table2_cl12_sfpp_pres = 	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[11].monitor};
params_t2_add_t table2_cl12_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[11].monitor,};
params_t2_add_t table2_cl12_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[11].monitor,};
params_t2_add_t table2_cl12_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[11].monitor,};
params_t2_add_t table2_cl12_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[11].monitor,};

params_t2_add_t table2_cl13_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[12].monitor,};
params_t2_add_t table2_cl13_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[12].monitor,};
params_t2_add_t table2_cl13_sfpp_pres = 	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[12].monitor};
params_t2_add_t table2_cl13_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[12].monitor,};
params_t2_add_t table2_cl13_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[12].monitor,};
params_t2_add_t table2_cl13_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[12].monitor,};
params_t2_add_t table2_cl13_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[12].monitor,};

params_t2_add_t table2_cl14_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[13].monitor,};
params_t2_add_t table2_cl14_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[13].monitor,};
params_t2_add_t table2_cl14_sfpp_pres = 	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[13].monitor};
params_t2_add_t table2_cl14_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[13].monitor,};
params_t2_add_t table2_cl14_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[13].monitor,};
params_t2_add_t table2_cl14_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[13].monitor,};
params_t2_add_t table2_cl14_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[13].monitor,};

params_t2_add_t table2_cl15_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[14].monitor,};
params_t2_add_t table2_cl15_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[14].monitor,};
params_t2_add_t table2_cl15_sfpp_pres = 	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[14].monitor};
params_t2_add_t table2_cl15_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[14].monitor,};
params_t2_add_t table2_cl15_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[14].monitor,};
params_t2_add_t table2_cl15_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[14].monitor,};
params_t2_add_t table2_cl15_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[14].monitor,};

params_t2_add_t table2_cl16_pin_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[15].monitor,};
params_t2_add_t table2_cl16_pout_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[15].monitor,};
params_t2_add_t table2_cl16_sfpp_pres = 	{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[15].monitor};
params_t2_add_t table2_cl16_temp_add = 		{COLOR_Normal, COLOR_Minor, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0', (uint32_t*)&card.interfaces[15].monitor,};
params_t2_add_t table2_cl16_otu_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[15].monitor,};
params_t2_add_t table2_cl16_sdh_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[15].monitor,};
params_t2_add_t table2_cl16_eth_alrm_add =	{COLOR_Normal, COLOR_NA, COLOR_Critical, COLOR_TIP_GET, NULL, NULL, NULL, NULL, NULL, NULL, '\0',(uint32_t*)&card.interfaces[15].monitor,};

extern uint32_t block_color;
uint32_t dummy_aps = 0;

profile_param_t section_2[] =/* __attribute__((section(".prof"))) = */
{
	/*General parameters */
	{ ID_2_TIME,				0,	0,	DC_U32,		85,	 &card.sys_uptime,        											sizeof(uint32_t), 	NULL,	NULL, 	NULL,	NULL },
	{ ID_2_ALARM1,				0,	0,	DC_BOOL,	85,	 &card.alarm1.current,  											sizeof(uint32_t), 	NULL, 	NULL, 	&table2_alarm1_add, 	NULL },
	{ ID_2_ALARM2,				0,	0,	DC_BOOL,	85,	 &card.alarm2.current,  											sizeof(uint32_t), 	NULL,	NULL, 	&table2_alarm2_add, 	NULL },
	{ ID_2_BLOCK_COLOR,			0,	0,	DC_S32,		80,	 &block_color, 														sizeof(uint32_t), 	NULL,	NULL, 	NULL, 	NULL },
	{ ID_2_BLOCK_STATE,			0,	0,	DC_ENUM,	80,	 &card.block_state.current, 										sizeof(uint32_t), 	NULL,	NULL, 	NULL, 	NULL },
	{ ID_2_BLOCK_TEMP,			0,	0,	DC_FLOAT,	85,	 &card.temperature.value,											sizeof(float_t), 	NULL,	NULL, 	&table2_tcase_add,	    "%.1f" },
#ifdef KURS_APS
	{ ID_2_AGG_APS_REQUEST,	    0,	0,	DC_ENUM,	85,	 &_aps.assigned_aps_request.current,								sizeof(uint32_t), 	NULL,	NULL, 	NULL,	    NULL },
	{ ID_2_AGG_APS_SIGNAL,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL,	NULL, 	NULL,	    NULL },
#endif
	/*Line 1 port parameters */
	{ ID_2_LN1_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_ln1_pin_add,	"%.1f" },
	{ ID_2_LN1_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_ln1_pout_add,	"%.1f" },
	{ ID_2_LN1_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_ln1_otu_alrm_add,NULL },
	{ ID_2_LN1_ODU_ALRMS,		0,	0,	DC_BITMSK,	85,	 &odux_alarms[16],													sizeof(uint32_t), 	NULL, 	NULL, 	&table2_ln1_odu_alrm_add,NULL },
	{ ID_2_LN1_PREFECBER,		0,	0,	DC_FLOAT,	85,  &card.interfaces[16].port.monitor.fec_statistics.prefec_ber,		sizeof(float), 		NULL, 	NULL, 	&table2_ln1_prefecber_add,	"%.1f"},
	{ ID_2_LN1_POSTFECBER,		0,	0,	DC_FLOAT,	85,  &card.interfaces[16].port.monitor.fec_statistics.postfec_ber,		sizeof(float), 		NULL, 	NULL, 	&table2_ln1_postfecber_add,	"%.1f"},
	{ ID_2_LN1_FEC_LOAD,		0,	0,	DC_FLOAT,	85,  &card.interfaces[16].port.monitor.fec_statistics.fec_load.value,	sizeof(float), 		NULL, 	NULL, 	&table2_ln1_fecload_add,	"%.1f"},
	{ ID_2_LN1_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_ln1_sfp_pres,	NULL },
	{ ID_2_LN1_SFP_TEMP,		0,	0,	DC_FLOAT,	85,	 &card.interfaces[16].transceiver.monitor.temperature.value,		sizeof(float_t), 	NULL,	NULL, 	&table2_ln1_temp_add,	"%.1f" },
	{ ID_2_LN1_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Line 2 port parameters */
	{ ID_2_LN2_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_ln2_pin_add,	"%.1f" },
	{ ID_2_LN2_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_ln2_pout_add,	"%.1f" },
	{ ID_2_LN2_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_ln2_otu_alrm_add,NULL },
	{ ID_2_LN2_ODU_ALRMS,		0,	0,	DC_BITMSK,	85,	 &odux_alarms[17],													sizeof(uint32_t), 	NULL, 	NULL, 	&table2_ln2_odu_alrm_add,NULL },
	{ ID_2_LN2_PREFECBER,		0,	0,	DC_FLOAT,	85,  &card.interfaces[17].port.monitor.fec_statistics.prefec_ber,	    sizeof(float), 		NULL, 	NULL, 	&table2_ln2_prefecber_add,	"%.1f"},
	{ ID_2_LN2_POSTFECBER,	    0,	0,	DC_FLOAT,	85,  &card.interfaces[17].port.monitor.fec_statistics.postfec_ber,	    sizeof(float), 		NULL, 	NULL, 	&table2_ln2_postfecber_add,	"%.1f"},
	{ ID_2_LN2_FEC_LOAD,		0,	0,	DC_FLOAT,	85,  &card.interfaces[17].port.monitor.fec_statistics.fec_load.value,	sizeof(float), 		NULL, 	NULL, 	&table2_ln2_fecload_add,	"%.1f"},
	{ ID_2_LN2_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,				                                        sizeof(uint32_t), 	NULL, 	NULL, 	&table2_ln2_sfp_pres,	NULL },
	{ ID_2_LN2_SFP_TEMP,		0,	0,	DC_FLOAT,	85,	 &card.interfaces[17].transceiver.monitor.temperature.value,		sizeof(float_t), 	NULL,	NULL, 	&table2_ln2_temp_add,	"%.1f" },
	{ ID_2_LN2_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 1 port parameters */
	{ ID_2_CL1_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl1_pin_add,	"%.1f" },
	{ ID_2_CL1_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl1_pout_add,	"%.1f" },
	{ ID_2_CL1_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl1_otu_alrm_add,NULL },
	{ ID_2_CL1_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl1_sdh_alrm_add,NULL },
	{ ID_2_CL1_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl1_eth_alrm_add,NULL },
	{ ID_2_CL1_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl1_sfpp_pres,	NULL },
	{ ID_2_CL1_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl1_temp_add,	"%.1f" },
	{ ID_2_CL1_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 2 port parameters */
	{ ID_2_CL2_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl2_pin_add,	"%.1f" },
	{ ID_2_CL2_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl2_pout_add,	"%.1f" },
	{ ID_2_CL2_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl2_otu_alrm_add,NULL },
	{ ID_2_CL2_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl2_sdh_alrm_add,NULL },
	{ ID_2_CL2_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl2_eth_alrm_add,NULL },
	{ ID_2_CL2_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl2_sfpp_pres,	NULL },
	{ ID_2_CL2_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl2_temp_add,	"%.1f" },
	{ ID_2_CL2_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 3 port parameters */
	{ ID_2_CL3_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl3_pin_add,	"%.1f" },
	{ ID_2_CL3_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl3_pout_add,	"%.1f" },
	{ ID_2_CL3_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl3_otu_alrm_add,NULL },
	{ ID_2_CL3_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl3_sdh_alrm_add,NULL },
	{ ID_2_CL3_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl3_eth_alrm_add,NULL },
	{ ID_2_CL3_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl3_sfpp_pres,	NULL },
	{ ID_2_CL3_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl3_temp_add,	"%.1f" },
	{ ID_2_CL3_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 4 port parameters */
	{ ID_2_CL4_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl4_pin_add,	"%.1f" },
	{ ID_2_CL4_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl4_pout_add,	"%.1f" },
	{ ID_2_CL4_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl4_otu_alrm_add,NULL },
	{ ID_2_CL4_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl4_sdh_alrm_add,NULL },
	{ ID_2_CL4_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl4_eth_alrm_add,NULL },
	{ ID_2_CL4_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl4_sfpp_pres,	NULL },
	{ ID_2_CL4_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl4_temp_add,	"%.1f" },
	{ ID_2_CL4_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 5 port parameters */
	{ ID_2_CL5_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl5_pin_add,	"%.1f" },
	{ ID_2_CL5_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl5_pout_add,	"%.1f" },
	{ ID_2_CL5_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl5_otu_alrm_add,NULL },
	{ ID_2_CL5_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl5_sdh_alrm_add,NULL },
	{ ID_2_CL5_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl5_eth_alrm_add,NULL },
	{ ID_2_CL5_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl5_sfpp_pres,	NULL },
	{ ID_2_CL5_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl5_temp_add,	"%.1f" },
	{ ID_2_CL5_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 6 port parameters */
	{ ID_2_CL6_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl6_pin_add,	"%.1f" },
	{ ID_2_CL6_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl6_pout_add,	"%.1f" },
	{ ID_2_CL6_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl6_otu_alrm_add,NULL },
	{ ID_2_CL6_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl6_sdh_alrm_add,NULL },
	{ ID_2_CL6_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl6_eth_alrm_add,NULL },
	{ ID_2_CL6_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl6_sfpp_pres,	NULL },
	{ ID_2_CL6_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl6_temp_add,	"%.1f" },
	{ ID_2_CL6_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 7 port parameters */
	{ ID_2_CL7_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl7_pin_add,	"%.1f" },
	{ ID_2_CL7_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl7_pout_add,	"%.1f" },
	{ ID_2_CL7_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl7_otu_alrm_add,NULL },
	{ ID_2_CL7_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl7_sdh_alrm_add,NULL },
	{ ID_2_CL7_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl7_eth_alrm_add,NULL },
	{ ID_2_CL7_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl7_sfpp_pres,	NULL },
	{ ID_2_CL7_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl7_temp_add,	"%.1f" },
	{ ID_2_CL7_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 8 port parameters */
	{ ID_2_CL8_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl8_pin_add,	"%.1f" },
	{ ID_2_CL8_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl8_pout_add,	"%.1f" },
	{ ID_2_CL8_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl8_otu_alrm_add,NULL },
	{ ID_2_CL8_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl8_sdh_alrm_add,NULL },
	{ ID_2_CL8_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl8_eth_alrm_add,NULL },
	{ ID_2_CL8_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl8_sfpp_pres,	NULL },
	{ ID_2_CL8_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl8_temp_add,	"%.1f" },
	{ ID_2_CL8_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 9 port parameters */
	{ ID_2_CL9_PIN,				1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl9_pin_add,	"%.1f" },
	{ ID_2_CL9_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl9_pout_add,	"%.1f" },
	{ ID_2_CL9_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl9_otu_alrm_add,NULL },
	{ ID_2_CL9_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl9_sdh_alrm_add,NULL },
	{ ID_2_CL9_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl9_eth_alrm_add,NULL },
	{ ID_2_CL9_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl9_sfpp_pres,	NULL },
	{ ID_2_CL9_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl9_temp_add,	"%.1f" },
	{ ID_2_CL9_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 10 port parameters */
	{ ID_2_CL10_PIN,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl10_pin_add,	"%.1f" },
	{ ID_2_CL10_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl10_pout_add,	"%.1f" },
	{ ID_2_CL10_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl10_otu_alrm_add,NULL },
	{ ID_2_CL10_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl10_sdh_alrm_add,NULL },
	{ ID_2_CL10_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl10_eth_alrm_add,NULL },
	{ ID_2_CL10_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl10_sfpp_pres,	NULL },
	{ ID_2_CL10_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl10_temp_add,	"%.1f" },
	{ ID_2_CL10_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 11 port parameters */
	{ ID_2_CL11_PIN,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl11_pin_add,	"%.1f" },
	{ ID_2_CL11_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl11_pout_add,	"%.1f" },
	{ ID_2_CL11_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl11_otu_alrm_add,NULL },
	{ ID_2_CL11_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl11_sdh_alrm_add,NULL },
	{ ID_2_CL11_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl11_eth_alrm_add,NULL },
	{ ID_2_CL11_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl11_sfpp_pres,	NULL },
	{ ID_2_CL11_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl11_temp_add,	"%.1f" },
	{ ID_2_CL11_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 12 port parameters */
	{ ID_2_CL12_PIN,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl12_pin_add,	"%.1f" },
	{ ID_2_CL12_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl12_pout_add,	"%.1f" },
	{ ID_2_CL12_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl12_otu_alrm_add,NULL },
	{ ID_2_CL12_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl12_sdh_alrm_add,NULL },
	{ ID_2_CL12_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl12_eth_alrm_add,NULL },
	{ ID_2_CL12_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl12_sfpp_pres,	NULL },
	{ ID_2_CL12_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl12_temp_add,	"%.1f" },
	{ ID_2_CL12_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 13 port parameters */
	{ ID_2_CL13_PIN,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl13_pin_add,	"%.1f" },
	{ ID_2_CL13_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl13_pout_add,	"%.1f" },
	{ ID_2_CL13_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl13_otu_alrm_add,NULL },
	{ ID_2_CL13_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl13_sdh_alrm_add,NULL },
	{ ID_2_CL13_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl13_eth_alrm_add,NULL },
	{ ID_2_CL13_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl13_sfpp_pres,	NULL },
	{ ID_2_CL13_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl13_temp_add,	"%.1f" },
	{ ID_2_CL13_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 14 port parameters */
	{ ID_2_CL14_PIN,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl14_pin_add,	"%.1f" },
	{ ID_2_CL14_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl14_pout_add,	"%.1f" },
	{ ID_2_CL14_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl14_otu_alrm_add,NULL },
	{ ID_2_CL14_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl14_sdh_alrm_add,NULL },
	{ ID_2_CL14_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl14_eth_alrm_add,NULL },
	{ ID_2_CL14_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl14_sfpp_pres,	NULL },
	{ ID_2_CL14_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl14_temp_add,	"%.1f" },
	{ ID_2_CL14_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 15 port parameters */
	{ ID_2_CL15_PIN,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl15_pin_add,	"%.1f" },
	{ ID_2_CL15_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl15_pout_add,	"%.1f" },
	{ ID_2_CL15_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl15_otu_alrm_add,NULL },
	{ ID_2_CL15_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl15_sdh_alrm_add,NULL },
	{ ID_2_CL15_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl15_eth_alrm_add,NULL },
	{ ID_2_CL15_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl15_sfpp_pres,	NULL },
	{ ID_2_CL15_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl15_temp_add,	"%.1f" },
	{ ID_2_CL15_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
	/*Client 16 port parameters */
	{ ID_2_CL16_PIN,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl16_pin_add,	"%.1f" },
	{ ID_2_CL16_POUT,			1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL, 	NULL, 	&table2_cl16_pout_add,	"%.1f" },
	{ ID_2_CL16_OTU_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl16_otu_alrm_add,NULL },
	{ ID_2_CL16_SDH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl16_sdh_alrm_add,NULL },
	{ ID_2_CL16_ETH_ALRMS,		1,	0,	DC_BITMSK,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl16_eth_alrm_add,NULL },
	{ ID_2_CL16_SFP_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	&table2_cl16_sfpp_pres,	NULL },
	{ ID_2_CL16_SFP_TEMP,		1,	0,	DC_FLOAT,	85,	 &dummy_float,														sizeof(float_t), 	NULL,	NULL, 	&table2_cl16_temp_add,	"%.1f" },
	{ ID_2_CL16_ALS_STATE,		1,	0,	DC_ENUM,	85,	 &dummy_u32,														sizeof(uint32_t), 	NULL, 	NULL, 	NULL,					NULL },
};

float min_tx_power = -5.00;			/*!<smallest supported ITU frequency */
float max_tx_power = 1.00;			/*!<largest supported ITU frequency */
float ln_desired_tx_freq = 0;
float ln_desired_rx_freq = 0;

uint32_t dummy_aps_set = 0;
profile_param_t section_3[] =/*__attribute__((section(".prof"))) = */
{
	{ ID_3_RESET,				 0,	1, 	DC_ENUM,	213,	&card.reset_settings.current,		sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	{ ID_3_RESET_ALL_COUNTERS,	 0,	1, 	DC_ENUM,	213,	&card.reset_counters.current,		sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	#ifdef KURS_APS
	{ ID_3_AGG_SET_APS_MODE, 			0, 	1, 	DC_ENUM,	245,	&_aps.aps_mode.current,		sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_AGG_SET_APS_PROT_CHANNEL, 	1, 	1, 	DC_ENUM,	245,	&dummy_u32,					sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_AGG_SET_APS_REVERSE_MODE, 	1, 	1, 	DC_ENUM,	245, 	&dummy_u32,					sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_AGG_SET_APS_WTR, 			0, 	1, 	DC_U32,   	245, 	&_aps.wtr_timer,			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_AGG_SET_APS_HO, 				0, 	1, 	DC_U32,		245, 	&_aps.holdoff_timer,		sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
#endif
	/*Line 1 port parameters */
	{ ID_3_LN1_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[16].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_LN1_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_LN1_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[16].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_LN1_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_LN1_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[16].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_LN1_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[16].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Line 2 port parameters */
	{ ID_3_LN2_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[17].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_LN2_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_LN2_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[17].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_LN2_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_LN2_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[17].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_LN2_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[17].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 1 port parameters */
	{ ID_3_CL1_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[0].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL1_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL1_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[0].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL1_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL1_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL1_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[0].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL1_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[0].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 2 port parameters */
	{ ID_3_CL2_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[1].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL2_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL2_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[1].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL2_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL2_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL2_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[1].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL2_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[1].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 3 port parameters */
	{ ID_3_CL3_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[2].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL3_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL3_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[2].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL3_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL3_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL3_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[2].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL3_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[2].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 4 port parameters */
	{ ID_3_CL4_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[3].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL4_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL4_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[3].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL4_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL4_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL4_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[3].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL4_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[3].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 5 port parameters */
	{ ID_3_CL5_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[4].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL5_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL5_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[4].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL5_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL5_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL5_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[4].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL5_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[4].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 6 port parameters */
	{ ID_3_CL6_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[5].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL6_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL6_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[5].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL6_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL6_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL6_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[5].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL6_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[5].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 7 port parameters */
	{ ID_3_CL7_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[6].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL7_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL7_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[6].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL7_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL7_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL7_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[6].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL7_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[6].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 8 port parameters */
	{ ID_3_CL8_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[7].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL8_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL8_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[7].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL8_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL8_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL8_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[7].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL8_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[7].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 9 port parameters */
	{ ID_3_CL9_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[8].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL9_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL9_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[8].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL9_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL9_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL9_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[8].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL9_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[8].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 10 port parameters */
	{ ID_3_CL10_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[9].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL10_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL10_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[9].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL10_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL10_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL10_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[9].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL10_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[9].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 11 port parameters */
	{ ID_3_CL11_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[10].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL11_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL11_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[10].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL11_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL11_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL11_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[10].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL11_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[10].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 12 port parameters */
	{ ID_3_CL12_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[11].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL12_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL12_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[11].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL12_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL12_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL12_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[11].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL12_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[11].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 13 port parameters */
	{ ID_3_CL13_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[12].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL13_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL13_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[12].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL13_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL13_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL13_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[12].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL13_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[12].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 14 port parameters */
	{ ID_3_CL14_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[13].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL14_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL14_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[13].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL14_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL14_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL14_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[13].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL14_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[13].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 15 port parameters */
	{ ID_3_CL15_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[14].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL15_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL15_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[14].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL15_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL15_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL15_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[14].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL15_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[14].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
	/*Client 16 port parameters */
	{ ID_3_CL16_SET_STATE,        0, 1, DC_ENUM,    245, 	&card.interfaces[15].service_state.current,      		       			sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL16_ENABLE_TX,        1, 1, DC_ENUM,    245, 	&dummy_u32,																sizeof(uint32_t), 	    NULL, 			NULL,    		    NULL, 	NULL },
	{ ID_3_CL16_SET_LOOPBACK,     0, 1, DC_ENUM,    245, 	&card.interfaces[15].port.control.loopback_mode.current,					sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL16_SET_DATA_TYPE,    1, 1, DC_ENUM,    245, 	&dummy_u32,   															sizeof(uint32_t), 	    NULL,           NULL,               NULL, 	NULL },
	{ ID_3_CL16_SET_ALS_MODE,     1, 1, DC_ENUM,    245, 	&dummy_u32,  		            										sizeof(uint32_t), 	    NULL, 			NULL,    			NULL, 	NULL },
	{ ID_3_CL16_SFP_THRS_RESET,   0, 1, DC_ENUM,    245, 	&card.interfaces[15].transceiver.control.threshold_mode_reset.current,  	sizeof(uint32_t), 	    NULL,           NULL,           	NULL, 	NULL },
	{ ID_3_CL16_RESET_COUNTERS,	 0,	1, DC_ENUM,	   213,		&card.interfaces[15].port.control.reset_counters.current,				sizeof(uint32_t),		NULL,			NULL,				NULL, 	NULL},
};


extern char 		pt_cl_1_in_info[PORT_INFO_SIZE];    /*!<info string for port 1 in */
extern uint32_t 	pt_cl_1_in_cat;                 /*!<cat index for   port 1 in */
extern char 		pt_cl_2_in_info[PORT_INFO_SIZE];    /*!<info string for port 2 in */
extern uint32_t 	pt_cl_2_in_cat;                 /*!<cat index for   port 2 in */
extern char 		pt_cl_3_in_info[PORT_INFO_SIZE];    /*!<info string for port 3 in */
extern uint32_t 	pt_cl_3_in_cat;                 /*!<cat index for   port 3 in */
extern char 		pt_cl_4_in_info[PORT_INFO_SIZE];    /*!<info string for port 4 in */
extern uint32_t 	pt_cl_4_in_cat;                 /*!<cat index for   port 4 in */
extern char 		pt_cl_5_in_info[PORT_INFO_SIZE];    /*!<info string for port 5 in */
extern uint32_t 	pt_cl_5_in_cat;                 /*!<cat index for   port 5 in */
extern char 		pt_cl_6_in_info[PORT_INFO_SIZE];    /*!<info string for port 6 in */
extern uint32_t 	pt_cl_6_in_cat;                 /*!<cat index for   port 6 in */
extern char 		pt_cl_7_in_info[PORT_INFO_SIZE];    /*!<info string for port 7 in */
extern uint32_t 	pt_cl_7_in_cat;                 /*!<cat index for   port 7 in */
extern char 		pt_cl_8_in_info[PORT_INFO_SIZE];    /*!<info string for port 8 in */
extern uint32_t 	pt_cl_8_in_cat;                 /*!<cat index for   port 8 in */
extern char 		pt_cl_9_in_info[PORT_INFO_SIZE];    /*!<info string for port 9 in */
extern uint32_t 	pt_cl_9_in_cat;                 /*!<cat index for   port 9 in */
extern char 		pt_cl_10_in_info[PORT_INFO_SIZE];    /*!<info string for port 10 in */
extern uint32_t 	pt_cl_10_in_cat;                 /*!<cat index for   port 10 in */
extern char 		pt_cl_11_in_info[PORT_INFO_SIZE];    /*!<info string for port 11 in */
extern uint32_t 	pt_cl_11_in_cat;                 /*!<cat index for   port 11 in */
extern char 		pt_cl_12_in_info[PORT_INFO_SIZE];    /*!<info string for port 12 in */
extern uint32_t 	pt_cl_12_in_cat;                 /*!<cat index for   port 12 in */
extern char 		pt_cl_13_in_info[PORT_INFO_SIZE];    /*!<info string for port 13 in */
extern uint32_t 	pt_cl_13_in_cat;                 /*!<cat index for   port 13 in */
extern char 		pt_cl_14_in_info[PORT_INFO_SIZE];    /*!<info string for port 14 in */
extern uint32_t 	pt_cl_14_in_cat;                 /*!<cat index for   port 14 in */
extern char 		pt_cl_15_in_info[PORT_INFO_SIZE];    /*!<info string for port 15 in */
extern uint32_t 	pt_cl_15_in_cat;                 /*!<cat index for   port 15 in */
extern char 		pt_cl_16_in_info[PORT_INFO_SIZE];    /*!<info string for port 16 in */
extern uint32_t 	pt_cl_16_in_cat;                 /*!<cat index for   port 16 in */
extern char 		pt_ln_1_in_info[PORT_INFO_SIZE];      /*!<info string for line 1 in */
extern uint32_t 	pt_ln_1_in_cat;                   /*!<cat index for   line 1 in */
extern char 		pt_ln_2_in_info[PORT_INFO_SIZE];      /*!<info string for line 2 in */
extern uint32_t 	pt_ln_2_in_cat;                   /*!<cat index for   line 2 in */

uint32_t u32MinMax99 = 99;

/**List of profile section 103 parameters.
 *
 */
profile_param_t section_103[] =/*__attribute__((section(".prof"))) =*/
{
	{ ID_103_PORT_LN1_IN_INFO, 		0, 0, DC_STR, 245, pt_ln_1_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_LN1_IN_CAT, 		0, 0, DC_U32, 245, &pt_ln_1_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_LN2_IN_INFO, 		0, 0, DC_STR, 245, pt_ln_2_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_LN2_IN_CAT, 		0, 0, DC_U32, 245, &pt_ln_2_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_1_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_1_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_1_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_1_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_2_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_2_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_2_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_2_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_3_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_3_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_3_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_3_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_4_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_4_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_4_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_4_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_5_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_5_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_5_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_5_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_6_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_6_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_6_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_6_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_7_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_7_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_7_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_7_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_8_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_8_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_8_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_8_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_9_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_9_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_9_IN_CAT, 		0, 0, DC_U32, 245, &pt_cl_9_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_10_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_10_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_10_IN_CAT, 	0, 0, DC_U32, 245, &pt_cl_10_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_11_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_11_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_11_IN_CAT, 	0, 0, DC_U32, 245, &pt_cl_11_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_12_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_12_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_12_IN_CAT, 	0, 0, DC_U32, 245, &pt_cl_12_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_13_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_13_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_13_IN_CAT, 	0, 0, DC_U32, 245, &pt_cl_13_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_14_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_14_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_14_IN_CAT, 	0, 0, DC_U32, 245, &pt_cl_14_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_15_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_15_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_15_IN_CAT, 	0, 0, DC_U32, 245, &pt_cl_15_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
	{ ID_103_PORT_CL_16_IN_INFO, 	0, 0, DC_STR, 245, pt_cl_16_in_info, 	(PORT_INFO_SIZE), 	NULL, NULL, 			NULL, NULL },
	{ ID_103_PORT_CL_16_IN_CAT, 	0, 0, DC_U32, 245, &pt_cl_16_in_cat, 	sizeof(uint32_t), 	NULL, &u32MinMax99, 	NULL, NULL },
};

float s32_min_max_0 = 0;
float s32_min_max_40n = -40;
float s32_min_max_99 = 99;
float s32_min_max_100 = 100;

/**List of profile section 4 parameters.
 *
 */
profile_param_t section_4[] =/*__attribute__((section(".prof")))=*/
{
	{ ID_4_BLOCK_TEMP_CMin, 	0, 	0, 	DC_FLOAT, 	245, 	&card.temperature.critical_min,											sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f", },
	{ ID_4_BLOCK_TEMP_WMin, 	0,	0, 	DC_FLOAT, 	245, 	&card.temperature.warning_min,											sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f", },
	{ ID_4_BLOCK_TEMP_WMax, 	0, 	0, 	DC_FLOAT, 	245, 	&card.temperature.warning_max, 											sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_BLOCK_TEMP_CMax,	 	0, 	0, 	DC_FLOAT, 	245, 	&card.temperature.critical_max, 										sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[16].critical_min, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_LN1_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[16].warning_min,	 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[16].warning_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[16].critical_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[16].critical_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[16].warning_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[16].warning_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[16].critical_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_FEC_LOAD_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].port.monitor.fec_statistics.fec_load.critical_min, sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN1_FEC_LOAD_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].port.monitor.fec_statistics.fec_load.warning_min, 	sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN1_FEC_LOAD_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].port.monitor.fec_statistics.fec_load.warning_max, 	sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN1_FEC_LOAD_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].port.monitor.fec_statistics.fec_load.critical_max, sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN1_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN1_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[16].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[17].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_LN2_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[17].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[17].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[17].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[17].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[17].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[17].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[17].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_FEC_LOAD_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].port.monitor.fec_statistics.fec_load.critical_min, sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN2_FEC_LOAD_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].port.monitor.fec_statistics.fec_load.warning_min, 	sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN2_FEC_LOAD_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].port.monitor.fec_statistics.fec_load.warning_max, 	sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN2_FEC_LOAD_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].port.monitor.fec_statistics.fec_load.critical_max, sizeof(float), 				(void *)&s32_min_max_0, 	(void *)&s32_min_max_100, 	NULL, 	"%.1f" },
	{ ID_4_LN2_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_LN2_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[17].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[0].critical_min, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL1_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[0].warning_min,	 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[0].warning_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[0].critical_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[0].critical_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[0].warning_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[0].warning_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[0].critical_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[0].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[0].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[0].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL1_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[0].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[1].critical_min, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL2_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[1].warning_min,	 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[1].warning_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[1].critical_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[1].critical_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[1].warning_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[1].warning_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[1].critical_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[1].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[1].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[1].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL2_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[1].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[2].critical_min, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL3_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[2].warning_min,	 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[2].warning_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[2].critical_max, 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[2].critical_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[2].warning_min,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[2].warning_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[2].critical_max,													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[2].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[2].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[2].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL3_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[2].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[3].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[3].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[3].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[3].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[3].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL4_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[3].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[3].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[3].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[3].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[3].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[3].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL4_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[3].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[4].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[4].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[4].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[4].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[4].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL5_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[4].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[4].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[4].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[4].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[4].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[4].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL5_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[4].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[5].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[5].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[5].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[5].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[5].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL6_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[5].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[5].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[5].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[5].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[5].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[5].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL6_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[5].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[6].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[6].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[6].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[6].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[6].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL7_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[6].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[6].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[6].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[6].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[6].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[6].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL7_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[6].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[7].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[7].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[7].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[7].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[7].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL8_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[7].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[7].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[7].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[7].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[7].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[7].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL8_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[7].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[8].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[8].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[8].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[8].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[8].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL9_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[8].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[8].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[8].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[8].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[8].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[8].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL9_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[8].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[9].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[9].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[9].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[9].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[9].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL10_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[9].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[9].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[9].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[9].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[9].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[9].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL10_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[9].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[10].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[10].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[10].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[10].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[10].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL11_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[10].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[10].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[10].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[10].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[10].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[10].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL11_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[10].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[11].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[11].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[11].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[11].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[11].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL12_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[11].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[11].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[11].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[11].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[11].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[11].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL12_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[11].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[12].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[12].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[12].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[12].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[12].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL13_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[12].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[12].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[12].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[12].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[12].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[12].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL13_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[12].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[13].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[13].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[13].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[13].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[13].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL14_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[13].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[13].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[13].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[13].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[13].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[13].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL14_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[13].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[14].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[14].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[14].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[14].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[14].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL15_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[14].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[14].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[14].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[14].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[14].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[14].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL15_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[14].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_PIN_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[15].critical_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_PIN_WMIN,	 	0, 	1, 	DC_FLOAT, 	245, 	&pin[15].warning_min,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_PIN_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pin[15].warning_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_PIN_CMAX, 		0,	1, 	DC_FLOAT, 	245, 	&pin[15].critical_max,														sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_POUT_CMIN, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[15].critical_min, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99,	NULL, 	"%.1f" },
	{ ID_4_CL16_POUT_WMIN, 		0, 	1,	DC_FLOAT, 	245, 	&pout[15].warning_min,		 												sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_POUT_WMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[15].warning_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_POUT_CMAX, 		0, 	1, 	DC_FLOAT, 	245, 	&pout[15].critical_max, 	 													sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_SFP_TEMP_CMIN, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[15].transceiver.monitor.temperature.critical_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_SFP_TEMP_WMIN,	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[15].transceiver.monitor.temperature.warning_min, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_SFP_TEMP_WMAX, 	0, 	0, 	DC_FLOAT, 	245, 	&card.interfaces[15].transceiver.monitor.temperature.warning_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
	{ ID_4_CL16_SFP_TEMP_CMAX, 	0,	0, 	DC_FLOAT, 	245, 	&card.interfaces[15].transceiver.monitor.temperature.critical_max, 		sizeof(float), 				(void *)&s32_min_max_40n, 	(void *)&s32_min_max_99, 	NULL, 	"%.1f" },
};

extern board_leds_t board_leds;
/**List of profile section 5 parameters.
 *
 */

profile_param_t section_5[] =
{
	{ ID_5_LED_PWR, 	0, 0, DC_U32, 85, &board_leds.power_led_g.current,  sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_STATUS, 	0, 0, DC_U32, 85, &board_leds.status_led.current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_LN1_TX, 	0, 0, DC_U32, 85, &board_leds.led_tx[16].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_LN1_RX, 	0, 0, DC_U32, 85, &board_leds.led_rx[16].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_LN2_TX, 	0, 0, DC_U32, 85, &board_leds.led_tx[17].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_LN2_RX, 	0, 0, DC_U32, 85, &board_leds.led_rx[17].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL1_TX, 	0, 0, DC_U32, 85, &board_leds.led_tx[0].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL1_RX, 	0, 0, DC_U32, 85, &board_leds.led_rx[0].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL2_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[1].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL2_TX,	0, 0, DC_U32, 85, &board_leds.led_rx[1].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL3_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[2].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL3_TX,	0, 0, DC_U32, 85, &board_leds.led_rx[2].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL4_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[3].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL4_TX, 	0, 0, DC_U32, 85, &board_leds.led_rx[3].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL5_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[4].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL5_TX, 	0, 0, DC_U32, 85, &board_leds.led_rx[4].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL6_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[5].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL6_TX, 	0, 0, DC_U32, 85, &board_leds.led_rx[5].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL7_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[6].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL7_TX, 	0, 0, DC_U32, 85, &board_leds.led_rx[6].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL8_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[7].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL8_TX, 	0, 0, DC_U32, 85, &board_leds.led_rx[7].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL9_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[8].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL9_TX, 	0, 0, DC_U32, 85, &board_leds.led_rx[8].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL10_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[9].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL10_TX, 0, 0, DC_U32, 85, &board_leds.led_rx[9].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL11_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[10].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL11_TX, 0, 0, DC_U32, 85, &board_leds.led_rx[10].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL12_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[11].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL12_TX, 0, 0, DC_U32, 85, &board_leds.led_rx[11].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL13_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[12].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL13_TX, 0, 0, DC_U32, 85, &board_leds.led_rx[12].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL14_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[13].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL14_TX, 0, 0, DC_U32, 85, &board_leds.led_rx[13].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL15_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[14].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL15_TX, 0, 0, DC_U32, 85, &board_leds.led_rx[14].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL16_TX,	0, 0, DC_U32, 85, &board_leds.led_tx[15].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
	{ ID_5_LED_CL16_TX, 0, 0, DC_U32, 85, &board_leds.led_rx[15].current, 	sizeof(uint32_t), NULL, NULL, NULL, NULL },
};

extern uint32_t _otu_eb[INTERFACE_COUNT];
extern uint32_t _otu_es[INTERFACE_COUNT];
extern uint32_t _otu_ses[INTERFACE_COUNT];
extern uint64_t _otu_bbe[INTERFACE_COUNT];
extern uint32_t _otu_uas[INTERFACE_COUNT];
extern uint32_t _sdh_eb[INTERFACE_COUNT];
extern uint32_t _sdh_es[INTERFACE_COUNT];
extern uint32_t _sdh_ses[INTERFACE_COUNT];
extern uint64_t _sdh_bbe[INTERFACE_COUNT];
extern uint32_t _sdh_uas[INTERFACE_COUNT];

profile_param_t section_8[] = {
	{ ID_8_LN_1_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,	NULL },
	{ ID_8_LN_1_OTU_SES,		    1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	NULL },
	{ ID_8_LN_1_OTU_BBE,		    1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	NULL },
	{ ID_8_LN_1_OTU_UAS,		    1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	NULL },
	{ ID_8_LN_2_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,	NULL },
	{ ID_8_LN_2_OTU_SES,		    1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	NULL },
	{ ID_8_LN_2_OTU_BBE,		    1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	NULL },
	{ ID_8_LN_2_OTU_UAS,		    1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	NULL },
	{ ID_8_CL_1_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_1_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_1_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_1_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_1_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_1_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_1_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_1_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_1_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_1_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_1_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_1_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_1_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_2_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_2_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_2_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_2_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_2_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_2_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_2_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_2_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_2_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_2_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_2_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_2_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_2_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_3_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_3_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_3_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_3_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_3_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_3_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_3_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_3_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_3_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_3_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_3_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_3_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_3_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_4_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_4_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_4_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_4_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_4_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_4_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_4_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_4_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_4_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_4_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_4_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_4_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_4_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_5_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_5_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_5_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_5_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_5_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_5_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_5_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_5_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_5_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_5_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_5_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_5_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_5_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_6_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_6_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_6_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_6_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_6_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_6_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_6_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_6_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_6_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_6_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_6_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_6_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_6_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_7_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_7_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_7_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_7_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_7_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_7_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_7_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_7_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_7_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_7_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_7_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_7_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_7_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_8_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_8_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_8_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_8_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_8_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_8_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_8_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_8_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_8_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_8_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_8_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_8_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_8_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_9_OTU_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_9_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_9_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_9_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_9_SDH_ES,				1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_9_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_9_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_9_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_9_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_9_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_9_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_9_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_9_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_10_OTU_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_10_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_10_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_10_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_10_SDH_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_10_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_10_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_10_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_10_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_10_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_10_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_10_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_10_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_11_OTU_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_11_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_11_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_11_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_11_SDH_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_11_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_11_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_11_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_11_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_11_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_11_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_11_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_11_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_12_OTU_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_12_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_12_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_12_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_12_SDH_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_12_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_12_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_12_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_12_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_12_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_12_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_12_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_12_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_13_OTU_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_13_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_13_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_13_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_13_SDH_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_13_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_13_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_13_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_13_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_13_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_13_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_13_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_13_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_14_OTU_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_14_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_14_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_14_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_14_SDH_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_14_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_14_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_14_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_14_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_14_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_14_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_14_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_14_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_15_OTU_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_15_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_15_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_15_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_15_SDH_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_15_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_15_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_15_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_15_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_15_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_15_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_15_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_15_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_16_OTU_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_16_OTU_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_16_OTU_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t),   NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_16_OTU_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t),    NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_16_SDH_ES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL, 	NULL, 	NULL,				NULL },
	{ ID_8_CL_16_SDH_SES,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_16_SDH_BBE,			1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_16_SDH_UAS,			1,	0,	DC_U32,		85,	 &dummy_u32,				sizeof(uint32_t), 	NULL,	NULL, 	NULL,	 			NULL },
	{ ID_8_CL_16_ETH_RX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_16_ETH_RX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_16_ETH_RX_ERRS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL, 	NULL, 	NULL,					NULL },
	{ ID_8_CL_16_ETH_TX_PKTS,		1,	0,	DC_U64,		85,	 &dummy_u64,  				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },
	{ ID_8_CL_16_ETH_TX_OCTS,		1,	0,	DC_U64,		85,	 &dummy_u64,				sizeof(uint64_t), 	NULL,	NULL, 	NULL,	 				NULL },

};
#endif /* KURS_SECTIONS_H_ */
