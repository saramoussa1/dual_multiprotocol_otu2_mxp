/*
 * ipglb_adapter.c
 *
 *  Created on: Jun 6, 2018
 *      Author: dcham
 */

#include "ipglb_adapter.h"
#include "common/inc/ipg_types.h"
#include "common/inc/ipg_pfc.h"
#include "common/inc/ipg_utils.h"
#include "configs.h"
#if ENABLE_SFPP_CODE
#include "common/inc/ipg_sfpp.h"
#endif
#if ENABLE_SFP_CODE
#include "common/inc/ipg_sfp.h"
#endif
#if ENABLE_XFP_CODE
#include "common/inc/ipg_xfp.h"
#endif
#if ENABLE_CFP_CODE
#include "common/inc/ipg_cfp.h"
#endif
#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
#include "common/inc/stm1416.h"
#endif
#ifndef RES
#define RES			(0.0000001f)
#endif
#if ENABLE_ETH_CODE
#include "common/inc/ipg_ge_mac.h"
#include "common/inc/ipg_ge_pcs.h"
#include "common/inc/ipg_gfp.h"
#endif
#include "common/inc/ipglb_transceiver.h"
#include "common/inc/ipglb_port.h"
#include "common/inc/ipglb_interface.h"
#include "common/inc/ipglb_card.h"
#include "mmap.h"
#include"ipglb_otu2_brick_dual.h"

#define LN_MMAP(x)	((line_mmap_t*)(card.interfaces[x+16].port.mmap))
#define CL_MMAP(x)	((client_mmap_t*)(card.interfaces[x].port.mmap))
#define CLEAR_ETH_ALARMS(x)(x.alarms.all &= 0x07)
#define CLEAR_STM_ALARMS(x)(x.alarms.all &= 0xFF)
#define CLEAR_OTU_ALARMS(x)(x.alarms.all &= 0x7F)
#define	PORT_CTRL(x)(card.interfaces[x].port.control
#define TRANSDEV _trans->device
#define PM_FECSTATISTICS _port->monitor.fec_statistics
#define PM_STATSTM  _port->monitor.statistics.stmx_sts
#define PM_STATOTU  _port->monitor.statistics.otux_sts
#define OALARMS(x)  odux_alarms[x]
#define PMSTAT_ETHSTS _port->monitor.statistics.eth_sts
#define CARDINT_TRANS(x) card.interfaces[x].transceiver
#define CARDINT_port(x) card.interfaces[x].port
#define C_FECLOAD(x) CARDINT_port(x).monitor.fec_statistics.fec_load
#define C_TEMPALARM(x) CARDINT_TRANS(x).monitor.temperature.alarm
#define C_TEMPMONITOR(x) CARDINT_TRANS(x).info.capabilities.TEMPERATURE_MONITOR
#define C_OPM(x) CARDINT_TRANS(if_index).info.capabilities.OPTICAL_POWER_MONITOR
#define C_MONPROT(x) CARDINT_port(x).monitor.prot
#define C_ETHSTSALARM(x) CARDINT_port(x).monitor.statistics.eth_sts.alarms
#define C_ETHSTMXALARM(x) CARDINT_port(x).monitor.statistics.stmx_sts.alarms
#define C_ETHOTUXALARM(x) CARDINT_port(x).monitor.statistics.otux_sts.alarms
#define TLM_REG_1_BITS  top_level_misc_reg.config_1->bits
#define TLM_REG_2_BITS  top_level_misc_reg.config_2->bits
//#define FRAMER_MODE(x) LN_MMAP(x)->otu1_mapper->ln_cfg_reg->bits.FRAMER_MODE
/* maximum number of correctable bits in one second*/
#define MAX_EFEC_CORRECTABLE_ONE_SEC	6000000.0
#define OTU2_LINE_RATE_BIT				10709000000.0//10709000000‬
#define FPGA1 (_trans->index < 8)
#define FPGA2 (_trans->index > 7 && _trans->index < 16)

#define PORT_FPGA1 (_port->index < 8)
#define PORT_FPGA2 (_port->index > 7 && _port->index < 16)

/*
 * Local variables
 */
protocol_odux_alarms_t odux_alarms[INTERFACE_COUNT];

extern ipglb_card_t card;
extern int port_rate_change_flag[CLIENTS_IF_COUNT];
extern diagnostics_t pin[INTERFACE_COUNT];
extern diagnostics_t pout[INTERFACE_COUNT];

extern void switch_to_ge(port_t* _port);
extern void switch_to_sonet(port_t* _port);
extern void switch_to_fc1(port_t* _port);
extern void switch_to_fc2(port_t* _port);
extern void switch_to_otu1(port_t* _port);
extern void reconfigure_ge_port(port_t* _port);
/*
 * Local prototypes
 */
static void client_port_mmap(port_t* _port_ptr);
static void line_port_mmap(port_t* _port_ptr);
void reset_if_kurs_power_threshold(interface_t* _if_ptr);


/**
 * This function initializes a port.
 * It is project dependent.
 * @param[in] _port pointer to the port to be initialized
 * Return none
 */
void custom_init_port_data(port_t* _port)
{
	int index = _port->index;
	if((index >= CL_INDEX_START) && (index <= CL_INDEX_END))
	{
		_port->info.operation = PORT_OPERATION_CLIENT;
		/* only in OTUx mode*/
		_port->info.capabilities.FEC_CAPABLE = 0;
		/* not available for ETHERNET rates*/
		_port->info.capabilities.PFC_CAPABLE = 1;
		_port->info.capabilities.MULTI_RATE = 1;
		_port->info.capabilities.ALS_CAPABLE = 1;
		_port->monitor.prot = PORT_PROTOCOL_SDH;
		_port->monitor.rate = PORT_RATE_STM1;

		/* CONTROLs */
		/* STM1 */
		_port->control.rate.current = _port->control.rate.next =
									  PORT_RATE_STM1;
		_port->control.rate.change = 1;
	}
	else
	{
		_port->info.operation = PORT_OPERATION_LINE;
		_port->info.capabilities.FEC_CAPABLE = 1;
		_port->info.capabilities.PFC_CAPABLE = 1;
		_port->info.capabilities.MULTI_RATE = 0;
		_port->info.capabilities.ALS_CAPABLE = 1;
		_port->monitor.prot = PORT_PROTOCOL_OTU;
		_port->control.rate.current = _port->monitor.rate =
									  PORT_RATE_OTU2;
		_port->monitor.fec_statistics.type = FEC_TYPE_EFEC;
	}
	custom_port_mmap(_port);
}

/**
 * This function maps the registers of a port.
 * @param[in] _port pointer to the port object to be mapped.
 * Return none
 */
void custom_port_mmap(port_t* _port)
{
	if(_port->info.operation == PORT_OPERATION_CLIENT)
	{
		client_port_mmap(_port);
	}
	else
	if( _port->info.operation == PORT_OPERATION_LINE)
	{
		line_port_mmap(_port);
	}
	else
	{
		_port->mmap = NULL;
	}
}

/**
 * This functions maps a port's registers to a client's registers
 * @param[in] _port_ptr	pointer to the port	object
 * Return none
 */
static void client_port_mmap(port_t* _port_ptr)
{
	_port_ptr->mmap = malloc(sizeof(client_mmap_t));
	client_mmap_t* cl_mmap = (client_mmap_t*)_port_ptr->mmap;
	/* start mapping */
	alt_u32 base = ODUK_MPIF_BRICK_0_BASE + (_port_ptr->index * 0x00004000);
	/* map STM 1/4 */
#define stm14_base_1 (base + 0x00000400)
	cl_mmap->stm_14_1 = create_stm1416_registers(stm14_base_1);
	/* map STM 1/4 */
#define stm14_base_2 (0x10000000 + base + 0x00000400)
	cl_mmap->stm_14_2 = create_stm1416_registers(stm14_base_2);

	/* map STM 16 */
#define stm16_base_1 (base + 0x00000800)
	cl_mmap->stm_16_1 = create_stm1416_registers(stm16_base_1);
	/* map STM 16 */
#define stm16_base_2 (0x10000000 + base + 0x00000800)
	cl_mmap->stm_16_2 = create_stm1416_registers(stm16_base_2);

#define gemac_base_1 (base + 0x00001000)
	/* map GE MAC*/
	cl_mmap->gemac_1 = create_gemac_registers(gemac_base_1);
#define gemac_base_2 (0x10000000 + base + 0x00001000)
	/* map GE MAC*/
	cl_mmap->gemac_1 = create_gemac_registers(gemac_base_2);

#define gebrick_base_1 (base + 0x00001440)
#define gebrick_base_2 (0x10000000 + base + 0x00001440)
	/* map GE brick*/
	cl_mmap->ge = malloc(sizeof(ge_brick_t));
	/*TODO define proper handler*/
	while(cl_mmap->ge == NULL);
	cl_mmap->ge->general_ctrl_reg_1 =
	(volatile union BRICK_GE_GENERAL_CTRL_REG*) ((gebrick_base_1 + 0x00) |
												  DE_CACHE_EN_BIT);
	cl_mmap->ge->general_ctrl_reg_2 =
	(volatile union BRICK_GE_GENERAL_CTRL_REG*) ((gebrick_base_2 + 0x00) |
												  DE_CACHE_EN_BIT);
	cl_mmap->ge->general_sts_reg_1 =
	(volatile union BRICK_GE_GENERAL_STS_REG*) ((gebrick_base_1 + 0x04) |
												 DE_CACHE_EN_BIT);
	cl_mmap->ge->general_sts_reg_2 =
	(volatile union BRICK_GE_GENERAL_STS_REG*) ((gebrick_base_2 + 0x04) |
												 DE_CACHE_EN_BIT);
	cl_mmap->ge->reset_ctrl_reg_1  =
	(volatile union BRICK_GE_RESET_CTRL_REG*) ((gebrick_base_1 + 0x08) |
												DE_CACHE_EN_BIT);
	cl_mmap->ge->reset_ctrl_reg_2  =
	(volatile union BRICK_GE_RESET_CTRL_REG*) ((gebrick_base_2 + 0x08) |
												DE_CACHE_EN_BIT);
#define gepcs_base_1 (base + 0x00001000)
	/* map GE pcs base*/
	cl_mmap->gepcs_1 = create_gepcs_registers(gepcs_base_1);
#define gepcs_base_2 (0x10000000 + base + 0x00001000)
	/* map GE pcs base*/
	cl_mmap->gepcs_2 = create_gepcs_registers(gepcs_base_2);

	/*#todo check if i will need gmp*/
//#define gmp_base (base + 0x1D000)
//	/*map ODU2 wrapper registers*/
//	cl_mmap->gmp = malloc(sizeof(gmp_aligner_t));
//	/*TODO define proper error handler*/
//	while(cl_mmap->gmp == NULL);
//	cl_mmap->gmp->general_cfg_reg =
//	(volatile union GMP_ALIGNER_GENERAL_CFG_REG*) ((gmp_base + 0x04) |
//													 DE_CACHE_EN_BIT);

/*warning Fix mapping otu1 after giving teddy their reg map*/
#define otu1_base_1 (base + 0x2000)
#define otu1_base_2 (0x10000000 + base + 0x2000)
	//map OTU1 framer registers
	cl_mmap->otu1 = malloc(sizeof(otu1_mapper_registers_t));
	//cl_mmap->otu1->ln_cfg_reg 	 = (volatile union OTU1_MAPPER_LN_CFG_REG*) ((otu1_base + 0x00) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->live_status_1 	 = (volatile union OTU1_LIVE_STS_REG*) 		((otu1_base_1 + 0x20) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->live_status_2 	 = (volatile union OTU1_LIVE_STS_REG*) 		((otu1_base_2 + 0x20) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->int_status_1 	 = (volatile union OTU1_INT_STS_REG*) 		((otu1_base_1 + 0x68) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->int_status_2 	 = (volatile union OTU1_INT_STS_REG*) 		((otu1_base_2 + 0x68) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->uncorr_cw_1 	     = (volatile union OTU1_UNCORR_CW_REG*) 	((otu1_base_1 + 0x6C) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->uncorr_cw_2 	     = (volatile union OTU1_UNCORR_CW_REG*) 	((otu1_base_2 + 0x6C) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->fec_corr_bits_1   = (volatile union OTU1_FEC_CORR_BITS_REG*)	((otu1_base_1 + 0x70) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->fec_corr_bits_2   = (volatile union OTU1_FEC_CORR_BITS_REG*)	((otu1_base_2 + 0x70) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->sm_bip8_1 		 = (volatile union OTU1_SM_BIP8_REG*) 		((otu1_base_1 + 0x74) | DE_CACHE_EN_BIT);
	cl_mmap->otu1->sm_bip8_2 		 = (volatile union OTU1_SM_BIP8_REG*) 		((otu1_base_2 + 0x74) | DE_CACHE_EN_BIT);
/**/
#define odtu_1 (base)
#define odtu_2 (0x10000000 + base)
	cl_mmap->odtu_brick = malloc(sizeof(odtu_brick_register_t));
	while(cl_mmap->odtu_brick == NULL);
	cl_mmap->odtu_brick->odtu_ctrl_reg_1 =
	(volatile union ODTU_CTRL_REG*) ((odtu_1 + 0x00) |
									  DE_CACHE_EN_BIT);
	cl_mmap->odtu_brick->odtu_ctrl_reg_2 =
	(volatile union ODTU_CTRL_REG*) ((odtu_2 + 0x00) |
									  DE_CACHE_EN_BIT);
	cl_mmap->odtu_brick->odtu_sts_reg_1 =
	(volatile union ODTU_STATUS_REG*) ((odtu_1 + 0x04) |
									  DE_CACHE_EN_BIT);
	cl_mmap->odtu_brick->odtu_sts_reg_2 =
	(volatile union ODTU_STATUS_REG*) ((odtu_2 + 0x04) |
									  DE_CACHE_EN_BIT);

	cl_mmap->odtu_brick->als_ctrl_reg_1 =
	(volatile union ALS_CTRL_REG*) ((odtu_1 + 0x44) |
									 DE_CACHE_EN_BIT);

	cl_mmap->odtu_brick->als_ctrl_reg_2 =
	(volatile union ALS_CTRL_REG*) ((odtu_2 + 0x44) |
										 DE_CACHE_EN_BIT);

//	/*map GFP registers*/
//#define gfp_base (odtu_base + 0xC00)
//	/* map GFP base*/
//	CL_GFP = create_gfp_registers(gfp_base);
//	CL_GFP = malloc(sizeof(gfp_registers_t));
//	CL_GFP_STATIONREG = (volatile union GFP_STATION_ADDRESS_REG*)
//			            ((gfp_base + 0x00)	| DE_CACHE_EN_BIT);
//	CL_GFP_CONFIGREG1 = (volatile union GFP_CFG_1_REG*)((gfp_base + 0x04) |
//														 DE_CACHE_EN_BIT);
//	CL_GFP_CONFIGREG2 = (volatile union GFP_CFG_2_REG*)((gfp_base + 0x08) |
//														 DE_CACHE_EN_BIT);
//	CL_GFP_STATUSREG  = (volatile union GFP_STATUS_REG*)((gfp_base + 0x0C) |
//														  DE_CACHE_EN_BIT);
}

/**
 * This functions maps a port's registers to a line's registers
 * @param[in] _port_ptr	pointer to the port object to be mapped
 * Return none
 */
static void line_port_mmap(port_t* _port_ptr)
{
	_port_ptr->mmap = malloc(sizeof(line_mmap_t));
	line_mmap_t* line_mmap = (line_mmap_t*)_port_ptr->mmap;
	alt_u32 ln_index = _port_ptr->index - LN_INDEX_OFFSET;
	//OTU2 brick registers, including top, framers and APS
	if(ln_index == LN_INDEX_START)
	{
		map_otu2_brick_dual_regs(&line_mmap->otu2_brick_1, OTU2_BRICK_BASE_LINE_1);
	}
	else
	{
		if(ln_index == LN_INDEX_END)
		{
			map_otu2_brick_dual_regs(&line_mmap->otu2_brick_2, OTU2_BRICK_BASE_LINE_2);
		}
	}
}

/**
 * This function initializes a transceiver.
 * @param[in] _trans pointer to the transceiver object to be mapped.
 * Return none
 */
void custom_init_transceiver_data(transceiver_t* _trans)
{
	/*All interfaces use SFP transceivers*/
	/*map physical device (SFP)*/
	TRANSDEV.device = malloc(sizeof(sfp_device_t));
	((sfp_device_t*)_trans->device.device)->index = _trans->index;
	init_sfp_data((sfp_device_t*)_trans->device.device);
	/*default transceiver initialization*/
	_trans->info.type = TRANS_SFP_OPTICAL;
	_trans->info.capabilities.tx_tunable = 0;
	_trans->info.capabilities.rx_tunable = 0;
	_trans->info.capabilities.TEMPERATURE_MONITOR = 1;
	_trans->info.capabilities.OPTICAL_POWER_MONITOR = 1;
	_trans->info.capabilities.tx_power_control = 0;
	_trans->info.capabilities.optical_ios_number = 1;
	/* map transceiver IOs*/
	if((_trans->index >= CL_INDEX_START) && (_trans->index <= CL_INDEX_END))
	{
		if FPGA1
		{
			/* get client mmap*/
			client_mmap_t* cl_mmap  =
			(client_mmap_t*)card.interfaces[_trans->index].port.mmap;
			TRANSDEV.absent.address =(uint32_t)
									 (cl_mmap->odtu_brick->odtu_sts_reg_1);
			TRANSDEV.absent.bit	= BIT3;
			TRANSDEV.fault.address = (uint32_t)(cl_mmap->odtu_brick->odtu_sts_reg_1);
			TRANSDEV.fault.bit = BIT0;
			TRANSDEV.los.address = (uint32_t)(cl_mmap->odtu_brick->odtu_sts_reg_1);
			TRANSDEV.los.bit = BIT2;
			TRANSDEV.tx_disable.address = (uint32_t)
										  (cl_mmap->odtu_brick->odtu_ctrl_reg_1);
			TRANSDEV.tx_disable.bit	= BIT0;
		}
		else
		{
			if FPGA2
			{
				client_mmap_t* cl_mmap  =
				(client_mmap_t*)CARDINT_port(_trans->index).mmap;
				TRANSDEV.absent.address =(uint32_t)
										 (cl_mmap->odtu_brick->odtu_sts_reg_2);
				TRANSDEV.absent.bit	= BIT3;
				TRANSDEV.fault.address = (uint32_t)(cl_mmap->odtu_brick->odtu_sts_reg_2);
				TRANSDEV.fault.bit = BIT0;
				TRANSDEV.los.address = (uint32_t)(cl_mmap->odtu_brick->odtu_sts_reg_2);
				TRANSDEV.los.bit = BIT2;
				TRANSDEV.tx_disable.address = (uint32_t)
											  (cl_mmap->odtu_brick->odtu_ctrl_reg_2);
				TRANSDEV.tx_disable.bit	= BIT0;
			}
		}
	}
	else
	if((_trans->index >= LN_INDEX_START) && (_trans->index <= LN_INDEX_END))
	{
		/* get line mmap*/
		line_mmap_t* line_mmap = (line_mmap_t*)
								 CARDINT_port(_trans->index).mmap;
		if(_trans->index == LN_INDEX_START)
		{
			TRANSDEV.absent.address     = (uint32_t)(line_mmap->otu2_brick_1.top_level.otu2_brick_li_gen_status_reg);
			TRANSDEV.absent.bit	        = BIT0;
			TRANSDEV.los.address        = (uint32_t)(line_mmap->otu2_brick_1.top_level.otu2_brick_li_gen_status_reg);
			TRANSDEV.los.bit            = BIT5;
			TRANSDEV.fault.address      = (uint32_t)(line_mmap->otu2_brick_1.top_level.otu2_brick_li_gen_status_reg);
			TRANSDEV.fault.bit          = BIT2;
			TRANSDEV.tx_disable.address = (uint32_t)(line_mmap->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg);
			TRANSDEV.tx_disable.bit	    = BIT0;
		}
		else
		{
			if(_trans->index == LN_INDEX_END)
			{
				TRANSDEV.absent.address     = (uint32_t)(line_mmap->otu2_brick_2.top_level.otu2_brick_li_gen_status_reg);
				TRANSDEV.absent.bit     	= BIT19;
				TRANSDEV.los.address        = (uint32_t)(line_mmap->otu2_brick_2.top_level.otu2_brick_li_gen_status_reg);
				TRANSDEV.los.bit            = BIT16;
				TRANSDEV.fault.address      = (uint32_t)(line_mmap->otu2_brick_2.top_level.otu2_brick_li_gen_status_reg);
				TRANSDEV.fault.bit          = BIT18;
				TRANSDEV.tx_disable.address = (uint32_t)(line_mmap->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg);
				TRANSDEV.tx_disable.bit	    = BIT16;
			}
		}
	}
	else
	{
		_trans->info.type = TRANS_UNKNOWN;
		_trans->info.capabilities.tx_tunable = 0;
		_trans->info.capabilities.rx_tunable = 0;
		_trans->info.capabilities.TEMPERATURE_MONITOR = 0;
		_trans->info.capabilities.OPTICAL_POWER_MONITOR = 0;
		_trans->info.capabilities.tx_power_control = 0;
		_trans->info.capabilities.optical_ios_number = 0;
	}
}

/**
 * This function is to update port fec stats
 * @param[in] _port pointer to port object to be mapped
 * Return none
 */
#if ENABLE_FEC_CODE
void update_port_fec_stats(port_t* _port)
{
	ipg_counter64_t fec_bit_errors;
	if(_port->monitor.rate == PORT_RATE_OTU2)
	{

		if(PORT_OPERATION_LINE == _port->info.operation)
		{
			if(_port->index == LN_INDEX_START)
			{
				PM_FECSTATISTICS.corrected_bits.current      = LN_MMAP(0)->otu2_brick_1.framer1.regs.fec_corr_bits_reg->all;
				PM_FECSTATISTICS.corrected_bits.accumulated += PM_FECSTATISTICS.corrected_bits.current;
				PM_FECSTATISTICS.uncorrected_cw.current      = LN_MMAP(0)->otu2_brick_1.framer1.regs.uncorrected_reg->all;
				PM_FECSTATISTICS.uncorrected_cw.accumulated += PM_FECSTATISTICS.uncorrected_cw.current;
				fec_bit_errors.current						 = LN_MMAP(0)->otu2_brick_1.framer1.regs.fec_bit_errors->all;
				PM_FECSTATISTICS.fec_load.value              = (PM_FECSTATISTICS.corrected_bits.current / MAX_EFEC_CORRECTABLE_ONE_SEC)*100;
				PM_FECSTATISTICS.prefec_ber                  = fec_bit_errors.current /OTU2_LINE_RATE_BIT;
				PM_FECSTATISTICS.postfec_ber                 = ((fec_bit_errors.current) - (PM_FECSTATISTICS.corrected_bits.current))/OTU2_LINE_RATE_BIT;
			}
			else
			{
				if(_port->index == LN_INDEX_END)
				{
					PM_FECSTATISTICS.corrected_bits.current      = LN_MMAP(1)->otu2_brick_2.framer1.regs.fec_corr_bits_reg->all;
					PM_FECSTATISTICS.corrected_bits.accumulated += PM_FECSTATISTICS.corrected_bits.current;
					PM_FECSTATISTICS.uncorrected_cw.current 	 = LN_MMAP(1)->otu2_brick_2.framer1.regs.uncorrected_reg->all;
					PM_FECSTATISTICS.uncorrected_cw.accumulated += PM_FECSTATISTICS.uncorrected_cw.current;
					fec_bit_errors.current						 = LN_MMAP(1)->otu2_brick_1.framer1.regs.fec_bit_errors->all;
					PM_FECSTATISTICS.fec_load.value              = (PM_FECSTATISTICS.corrected_bits.current / MAX_EFEC_CORRECTABLE_ONE_SEC)*100;
					PM_FECSTATISTICS.prefec_ber                  = fec_bit_errors.current /OTU2_LINE_RATE_BIT;
					PM_FECSTATISTICS.postfec_ber                 = ((fec_bit_errors.current) - (PM_FECSTATISTICS.corrected_bits.current))/OTU2_LINE_RATE_BIT;
				}
			}
			if(float_compare_gt(PM_FECSTATISTICS.fec_load.value, 100, RES))
			{
				PM_FECSTATISTICS.fec_load.value  = 100;
			}
		}
	}
}
#endif

#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
/**
 * This function updates the STMx related alarms and statistics.
 * @param[in] _port pointer to the port object to be monitored.
 * Return none
 */
void update_port_stmx_stats(port_t* _port)
{
	if(	(_port->info.operation == PORT_OPERATION_CLIENT) &&
		(_port->monitor.prot == PORT_PROTOCOL_SDH))
	{
		stm1416_registers_t* stmx;
		if(_port->monitor.rate == PORT_RATE_STM16)
		{
			if PORT_FPGA1
			{
				stmx = ((client_mmap_t*)(_port->mmap))->stm_16_1;
			}
			else
			{
				if PORT_FPGA2
				{
					stmx = ((client_mmap_t*)(_port->mmap))->stm_16_2;
				}
			}
		}
		else
		{
			if PORT_FPGA1
			{
				stmx = ((client_mmap_t*)(_port->mmap))->stm_14_1;
			}
			else
			{
				if PORT_FPGA2

				{
					stmx = ((client_mmap_t*)(_port->mmap))->stm_14_2;
				}
			}
		}
		CLEAR_STM_ALARMS(PM_STATSTM);
		PM_STATSTM.alarms.bits.LOS =
		CARDINT_TRANS(_port->index).monitor.los.current;
		PM_STATSTM.alarms.bits.LOF = stmx->live_status->bits.LINE_RX_LOF;
		if(stmx->b1_err->bits.LINE_B1_ERRORS > 0)
		{
			PM_STATSTM.alarms.bits.RS_BIP = 1;
		}
		else
		{
			PM_STATSTM.alarms.bits.RS_BIP = 0;
		}
		/*#warning alarms removed in this project*/

/*		PM_STATSTM.alarms.bits.RS_TIM = 0;
		if(stmx->b2_err->bits.LINE_B2_ERRORS > 0)
		{
			PM_STATSTM.alarms.bits.MS_BIP = 1;
		}
		else
		{
			PM_STATSTM.alarms.bits.MS_BIP = 0;
		}
		*/
		PM_STATSTM.alarms.bits.MS_AIS = stmx->live_status->bits.LINE_RX_LAIS;
		PM_STATSTM.alarms.bits.MS_RDI = stmx->live_status->bits.LINE_RX_LRDI;
		/*#warning alarms removed in this project
		 * if(stmx->rei_err->bits.LINE_REI_ERRORS > 0)
		{
			PM_STATSTM.alarms.bits.MS_REI = 1;
		}
		else
		{
			PM_STATSTM.alarms.bits.MS_REI = 0;
		}*/
		PM_STATSTM.bip_errors.current = stmx->b1_err->bits.LINE_B1_ERRORS;
		PM_STATSTM.bip_errors.accumulated += PM_STATSTM.bip_errors.current;
	}
}
#endif
/**
 * This function updates the port otux stats.
 * @param[in] _port pointer to the port object to be monitored.
 * Return none
 */
#if ENABLE_OTUx_CODE
void update_port_otux_stats(port_t* _port)
{
	if(_port->monitor.prot == PORT_PROTOCOL_OTU)
	{
		CLEAR_OTU_ALARMS(PM_STATOTU);
		PM_STATOTU.alarms.bits.LOS =
		CARDINT_TRANS(_port->index).monitor.los.current;
		otu1_mapper_registers_t* otu1;
		if(_port->info.operation == PORT_OPERATION_CLIENT)
		{
			 otu1 = ((client_mmap_t*)_port->mmap)->otu1;
			 if PORT_FPGA1
			 {
				_port->monitor.statistics.otux_sts.alarms.bits.SM_BDI      = otu1->live_status_1->bits.L_SBDI;
				_port->monitor.statistics.otux_sts.alarms.bits.SM_IAE      = otu1->live_status_1->bits.SM_IAE;
				_port->monitor.statistics.otux_sts.alarms.bits.LOF         = otu1->live_status_1->bits.L_LOF;
				_port->monitor.statistics.otux_sts.alarms.bits.LOM         = otu1->live_status_1->bits.L_LOM;
				_port->monitor.statistics.otux_sts.alarms.bits.SM_BIP      = otu1->int_status_1->bits.SM_BIP;
				_port->monitor.statistics.otux_sts.alarms.bits.SM_BEI_BIAE = otu1->live_status_1->bits.SM_BEI_BIAE;
			 }
			 else
			 {
				 if PORT_FPGA2
				 {
					_port->monitor.statistics.otux_sts.alarms.bits.SM_BDI      = otu1->live_status_2->bits.L_SBDI;
					_port->monitor.statistics.otux_sts.alarms.bits.SM_IAE      = otu1->live_status_2->bits.SM_IAE;
					_port->monitor.statistics.otux_sts.alarms.bits.LOF         = otu1->live_status_2->bits.L_LOF;
					_port->monitor.statistics.otux_sts.alarms.bits.LOM         = otu1->live_status_2->bits.L_LOM;
					_port->monitor.statistics.otux_sts.alarms.bits.SM_BIP      = otu1->int_status_2->bits.SM_BIP;
					_port->monitor.statistics.otux_sts.alarms.bits.SM_BEI_BIAE = otu1->live_status_2->bits.SM_BEI_BIAE;
				 }
			 }
		}
		else
		if(_port->info.operation == PORT_OPERATION_LINE)
		{
			if(_port->index == LN_INDEX_START)
			{
				PM_STATOTU.alarms.bits.SM_BDI      = LN_MMAP(0)->otu2_brick_1.framer1.regs.otu2_sts_reg->bits.S_BDI;
				PM_STATOTU.alarms.bits.LOF         = LN_MMAP(0)->otu2_brick_1.framer1.regs.live_sts_reg->bits.LOF;
				PM_STATOTU.alarms.bits.LOM         = LN_MMAP(0)->otu2_brick_1.framer1.regs.live_sts_reg->bits.LOM;
				PM_STATOTU.alarms.bits.SM_BIP      = LN_MMAP(0)->otu2_brick_1.framer1.regs.otu2_sts_reg->bits.S_BIP8;
				//PM_STATOTU.alarms.bits.SM_BEI_BIAE = LN_MMAP(0)->otu2_brick_1.framer1.regs.otu2_sts_reg->bits.S_BEI;
				(LN_MMAP(0)->otu2_brick_1.framer1.regs.otu2_sts_reg->bits.S_BEI > 0)?(PM_STATOTU.alarms.bits.SM_BEI_BIAE = 1):(PM_STATOTU.alarms.bits.SM_BEI_BIAE = 0);

				PM_STATOTU.bip_errors.current      = LN_MMAP(0)->otu2_brick_1.framer1.regs.sm_bip8_errors_reg->all;
				PM_STATOTU.bip_errors.accumulated  += PM_STATOTU.bip_errors.current;
				/*ODUx*/
				odux_alarms[_port->index].all = 0;
				odux_alarms[_port->index].bits.AIS    = LN_MMAP(0)->otu2_brick_1.framer1.regs.live_sts_reg->bits.AIS;
				odux_alarms[_port->index].bits.OCI    = LN_MMAP(0)->otu2_brick_1.framer1.regs.rx_exp_sts_pt_reg->bits.LI_RX_OCI;
				odux_alarms[_port->index].bits.LCK    = LN_MMAP(0)->otu2_brick_1.framer1.regs.rx_exp_sts_pt_reg->bits.LI_RX_LCK;
				odux_alarms[_port->index].bits.PM_BIP = LN_MMAP(0)->otu2_brick_1.framer1.regs.live_sts_reg->bits.PM_BIP8;
				odux_alarms[_port->index].bits.PM_BEI = LN_MMAP(0)->otu2_brick_1.framer1.regs.live_sts_reg->bits.PM_BEI_CNT;
				odux_alarms[_port->index].bits.PM_BDI = LN_MMAP(0)->otu2_brick_1.framer1.regs.live_sts_reg->bits.PM_BDI;

				/**/
			}
			else
			{
				if(_port->index == LN_INDEX_END)
				{
					PM_STATOTU.alarms.bits.SM_BDI      = LN_MMAP(1)->otu2_brick_2.framer1.regs.otu2_sts_reg->bits.S_BDI;
					PM_STATOTU.alarms.bits.LOF         = LN_MMAP(1)->otu2_brick_2.framer1.regs.live_sts_reg->bits.LOF;
					PM_STATOTU.alarms.bits.LOM         = LN_MMAP(1)->otu2_brick_2.framer1.regs.live_sts_reg->bits.LOM;
					PM_STATOTU.alarms.bits.SM_BIP      = LN_MMAP(1)->otu2_brick_2.framer1.regs.otu2_sts_reg->bits.S_BIP8;
					//PM_STATOTU.alarms.bits.SM_BEI_BIAE = LN_MMAP(1)->otu2_brick_2.framer1.regs.otu2_sts_reg->bits.S_BEI;
					(LN_MMAP(1)->otu2_brick_2.framer1.regs.otu2_sts_reg->bits.S_BEI > 0)?(PM_STATOTU.alarms.bits.SM_BEI_BIAE = 1):(PM_STATOTU.alarms.bits.SM_BEI_BIAE = 0);
					PM_STATOTU.bip_errors.current      = LN_MMAP(1)->otu2_brick_2.framer1.regs.sm_bip8_errors_reg->all;
					PM_STATOTU.bip_errors.accumulated  += PM_STATOTU.bip_errors.current;

					/*ODUx*/
					odux_alarms[_port->index].all = 0;
					odux_alarms[_port->index].bits.AIS    = LN_MMAP(1)->otu2_brick_2.framer1.regs.live_sts_reg->bits.AIS;
					odux_alarms[_port->index].bits.OCI    = LN_MMAP(1)->otu2_brick_2.framer1.regs.rx_exp_sts_pt_reg->bits.LI_RX_OCI;
					odux_alarms[_port->index].bits.LCK    = LN_MMAP(1)->otu2_brick_2.framer1.regs.rx_exp_sts_pt_reg->bits.LI_RX_LCK;
					odux_alarms[_port->index].bits.PM_BIP = LN_MMAP(1)->otu2_brick_2.framer1.regs.live_sts_reg->bits.PM_BIP8;
					odux_alarms[_port->index].bits.PM_BEI = LN_MMAP(1)->otu2_brick_2.framer1.regs.live_sts_reg->bits.PM_BEI_CNT;
					odux_alarms[_port->index].bits.PM_BDI = LN_MMAP(1)->otu2_brick_2.framer1.regs.live_sts_reg->bits.PM_BDI;
				}
			}
		}
	}
}
#endif

#if ENABLE_ETH_CODE
/**
 * This function reads the statistics and alarms of an ETHERNET port.
 * @param[in] _port pointer to the ETHERNET port
 * Return none
 */
void update_port_ethernet_stats(port_t* _port)
{
	if PORT_FPGA1
	{
		if((_port->info.operation == PORT_OPERATION_CLIENT) &&
		   (_port->monitor.prot == PORT_PROTOCOL_ETHERNET))
		{
			CLEAR_ETH_ALARMS(PMSTAT_ETHSTS);
			PMSTAT_ETHSTS.alarms.bits.LOS =
			CARDINT_TRANS(_port->index).monitor.los.current;
			ge_brick_t* ge = ((client_mmap_t*)(_port->mmap))->ge;
			ge_mac_registers_t* ge_mac_1 = ((client_mmap_t*)(_port->mmap))->gemac_1;
			if(ge->general_sts_reg_1->bits.GE_LINK == 1)
			{
				PMSTAT_ETHSTS.alarms.bits.LINK_DOWN = 0;
			}
			else
			{
				PMSTAT_ETHSTS.alarms.bits.LINK_DOWN = 1;
			}
			if(ge_mac_1->rx_fcs_err_reg->all > 0)
			{
				PMSTAT_ETHSTS.alarms.bits.FCS = 1;
			}
			else
			{
				PMSTAT_ETHSTS.alarms.bits.FCS = 0;
			}
			 /*counters */
			PMSTAT_ETHSTS.tx_ok.current       = ge_mac_1->tx_frm_ok_reg->all;
			PMSTAT_ETHSTS.rx_ok.current       = ge_mac_1->rx_frm_ok_reg->all;
			PMSTAT_ETHSTS.rx_crc.current      = ge_mac_1->rx_fcs_err_reg->all;
			PMSTAT_ETHSTS.rx_ok.accumulated   = PMSTAT_ETHSTS.rx_ok.current;
			PMSTAT_ETHSTS.tx_ok.accumulated   = PMSTAT_ETHSTS.tx_ok.current;
			PMSTAT_ETHSTS.rx_crc.accumulated  = PMSTAT_ETHSTS.rx_crc.current;
			PMSTAT_ETHSTS.tx_octs.current     = ge_mac_1->tx_oct_ok_reg->all;
			PMSTAT_ETHSTS.rx_octs.current     = ge_mac_1->rx_oct_ok_reg->all;
			PMSTAT_ETHSTS.tx_octs.accumulated = PMSTAT_ETHSTS.tx_octs.current;
			PMSTAT_ETHSTS.rx_octs.accumulated = PMSTAT_ETHSTS.rx_octs.current;
		}
	}
	else
	{
		if PORT_FPGA2
		{
			if((PORT_OPERATION_CLIENT == _port->info.operation) &&
					   (PORT_PROTOCOL_ETHERNET == _port->monitor.prot))
			{
				CLEAR_ETH_ALARMS(PMSTAT_ETHSTS);
				PMSTAT_ETHSTS.alarms.bits.LOS =
				CARDINT_TRANS(_port->index).monitor.los.current;
				ge_brick_t* ge = ((client_mmap_t*)(_port->mmap))->ge;
				ge_mac_registers_t* ge_mac_2 = ((client_mmap_t*)(_port->mmap))->gemac_2;
				if(ge->general_sts_reg_2->bits.GE_LINK == 1)
				{
					PMSTAT_ETHSTS.alarms.bits.LINK_DOWN = 0;
				}
				else
				{
					PMSTAT_ETHSTS.alarms.bits.LINK_DOWN = 1;
				}
				if(ge_mac_2->rx_fcs_err_reg->all > 0)
				{
					PMSTAT_ETHSTS.alarms.bits.FCS = 1;
				}
				else
				{
					PMSTAT_ETHSTS.alarms.bits.FCS = 0;
				}
				 /*counters */
				PMSTAT_ETHSTS.tx_ok.current       = ge_mac_2->tx_frm_ok_reg->all;
				PMSTAT_ETHSTS.rx_ok.current       = ge_mac_2->rx_frm_ok_reg->all;
				PMSTAT_ETHSTS.rx_crc.current      = ge_mac_2->rx_fcs_err_reg->all;
				PMSTAT_ETHSTS.rx_ok.accumulated   = PMSTAT_ETHSTS.rx_ok.current;
				PMSTAT_ETHSTS.tx_ok.accumulated   = PMSTAT_ETHSTS.tx_ok.current;
				PMSTAT_ETHSTS.rx_crc.accumulated  = PMSTAT_ETHSTS.rx_crc.current;
				PMSTAT_ETHSTS.tx_octs.current     = ge_mac_2->tx_oct_ok_reg->all;
				PMSTAT_ETHSTS.rx_octs.current     = ge_mac_2->rx_oct_ok_reg->all;
				PMSTAT_ETHSTS.tx_octs.accumulated = PMSTAT_ETHSTS.tx_octs.current;
				PMSTAT_ETHSTS.rx_octs.accumulated = PMSTAT_ETHSTS.rx_octs.current;
			}
		}
	}
}
#endif

/**
* Function to give the transceiver state
* @param[in] _trans pointer to _trans object to be mapped
*
* @return a string of character that reflect the transceiver state
*/
char* STATE_TEXT(transceiver_t* _trans)
{
	char* ret = "nothing";
	switch(_trans->monitor.state.current)
	{
		case TRANSCEIVER_STATE_ABSENT:
			ret = "unplugged";
			break;
		case TRANSCEIVER_STATE_OK:
			ret = "OK";
			break;
		case TRANSCEIVER_STATE_REBOOTING:
			ret = "plugged and rebooting";
			break;
		case TRANSCEIVER_STATE_FAIL:
			ret = "plugged but failing";
			break;
		case TRANSCEIVER_STATE_NOT_READY:
			ret = "plugged but not ready";
			break;
	}
	ret = "nothing";
	return (ret);
}
/**
 * Callback function for transceiver los change
 * @param[in] _trans pointer to _trans object
 * Return none
 */
void callback_on_transceiver_los_change(transceiver_t* _trans)
{

}

#if ENABLE_FEC_CODE
/**
* Function to port fect type
* @param[in] _port pointer to _port object
*
* @return RET_FAIL in case of failure
*/
ret_code_t get_if_port_fectype(port_t* _port)
{
	ret_code_t err = RET_FAIL;
	if((!_port->info.capabilities.FEC_CAPABLE)
		&&(_port->monitor.rate != PORT_RATE_OTU1))
	{
		err = RET_FAIL;
	}
	if(err != RET_FAIL)
	{
		if(PORT_OPERATION_CLIENT == _port->info.operation)
		{
			_port->monitor.fec_statistics.type = FEC_TYPE_GFEC;
		}
		else
		if(PORT_OPERATION_LINE == _port->info.operation)
		{
			_port->monitor.fec_statistics.type = FEC_TYPE_EFEC;
		}
		else
		{
			err = RET_INVALID_PARAMS;
		}
	}
	return (err);
}
#endif

#ifdef KURS_PROTOCOL
#define SEVERITY_NORMAL		0
#define SEVERITY_WARNING 	1
#define SEVERITY_CRITICAL	2
#define SEVERITY_INFO		3
/**
 * Function to update diagnostic alarms
 * @param[in] diagnostic_ptr pointer to diagnostics object
 * Return none
 */
void update_diagnostic_alarms(diagnostics_t* diagnostic_ptr)
{
	diagnostic_ptr->alarm.previous = diagnostic_ptr->alarm.current;
	/*Do not use ">" or "<" to compare float values */
	if((float_compare_lt(diagnostic_ptr->value,diagnostic_ptr->critical_min, 0.00001))||
	   (float_compare_gt(diagnostic_ptr->value,diagnostic_ptr->critical_max, 0.00001)))
	{
		diagnostic_ptr->alarm.current = SEVERITY_CRITICAL;
	}
	else
	if((float_compare_lt(diagnostic_ptr->value,diagnostic_ptr->warning_min, 0.00001))||
	   (float_compare_gt(diagnostic_ptr->value,diagnostic_ptr->warning_max, 0.00001)))
	{
		diagnostic_ptr->alarm.current = SEVERITY_WARNING;
	}
	else
	{
		diagnostic_ptr->alarm.current = SEVERITY_NORMAL;
	}
}

/**
 * Updates the current alarm state of a FEC load structure.
 * If current value of the diagnostics is greater than warning thresholds but
 * less than critical, state will be warning.
 * If current value is greater than critical thresholds, state will be changed
 * to critical.
 * Else, state is normal.
 */
/*void update_fec_load_alarms(diagnostics_t* fec_load){ */
/*	fec_load->alarm.previous = fec_load->alarm.current; */
/*	if(float_compare_gt(fec_load->value,fec_load->critical_max, 0.00001)){ */
/*		fec_load->alarm.current = SEVERITY_CRITICAL; */
/*	}else if(float_compare_gt(fec_load->value,fec_load->warning_max, 0.00001)){
 * */
/*		fec_load->alarm.current = SEVERITY_WARNING; */
/*	}else{ */
/*		fec_load->alarm.current = SEVERITY_NORMAL; */
/*	} */
/*	if(fec_load->value > fec_load->critical_max){ */
/*		fec_load->alarm.current = SEVERITY_CRITICAL; */
/*	}else if(fec_load->value > fec_load->warning_max){ */
/*		fec_load->alarm.current = SEVERITY_WARNING; */
/*	}else{ */
/*		fec_load->alarm.current = SEVERITY_NORMAL; */
/*	} */
/*} */

/**
 * Function to update diagnostics alarms
 * Return none
 */
void update_alarms(void)
{
	int if_index = 0;
	for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
	{
		update_diagnostic_alarms(&CARDINT_TRANS(if_index).monitor.temperature);
		update_diagnostic_alarms(&pin[if_index]);
		update_diagnostic_alarms(&pout[if_index]);
		/*update FEC load alarm */
		if(CARDINT_port(if_index).info.capabilities.FEC_CAPABLE)
		{
			update_diagnostic_alarms(&C_FECLOAD(if_index));
		}
		update_diagnostic_alarms (&card.temperature);
	}
}
/**
 * Function to update alarms 1
 * Return none
 */
/*TODO: Include block state in the Alarm1/2 algorithms */
void update_card_alarm1(void)
{
	ipg_status_t alarm;
	card.alarm1.previous = card.alarm1.current;
	alarm.current = 0;
	if(SEVERITY_WARNING == card.temperature.alarm.current)
	{
		alarm.current = 1;
	}
	else
	{
		int if_index = 0;
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			if(ACTIVITY_MONITOR_ON == card.interfaces[if_index].monitor)
			{
				if(C_TEMPMONITOR(if_index))
				{
					if(SEVERITY_WARNING == C_TEMPALARM(if_index).current)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
				if(C_OPM(if_index))
				{
					if(SEVERITY_WARNING == pin[if_index].alarm.current)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
					if(SEVERITY_WARNING == pout[if_index].alarm.current)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}

				if(CARDINT_port(if_index).info.capabilities.FEC_CAPABLE)
				{
					if(SEVERITY_WARNING == C_FECLOAD(if_index).alarm.current)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
			}
		}
	}
	card.alarm1.current = alarm.current;
	card.alarm1.change = card.alarm1.current ^ card.alarm1.previous;
}
/**
 * Function to update alarms 2
 * Return none
 */
void update_card_alarm2(void)
{
	ipg_status_t alarm;
	card.alarm2.previous = card.alarm2.current;
	alarm.current = 0;
	if(SEVERITY_CRITICAL == card.temperature.alarm.current)
	{
		alarm.current = 1;
	}
	else
	{
		int if_index=0;
		for(if_index = 0; if_index < INTERFACE_COUNT; if_index++)
		{
			if(ACTIVITY_MONITOR_ON == card.interfaces[if_index].monitor)
			{
				if(C_TEMPMONITOR(if_index))
				{
					if(SEVERITY_CRITICAL == C_TEMPALARM(if_index).current)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
				if(C_OPM(if_index))
				{
					if(SEVERITY_CRITICAL == pin[if_index].alarm.current)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
					if(SEVERITY_CRITICAL == pout[if_index].alarm.current)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}

				if(CARDINT_port(if_index).info.capabilities.FEC_CAPABLE)
				{
					if(C_FECLOAD(if_index).alarm.current == SEVERITY_CRITICAL)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
				/*Only client parameters */
				if(CARDINT_port(if_index).info.operation == PORT_OPERATION_LINE )
				{
					if(odux_alarms[if_index].all != 0)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
#if ENABLE_ETH_CODE
				/*Ethernet alarms */
				if(C_MONPROT(if_index) == PORT_PROTOCOL_ETHERNET)
				{
					if(C_ETHSTSALARM(if_index).all != 0)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
#endif
#if ENABLE_STMx_CODE || ENABLE_SDHx_CODE
				/*SONET alarms */
				if((C_MONPROT(if_index) == PORT_PROTOCOL_SONET) ||
					(C_MONPROT(if_index) == PORT_PROTOCOL_SDH ))
				{
					if(C_ETHSTMXALARM(if_index).all != 0)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
#endif
#if ENABLE_OTUx_CODE
				/*OTU alarms */
				if(C_MONPROT(if_index) == PORT_PROTOCOL_OTU)
				{
					if(C_ETHOTUXALARM(if_index).all != 0)
					{
						alarm.current = 1;
						break; /*no need to go through all the rest*/
					}
				}
#endif

#if ENABLE_FCx_CODE
				/*Fiber channel alarms */
				if(C_MONPROT(if_index) == PORT_PROTOCOL_FIBER_CHANNEL)
				{
					/*TODO: Update the following section when we have a fiber
					 *  channel type */
				}
#endif
			}
		}
	}
	card.alarm2.current = alarm.current;
	card.alarm2.change = card.alarm2.current ^ card.alarm2.previous;
}
#endif

/**Function that switches a port from one rate to another.
 * @param[in] _port pointer to the port object
 * @return RET_SUCCESS on success
 */
ret_code_t set_if_port_rate(port_t* _port)
{
	/* make sure this is a valid client port */
	if((_port == NULL)||(!_port->info.capabilities.MULTI_RATE))
	{
		return (RET_INVALID_PARAMS);
	}
	if(_port->mmap == NULL)
	{
		return (RET_FAIL);
	}
	ret_code_t ret       = RET_SUCCESS;
	uint32_t _rate       = _port->control.rate.next;
	uint32_t odtug2_rate = _port->control.rate.next;

	uint32_t top_level_rate = 1;
	switch(_rate)
	{
		case PORT_RATE_FC1:
			top_level_rate = 4;
			break;
		case PORT_RATE_FC2:
			top_level_rate = 5;
			break;
		case PORT_RATE_OTU1:
			top_level_rate = 3;
			break;
		case PORT_RATE_STM1:
		case PORT_RATE_STM4:
			top_level_rate = 1;
			break;
		case PORT_RATE_STM16:
			top_level_rate = 2;
			break;
		case PORT_RATE_FE:
		case PORT_RATE_GE:
			top_level_rate = 0;
			break;
		default:
			break;
	}
	if(PORT_OPERATION_CLIENT == _port->info.operation)
	{
		switch(_port->index)
		{
			case 0:
				port_rate_change_flag[0] = 1;
				TLM_REG_1_BITS.PORT_0_RATE_SELECT = top_level_rate;
				TLM_REG_1_BITS.PORT_0_RECONFIG_WR = 0;
				TLM_REG_1_BITS.PORT_0_RECONFIG_WR = 1;
				TLM_REG_1_BITS.PORT_0_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
//						LN_MMAP(0)->otu1_mapper->ln_cfg_reg->bits.FRAMER_MODE = 0;
//						LN_MMAP(1)->otu1_mapper->ln_cfg_reg->bits.FRAMER_MODE = 0;
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						/*#warning: check if i have this condition if yes added to all the clients*/
						/* when client 0 has to switch to FC2, client 1 has to
						 *  switch to STM1
						 * (inherited from older design, requested by Ali)
						 * */
//						CARDINT_port(0).control.rate.next = PORT_RATE_STM1;
//						CARDINT_port(0).control.rate.change = 1;
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_1->bits.PORT_0_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_1->bits.UPDATE_MSI = 0;
				odtug2.config_reg_1->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_0_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_0_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))//GE
				{
					reconfigure_ge_port(_port);
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(0)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
				CL_MMAP(0)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
				CL_MMAP(0)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;

				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(0)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(0)->misc->general_control->bits.FC_RESET = 0;
				}
			break;
		case 1:
			port_rate_change_flag[1] = 1;
			TLM_REG_1_BITS.PORT_1_RATE_SELECT = top_level_rate;
			TLM_REG_1_BITS.PORT_1_RECONFIG_WR = 0;
			TLM_REG_1_BITS.PORT_1_RECONFIG_WR = 1;
			TLM_REG_1_BITS.PORT_1_RECONFIG_WR = 0;
			/*Configure the SiLabs accordingly */
			switch(_rate)
			{
				case PORT_RATE_FE:
				case PORT_RATE_GE:
					switch_to_ge(_port);
					break;
				case PORT_RATE_STM1:
				case PORT_RATE_STM4:
				case PORT_RATE_STM16:
					switch_to_sonet(_port);
					break;
				case PORT_RATE_FC1:
					switch_to_fc1(_port);
					break;
				case PORT_RATE_FC2:
					switch_to_fc2(_port);
					break;
				default:
					break;
			}
			//Apply the changes on the ODTUG2
			odtug2.regA_portM_rate_reg_1->bits.PORT_1_RATE_RX = odtug2_rate;

			//We need this so that the updated changes take effect in the system
			odtug2.config_reg_1->bits.UPDATE_MSI = 0;
			odtug2.config_reg_1->bits.UPDATE_MSI = 1;
			/*Configure STM1 and STM4 */
			if(PORT_RATE_STM1 == _rate)
			{
				/*Clear SDH port rate bit */
				top_level_misc_reg.sdh_rate_config_1->bits.PORT_1_SDH_RATE = 0;
			}
			if(PORT_RATE_STM4 == _rate)
			{
				/*Set SDH port rate bit */
				top_level_misc_reg.sdh_rate_config_1->bits.PORT_1_SDH_RATE = 1;
			}
			if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
			{
//				CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//				Delay_ms(10);
//				CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
			}
			if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE)){
				reconfigure_ge_port(_port);
			}
			//Update client type
			_port->control.rate.current = _rate;
			CL_MMAP(1)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
			CL_MMAP(1)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
			CL_MMAP(1)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB= 0;
			break;
		case 2:
			port_rate_change_flag[2] = 1;
			TLM_REG_1_BITS.PORT_2_RATE_SELECT = top_level_rate;
			TLM_REG_1_BITS.PORT_2_RECONFIG_WR = 0;
			TLM_REG_1_BITS.PORT_2_RECONFIG_WR = 1;
			TLM_REG_1_BITS.PORT_2_RECONFIG_WR = 0;
			/*Configure the SiLabs accordingly */
			switch(_rate)
			{
				case PORT_RATE_FE:
				case PORT_RATE_GE:
					switch_to_ge(_port);
					break;
				case PORT_RATE_STM1:
				case PORT_RATE_STM4:
				case PORT_RATE_STM16:
					switch_to_sonet(_port);
					break;
				case PORT_RATE_OTU1:
					switch_to_otu1(_port);
					break;
				case PORT_RATE_FC1:
					switch_to_fc1(_port);
					break;
				case PORT_RATE_FC2:
					switch_to_fc2(_port);
					break;
				default:
					break;
			}
			//Apply the changes on the ODTUG2
			odtug2.regA_portM_rate_reg_1->bits.PORT_2_RATE_RX = odtug2_rate;

			//We need this so that the updated changes take effect in the system
			odtug2.config_reg_1->bits.UPDATE_MSI = 0;
			odtug2.config_reg_1->bits.UPDATE_MSI = 1;
			/*Configure STM1 and STM4 */
			if(PORT_RATE_STM1 == _rate)
			{
				/*Clear SDH port rate bit */
				top_level_misc_reg.sdh_rate_config_1->bits.PORT_2_SDH_RATE = 0;
			}
			if(PORT_RATE_STM4 == _rate)
			{
				/*Set SDH port rate bit */
				top_level_misc_reg.sdh_rate_config_1->bits.PORT_2_SDH_RATE = 1;
			}
			if((_rate == PORT_RATE_GE) ||(_rate == PORT_RATE_FE))
			{
				reconfigure_ge_port(_port);
			}
			//Update client type
			_port->control.rate.current = _rate;
			CL_MMAP(2)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
			CL_MMAP(2)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
			CL_MMAP(2)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB= 0;
			if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
			{
//				CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//				Delay_ms(10);
//				CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
			}
			break;
			case 3:
				port_rate_change_flag[3] = 1;
				TLM_REG_1_BITS.PORT_3_RATE_SELECT = top_level_rate;
				TLM_REG_1_BITS.PORT_3_RECONFIG_WR = 0;
				TLM_REG_1_BITS.PORT_3_RECONFIG_WR = 1;
				TLM_REG_1_BITS.PORT_3_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_1->bits.PORT_3_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_1->bits.UPDATE_MSI = 0;
				odtug2.config_reg_1->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_3_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_3_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) ||(_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(3)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
				CL_MMAP(3)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
				CL_MMAP(3)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB= 0;
				break;
			break;
			case 4:
				port_rate_change_flag[4] = 1;
				TLM_REG_1_BITS.PORT_4_RATE_SELECT = top_level_rate;
				TLM_REG_1_BITS.PORT_4_RECONFIG_WR = 0;
				TLM_REG_1_BITS.PORT_4_RECONFIG_WR = 1;
				TLM_REG_1_BITS.PORT_4_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_1->bits.PORT_4_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_1->bits.UPDATE_MSI = 0;
				odtug2.config_reg_1->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_4_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_4_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) ||(_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(4)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
				CL_MMAP(4)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
				CL_MMAP(4)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB= 0;

				break;
			case 5:
				port_rate_change_flag[5] = 1;
				TLM_REG_1_BITS.PORT_5_RATE_SELECT = top_level_rate;
				TLM_REG_1_BITS.PORT_5_RECONFIG_WR = 0;
				TLM_REG_1_BITS.PORT_5_RECONFIG_WR = 1;
				TLM_REG_1_BITS.PORT_5_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_1->bits.PORT_5_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_1->bits.UPDATE_MSI = 0;
				odtug2.config_reg_1->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_5_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_5_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) ||(_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(5)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
				CL_MMAP(5)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
				CL_MMAP(5)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;

				break;
			case 6:
				port_rate_change_flag[6] = 1;
				TLM_REG_1_BITS.PORT_6_RATE_SELECT = top_level_rate;
				TLM_REG_1_BITS.PORT_6_RECONFIG_WR = 0;
				TLM_REG_1_BITS.PORT_6_RECONFIG_WR = 1;
				TLM_REG_1_BITS.PORT_6_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_1->bits.PORT_6_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_1->bits.UPDATE_MSI = 0;
				odtug2.config_reg_1->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_6_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_6_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))//GE
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(6)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
				CL_MMAP(6)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
				CL_MMAP(6)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
				break;
			case 7:
				port_rate_change_flag[7] = 1;
				TLM_REG_1_BITS.PORT_7_RATE_SELECT = top_level_rate;
				TLM_REG_1_BITS.PORT_7_RECONFIG_WR = 0;
				TLM_REG_1_BITS.PORT_7_RECONFIG_WR = 1;
				TLM_REG_1_BITS.PORT_7_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_1->bits.PORT_7_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_1->bits.UPDATE_MSI = 0;
				odtug2.config_reg_1->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_7_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_1->bits.PORT_7_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(7)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
				CL_MMAP(7)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
				CL_MMAP(7)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;

				break;
			case 8:
				port_rate_change_flag[8] = 1;
				TLM_REG_2_BITS.PORT_0_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_0_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_0_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_0_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_0_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_0_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_0_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(8)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(8)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(8)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				break;
			case 9:
				port_rate_change_flag[9] = 1;
				TLM_REG_2_BITS.PORT_1_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_1_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_1_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_1_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_1_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_1_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_1_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(9)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(9)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(9)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				break;
			case 10:
				port_rate_change_flag[10] = 1;
				TLM_REG_2_BITS.PORT_2_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_2_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_2_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_2_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_2_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_2_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_2_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(10)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(10)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(10)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				break;
			case 11:
				port_rate_change_flag[11] = 1;
				TLM_REG_2_BITS.PORT_3_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_3_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_3_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_3_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_3_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_3_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_3_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(11)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(11)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(11)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;

				break;
			case 12:
				port_rate_change_flag[12] = 1;
				TLM_REG_2_BITS.PORT_4_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_4_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_4_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_4_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_4_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_4_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_4_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(12)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(12)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(12)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				break;
			case 13:
				port_rate_change_flag[13] = 1;
				TLM_REG_2_BITS.PORT_5_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_5_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_5_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_5_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_5_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_5_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_5_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(13)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(13)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(13)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				break;
			case 14:
				port_rate_change_flag[14] = 1;
				TLM_REG_2_BITS.PORT_6_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_6_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_6_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_6_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_6_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_6_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_6_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(14)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(14)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(14)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				break;
			case 15:
				port_rate_change_flag[15] = 1;
				TLM_REG_2_BITS.PORT_7_RATE_SELECT = top_level_rate;
				TLM_REG_2_BITS.PORT_7_RECONFIG_WR = 0;
				TLM_REG_2_BITS.PORT_7_RECONFIG_WR = 1;
				TLM_REG_2_BITS.PORT_7_RECONFIG_WR = 0;
				/*Configure the SiLabs accordingly */
				switch(_rate)
				{
					case PORT_RATE_FE:
					case PORT_RATE_GE:
						switch_to_ge(_port);
						break;
					case PORT_RATE_STM1:
					case PORT_RATE_STM4:
					case PORT_RATE_STM16:
						switch_to_sonet(_port);
						break;
					case PORT_RATE_OTU1:
						switch_to_otu1(_port);
						break;
					case PORT_RATE_FC1:
						switch_to_fc1(_port);
						break;
					case PORT_RATE_FC2:
						switch_to_fc2(_port);
						break;
					default:
						break;
				}
				//Apply the changes on the ODTUG2
				odtug2.regA_portM_rate_reg_2->bits.PORT_7_RATE_RX = odtug2_rate;

				//We need this so that the updated changes take effect in the system
				odtug2.config_reg_2->bits.UPDATE_MSI = 0;
				odtug2.config_reg_2->bits.UPDATE_MSI = 1;
				/*Configure STM1 and STM4 */
				if(PORT_RATE_STM1 == _rate)
				{
					/*Clear SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_7_SDH_RATE = 0;
				}
				if(PORT_RATE_STM4 == _rate)
				{
					/*Set SDH port rate bit */
					top_level_misc_reg.sdh_rate_config_2->bits.PORT_7_SDH_RATE = 1;
				}
				if((_rate == PORT_RATE_GE) || (_rate == PORT_RATE_FE))
				{
					reconfigure_ge_port(_port);
				}
				if((PORT_RATE_FC1 == _rate) || (PORT_RATE_FC2 == _rate))
				{
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 1;
//					Delay_ms(10);
//					CL_MMAP(_port)->misc->general_control->bits.FC_RESET = 0;
				}
				//Update client type
				_port->control.rate.current = _rate;
				CL_MMAP(15)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				CL_MMAP(15)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
				CL_MMAP(15)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
				break;

		}
	}
	return (ret);
}

/**Function that read a port from one rate to another.
 * @param[in] _port pointer to the port object
 * @return RET_SUCCESS on success
 */
ret_code_t read_if_port_rate(port_t* _port)
{
	ret_code_t rc = RET_SUCCESS;
	/* make sure this is a valid client port */
	if(	(_port == NULL) || (_port->mmap == NULL) ||
		(!_port->info.capabilities.MULTI_RATE) )
	{
		rc = RET_INVALID_PARAMS;
	}
	if(rc == RET_SUCCESS)
	{
		alt_u32 current_port_type = 0;
		switch (_port->index)
		{
			case 0:	/*Client 0 */
				current_port_type = TLM_REG_1_BITS.PORT_0_RATE_SELECT;
				break;
			case 1:	/*Client 1 */
				current_port_type = TLM_REG_1_BITS.PORT_1_RATE_SELECT;
				break;
			case 2:	/*Client 2 */
				current_port_type = TLM_REG_1_BITS.PORT_2_RATE_SELECT;
				break;
			case 3:	/*Client 3 */
				current_port_type = TLM_REG_1_BITS.PORT_3_RATE_SELECT;
				break;
			case 4:	/*Client 4 */
				current_port_type = TLM_REG_1_BITS.PORT_4_RATE_SELECT;
				break;
			case 5:	/*Client 5 */
				current_port_type = TLM_REG_1_BITS.PORT_5_RATE_SELECT;
				break;
			case 6:	/*Client 6 */
				current_port_type = TLM_REG_1_BITS.PORT_6_RATE_SELECT;
				break;
			case 7:	/*Client 7 */
				current_port_type = TLM_REG_1_BITS.PORT_7_RATE_SELECT;
				break;
			case 8:	/*Client 8 */
				current_port_type = TLM_REG_2_BITS.PORT_0_RATE_SELECT;
				break;
			case 9:	/*Client 9 */
				current_port_type = TLM_REG_2_BITS.PORT_1_RATE_SELECT;
				break;
			case 10:/*Client 10 */
				current_port_type = TLM_REG_2_BITS.PORT_2_RATE_SELECT;
				break;
			case 11:/*Client 11 */
				current_port_type = TLM_REG_2_BITS.PORT_3_RATE_SELECT;
				break;
			case 12:/*Client 12 */
				current_port_type = TLM_REG_2_BITS.PORT_4_RATE_SELECT;
				break;
			case 13:/*Client 13 */
				current_port_type = TLM_REG_2_BITS.PORT_5_RATE_SELECT;
				break;
			case 14:/*Client 14 */
				current_port_type = TLM_REG_2_BITS.PORT_6_RATE_SELECT;
				break;
			case 15:/*Client 15 */
				current_port_type = TLM_REG_2_BITS.PORT_7_RATE_SELECT;
				break;
			default:
				break;
		}
		switch(current_port_type)
		{
			case 0:
				if(_port->control.rate.next == PORT_RATE_FE)
				{
					_port->monitor.rate = PORT_RATE_FE;
					_port->monitor.prot = PORT_PROTOCOL_ETHERNET;
				}
				else
				if(_port->control.rate.next == PORT_RATE_GE)
				{
					_port->monitor.rate = PORT_RATE_GE;
					_port->monitor.prot = PORT_PROTOCOL_ETHERNET;
				}
				else
				{
					rc = RET_INVALID_PARAMS;
				}
				break;
			case 1:
				_port->monitor.rate = PORT_RATE_STM1;
				_port->monitor.prot = PORT_PROTOCOL_SDH;
				break;
			case 2:
				_port->monitor.rate = PORT_RATE_STM16;
				_port->monitor.prot = PORT_PROTOCOL_SDH;
				break;
			case 3:
				_port->monitor.rate = PORT_RATE_OTU1;
				_port->monitor.prot = PORT_PROTOCOL_OTU;
				break;
			case 4:
				_port->monitor.rate = PORT_RATE_FC1;
				_port->monitor.prot = PORT_PROTOCOL_FIBER_CHANNEL;
				break;
			case 5:
				_port->monitor.rate = PORT_RATE_FC2;
				_port->monitor.prot = PORT_PROTOCOL_FIBER_CHANNEL;
				break;
			case 8:
				_port->monitor.rate = PORT_RATE_STM4;
				_port->monitor.prot = PORT_PROTOCOL_SDH;
				break;
			default:
				rc = RET_FAIL;
				break;
		}
	}
	return (rc);
}
/*#warning check if i need to remove this function*/
///**
// * Function to write a selective port rate
// * @param[in] index
// * @param[in] v
// * Return none
// */
//void WR_port_rate_select(uint32_t index, uint32_t v)
//{
//	switch (index)
//	{
//		case 0:
//			TLM_REG_BITS.PORT_0_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_0_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_0_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_0_RECONFIG_WR = 0;
//			break;
//		case 1:
//			TLM_REG_BITS.PORT_1_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_1_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_1_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_1_RECONFIG_WR = 0;
//			break;
//		case 2:
//			TLM_REG_BITS.PORT_2_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_2_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_2_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_2_RECONFIG_WR = 0;
//			break;
//		case 3:
//			TLM_REG_BITS.PORT_3_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_3_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_3_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_3_RECONFIG_WR = 0;
//			break;
//		case 4:
//			TLM_REG_BITS.PORT_4_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_4_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_4_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_4_RECONFIG_WR = 0;
//			break;
//		case 5:
//			TLM_REG_BITS.PORT_5_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_5_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_5_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_5_RECONFIG_WR = 0;
//			break;
//		case 6:
//			TLM_REG_BITS.PORT_6_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_6_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_6_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_6_RECONFIG_WR = 0;
//			break;
//		case 7:
//			TLM_REG_BITS.PORT_7_RATE_SELECT = v;
//			TLM_REG_BITS.PORT_7_RECONFIG_WR = 0;
//			TLM_REG_BITS.PORT_7_RECONFIG_WR = 1;
//			TLM_REG_BITS.PORT_7_RECONFIG_WR = 0;
//			break;
//	}
//}
//
/**Function changes the loopback mode on an interface.
 * @param[in] _port 	pointer to the port object
 * @param[in] _lb_mode which loopback mode.
 * @return RET_SUCCESS in case of success
 * @todo add graph
 */
ret_code_t set_if_port_loopback(port_t* _port, loopback_mode_t _lb_mode)
{
	int index = _port->index;
	if(_port->info.operation == PORT_OPERATION_CLIENT)
	{
		if PORT_FPGA1
		{
			switch(_lb_mode)
			{
				case MODE_LB_NORMAL:
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_1->bits.RX_SERIAL_LB_EN =0;
					/*#warning check if i will need to add this test
					 * if(!( //((index == 1) || (index == 3)) &&
						((PORT_CTRL(index-1).rate.next == PORT_RATE_OTU1)
						||(PORT_RATE_STM16 == PORT_CTRL(index-1).rate.next)
						||(PORT_RATE_FC2 == PORT_CTRL(index-1).rate.next))))*/
//					if((card.interfaces[index].port.control.rate.next == PORT_RATE_OTU1)
//					 ||(card.interfaces[index].port.control.rate.next == PORT_RATE_STM16)
//					 ||(card.interfaces[index].port.control.rate.next == PORT_RATE_FC2))
//					{
						set_if_port_rate(_port);
					//}

					break;
				case MODE_LB_CL:
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_1->bits.RX_SERIAL_LB_EN = 0;
					set_if_port_rate(_port);
					/*#warning do i have to do this or just calling set_if_port_rate?*/
					/*switch (index)
					{
						case 0:
							TLM_REG_1_BITS.PORT_0_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_0_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_0_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_0_RECONFIG_WR = 0;
							break;
						case 1:
							TLM_REG_1_BITS.PORT_1_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_1_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_1_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_1_RECONFIG_WR = 0;
							break;
						case 2:
							TLM_REG_1_BITS.PORT_2_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_2_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_2_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_2_RECONFIG_WR = 0;
							break;
						case 3:
							TLM_REG_1_BITS.PORT_3_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_3_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_3_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_3_RECONFIG_WR = 0;
							break;
						case 4:
							TLM_REG_1_BITS.PORT_4_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_4_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_4_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_4_RECONFIG_WR = 0;
							break;
						case 5:
							TLM_REG_1_BITS.PORT_5_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_5_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_5_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_5_RECONFIG_WR = 0;
							break;
						case 6:
							TLM_REG_1_BITS.PORT_6_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_6_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_6_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_6_RECONFIG_WR = 0;
							break;
						case 7:
							TLM_REG_1_BITS.PORT_7_RATE_SELECT = 6;
							TLM_REG_1_BITS.PORT_7_RECONFIG_WR = 0;
							TLM_REG_1_BITS.PORT_7_RECONFIG_WR = 1;
							TLM_REG_1_BITS.PORT_7_RECONFIG_WR = 0;
							break;
					}*/
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 1;
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_1->bits.RESET_GXB = 0;
					break;
				case MODE_LB_LN:
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_1->bits.RX_SERIAL_LB_EN = 1;
					set_if_port_rate(_port);
					break;
				default:
					break;
			}
		}
	else
	{
		if PORT_FPGA2
		{
			switch(_lb_mode)
			{
				case MODE_LB_NORMAL:
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_2->bits.RX_SERIAL_LB_EN =0;
//					if(!(((index == 1) || (index == 3))
//						 &&	((PORT_RATE_STM16 == PORT_CTRL(index-1).rate.next)
//							||(PORT_RATE_FC2 == PORT_CTRL(index-1).rate.next))))
//					{
					if((card.interfaces[index].port.control.rate.next == PORT_RATE_OTU1)
					 ||(card.interfaces[index].port.control.rate.next == PORT_RATE_STM16)
					 ||(card.interfaces[index].port.control.rate.next == PORT_RATE_FC2))
					{
						set_if_port_rate(_port);
					}
					break;
				case MODE_LB_CL:
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_2->bits.RX_SERIAL_LB_EN = 0;
					set_if_port_rate(_port);
					/*#warning do i have to do this or just calling set_if_port_rate?*/
					/*switch (index)
					{
						case 0:
							TLM_REG_2_BITS.PORT_0_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_0_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_0_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_0_RECONFIG_WR = 0;
							break;
						case 1:
							TLM_REG_2_BITS.PORT_1_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_1_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_1_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_1_RECONFIG_WR = 0;
							break;
						case 2:
							TLM_REG_2_BITS.PORT_2_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_2_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_2_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_2_RECONFIG_WR = 0;
							break;
						case 3:
							TLM_REG_2_BITS.PORT_3_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_3_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_3_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_3_RECONFIG_WR = 0;
							break;
						case 4:
							TLM_REG_2_BITS.PORT_4_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_4_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_4_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_4_RECONFIG_WR = 0;
							break;
						case 5:
							TLM_REG_2_BITS.PORT_5_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_5_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_5_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_5_RECONFIG_WR = 0;
							break;
						case 6:
							TLM_REG_2_BITS.PORT_6_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_6_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_6_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_6_RECONFIG_WR = 0;
							break;
						case 7:
							TLM_REG_2_BITS.PORT_7_RATE_SELECT = 6;
							TLM_REG_2_BITS.PORT_7_RECONFIG_WR = 0;
							TLM_REG_2_BITS.PORT_7_RECONFIG_WR = 1;
							TLM_REG_2_BITS.PORT_7_RECONFIG_WR = 0;
							break;
					}*/
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 1;
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_2->bits.RESET_GXB = 0;
					break;
				case MODE_LB_LN:
					CL_MMAP(index)->odtu_brick->odtu_ctrl_reg_2->bits.RX_SERIAL_LB_EN =1;
					break;
				default:
					break;
			}
		 }
	   }
	}
	else
	{
		switch(_lb_mode)
		{
			case MODE_LB_NORMAL:
				switch (index - LN_INDEX_START)
				{
					case 0:
						LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LINE_LOOPBACK = 0;
						break;
					case 1:
						LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LINE_LOOPBACK = 0;
						break;
				}
				break;
				case MODE_LB_LN:
					switch (index - LN_INDEX_START)
					{
					case 0:
						LN_MMAP(0)->otu2_brick_1.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LINE_LOOPBACK = 1;
						break;
					case 1:
						LN_MMAP(1)->otu2_brick_2.top_level.otu2_brick_li_gen_ctrl_reg->bits.LI1_LINE_LOOPBACK = 1;
						break;
						default:
							break;
					}
				break;
				case MODE_LB_CL:
					break;
		}
	}
	return (RET_SUCCESS);
}

/**
 * Callback function when a transceiver state machine changes state.
 * @param _trans	pointer to the transceiver object
 * Return none
 */
void callback_on_transceiver_state_change(transceiver_t* _trans)
{
	if(_trans != NULL)
	{
		if(is_in_monitor_state(_trans) == RET_SUCCESS)
		{
			if(_trans->info.type == TRANS_SFP_ELECTRICAL)
			{
				sfp_device_t* _sfp = (sfp_device_t*)_trans->device.device;
				/* Configure the SFP so it can operate in ETHERNET mode*/
				enable_sfp_sgmii(_sfp);
				enable_sfp_ge(_sfp);
			}
			else
			if(_trans->info.type == TRANS_SFP_OPTICAL)
			{
				if(CARDINT_port(_trans->index).info.operation == PORT_OPERATION_CLIENT)
				{
					reconfigure_ge_port(&card.interfaces[_trans->index].port);
				}
			}
			else
			{
				/*shouldn't be here*/
			}
		}
	}
}
/**
 * Callback function when a transceiver plug machine changes state.
 * @param _trans	pointer to the transceiver object
 * Return none
 */
void callback_on_transceiver_plug_change(transceiver_t* _trans)
{
	uint32_t index = _trans->index;
	port_t* _port = &CARDINT_port(_trans->index);
	if(_trans->monitor.absent.current)
	{
		_trans->info.type = TRANS_SFP_OPTICAL;
		/*clear all alarms if SFP is absent*/
		_port->monitor.statistics.generic_sts.alarms.all = 0;
		odux_alarms[_trans->index].all = 0;
		reset_if_kurs_power_threshold(&card.interfaces[index]);
		reset_port_counters(_port);
		reset_port_statistics(_port);
		if(_port->index < 9 )
		{
			clear_ge_counters_ol(CL_MMAP(index)->gemac_1);
		}
		else
		{
			clear_ge_counters_ol(CL_MMAP(index)->gemac_2);
		}
	}
}

/**
 * Function to reset kurs power threshold to default values
 * @param[in] _if_ptr pointer to the interface object
 * Return none
 */
void reset_if_kurs_power_threshold(interface_t* _if_ptr)
{
	alt_u8 if_index = _if_ptr->index;
	pin[if_index].physical_max = DEF_SFP_PIN_HMAX;
	pin[if_index].critical_max = DEF_SFP_PIN_CMAX;
	pin[if_index].warning_max = DEF_SFP_PIN_WMAX;
	pin[if_index].warning_min = DEF_SFP_PIN_WMIN;
	pin[if_index].critical_min = DEF_SFP_PIN_CMIN;
	pin[if_index].physical_min = DEF_SFP_PIN_HMIN;
	pout[if_index].physical_max = DEF_SFP_POUT_HMAX;
	pout[if_index].critical_max = DEF_SFP_POUT_CMAX;
	pout[if_index].warning_max = DEF_SFP_POUT_WMAX;
	pout[if_index].warning_min = DEF_SFP_POUT_WMIN;
	pout[if_index].critical_min = DEF_SFP_POUT_CMIN;
	pout[if_index].physical_min = DEF_SFP_POUT_HMIN;
}
