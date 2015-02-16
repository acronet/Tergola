/*
 * ACRONET Project
 * http://www.acronet.cc
 *
 * Copyright ( C ) 2014 Acrotec srl
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the EUPL v.1.1 license.  See http://ec.europa.eu/idabc/eupl.html for details.
 */ 
#include <asf.h>
#include <stdio.h>
#include "board.h"
//#include "sysclk.h"
#include "twi_master.h"
//#include "led.h"
#include <stdio.h>
#include "globals.h"

#include "conf_board.h"
#include "MB7040.h"
#include "delay.h"


#define MB7040_TWI_ADDR  0x70

#define MB7040_TAKE_RANGE_COMMAND 0x51
#define MB7040_I2C_ADDR1_COMMAND 0xAA
#define MB7040_I2C_ADDR2_COMMAND 0xA5

static status_code_t MB7040_internalWrite(twi_package_t * ppak);
static status_code_t MB7040_internalRead(twi_package_t * ppak);

#define LG_EQUALMEASURES		2  // Number of measures with the same value we want to obtain
#define LG_NUMSTATS				24 // Number of reading involved in the getStats function
#define MEASUREBUFFERSIZE		256

static int16_t g_measureBuffer[MEASUREBUFFERSIZE];
static uint8_t g_index = 0;

static status_code_t MB7040_readDistance(MB7040VAL * val)
	{
	uint8_t cmd_buff1 = 0b01010001;

	debug_string(VERY_VERBOSE,PSTR("[MB7040_TriggerReadRange] IN\r\n"),true);
	
	twi_package_t pak = {
		.addr[0]	  = cmd_buff1,
		.addr_length  = 1,
		.chip         = MB7040_TWI_ADDR,
	};
	
	MB7040_internalWrite(&pak);
	
	delay_ms(100);
	
	pak.addr_length = 0;
	pak.chip = MB7040_TWI_ADDR;
	pak.buffer = val;
	pak.length = 2;

	delay_ms(2);
	debug_string(VERY_VERBOSE,PSTR("[MB7040_TriggerReadRange] OUT\r\n"),true);
	return MB7040_internalRead(&pak);
}

static status_code_t MB7040_internalWrite(twi_package_t * ppak)
{
	
	const status_code_t r = twi_master_write(AUX_TWI_PORT, ppak);
	switch(r) {
		case TWI_SUCCESS:
		debug_string(VERY_VERBOSE,PSTR("[MB7040_internalWrite] Write Succeeded\r\n"),true);
		return r;
		break;
		case ERR_IO_ERROR:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_IO_ERROR\r\n"),true);
		break;
		case ERR_FLUSHED:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_FLUSHED\r\n"),true);
		break;
		case ERR_TIMEOUT:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_TIMEOUT\r\n"),true);
		break;
		case ERR_BAD_DATA:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_BAD_DATA\r\n"),true);
		break;
		case ERR_PROTOCOL:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_PROTOCOL\r\n"),true);
		break;
		case ERR_UNSUPPORTED_DEV:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_UNSUPPORTED_DEV\r\n"),true);
		break;
		case ERR_NO_MEMORY:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_NO_MEMORY\r\n"),true);
		break;
		case ERR_INVALID_ARG:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_INVALID_ARG\r\n"),true);
		break;
		case ERR_BAD_ADDRESS:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_BAD_ADDRESS\r\n"),true);
		break;
		case ERR_BUSY:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_BUSY\r\n"),true);
		break;
		case ERR_BAD_FORMAT:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: ERR_BAD_FORMAT\r\n"),true);
		break;
		default:
		debug_string(NORMAL,PSTR("[MB7040_internalWrite] Write Failed: UNKONWN ERROR\r\n"),true);
	}
	return r;
}

static status_code_t MB7040_internalRead(twi_package_t * ppak)
{

	const status_code_t r = twi_master_read(AUX_TWI_PORT, ppak);
	switch(r) {
		case TWI_SUCCESS:
		debug_string(VERY_VERBOSE,PSTR("[MB7040_internalRead] Read Succeeded\r\n"),true);
		return r;
		break;
		case ERR_IO_ERROR:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ Failed: ERR_IO_ERROR\r\n"),true);
		break;
		case ERR_FLUSHED:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_FLUSHED\r\n"),true);
		break;
		case ERR_TIMEOUT:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_TIMEOUT\r\n"),true);
		break;
		case ERR_BAD_DATA:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_BAD_DATA\r\n"),true);
		break;
		case ERR_PROTOCOL:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_PROTOCOL\r\n"),true);
		break;
		case ERR_UNSUPPORTED_DEV:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_UNSUPPORTED_DEV\r\n"),true);
		break;
		case ERR_NO_MEMORY:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_NO_MEMORY\r\n"),true);
		break;
		case ERR_INVALID_ARG:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_INVALID_ARG\r\n"),true);
		break;
		case ERR_BAD_ADDRESS:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_BAD_ADDRESS\r\n"),true);
		break;
		case ERR_BUSY:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_BUSY\r\n"),true);
		break;
		case ERR_BAD_FORMAT:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: ERR_BAD_FORMAT\r\n"),true);
		break;
		default:
		debug_string(NORMAL,PSTR("[MB7040_internalRead] READ  Failed: UNKONWN ERROR\r\n"),true);

	}
	return r;
}

void MB7040_init(void)
{
	// TWI master initialization options.
	
	twi_master_options_t opt = {
		.speed =9600,
		.chip  = TWI_MASTER_ADDRESS,
	};
	
	// Initialize the TWI master driver.
	
	return twi_master_setup(AUX_TWI_PORT, &opt);
}

void MB7040_triggerReading(void)
{
	MB7040VAL range, range2;
	LG_MB7040_STATS levelstats;
	uint16_t rBuf=0;
	
	uint16_t measureBuf[3];
	int16_t delta;
	
	uint8_t samplecounter=0;
	char szBUF[64];
	
	while(1)
	{
		MB7040_readDistance(&range);
		range2.bval[1] = range.bval[0];
		range2.bval[0] = range.bval[1];
		//sprintf_P(szBUF,PSTR("R: %u\r\n"),range2.wval);
		//debug_string(VERBOSE,szBUF,false);
		delta = range2.wval - rBuf;
		if( delta <= 5 || delta >= -5)
		{
			measureBuf[samplecounter++]=range2.wval;
			//sprintf_P(szBUF,PSTR("sample_%d: %u\t\r\n"),samplecounter,range2.wval);
			//debug_string(VERBOSE,szBUF,false);
		}
		else
			rBuf=range2.wval;
		if(samplecounter==3)
			break;
	}
	uint16_t tempVal = 0;
	
	for (uint8_t i = 0 ; i < 3 ; i++)
	{
		for (uint8_t j = i+1 ; j < 3 ; j++)
		{
			if (measureBuf[i] > measureBuf[j])
			{
				tempVal =  measureBuf[i] ;
				measureBuf[i]  = measureBuf[j];
				measureBuf[j] = tempVal;
			}
		}
	}
	
	g_measureBuffer[g_index++] = measureBuf[1];
		
	//sprintf_P(szBUF,PSTR("%u - mean: %u\r\n"),g_index-1,g_measureBuffer[g_index-1]);
	//debug_string(VERBOSE,szBUF,false);
	memset(measureBuf, 0, 3 * sizeof(measureBuf[0]));
}

void MB7040_getStats(LG_MB7040_STATS * ps)
{
	LG_MB7040_STATS levelstats;
	char szBuf[128];
	uint32_t partialSum = 0;
	uint16_t start_index = 0, stop_index, tempVal;
	

	if (g_index >= LG_NUMSTATS)	//reading every 5 second --> 24 readings = 2 min
		start_index = g_index - LG_NUMSTATS;
	// Sort the measures array
	
	for (uint8_t i = start_index ; i < g_index ; i++)
	{
		for (uint8_t j = i+1 ; j < g_index ; j++)
		{
			if (g_measureBuffer[i] > g_measureBuffer[j])
			{
				tempVal =  g_measureBuffer[i] ;
				g_measureBuffer[i]  = g_measureBuffer[j];
				g_measureBuffer[j] = tempVal;
			}
		}
	}
	
	const uint16_t quartile = (g_index) / 8 ;
	start_index +=quartile;
	stop_index = g_index - quartile;
	//sprintf_P(szBuf,PSTR("start:%u\tstop:%u\tquartile:%u\r\n"),start_index,stop_index,quartile);
	//debug_string(NORMAL,szBuf,RAM_STRING);
	
	
	for (uint8_t i = start_index ; i < stop_index ; i++)
	{
		partialSum += g_measureBuffer[i];
		//sprintf_P(szBuf,PSTR("%u - %u\t%lu\r\n"),i,g_measureBuffer[i],partialSum);
		//debug_string(NORMAL,szBuf,RAM_STRING);
	}
	
	if(g_index)
	{
		levelstats.val = partialSum / (stop_index - start_index);
		levelstats.maxVal = g_measureBuffer[stop_index-1];
		levelstats.minVal = g_measureBuffer[start_index];
		levelstats.medianVal = g_measureBuffer[ start_index + (stop_index - start_index) / 2]; //num is the number of elements in the array
		levelstats.val_noPeaks = ( partialSum - levelstats.maxVal - levelstats.minVal ) / (stop_index - start_index - 2);
	}
	else
	{
		levelstats.val = -9998;
		levelstats.maxVal = -9998;
		levelstats.minVal = -9998;
		levelstats.medianVal = -9998;
		levelstats.val_noPeaks = -9998;
	}
	//measureBuf.adcVal=MB7062_adcGetValue();

	sprintf_P(szBuf,PSTR("val: %d - medianVal = %d - maxVal: %d - minVal: %d - Val_noPeaks: %d\r\n"),levelstats.val,levelstats.medianVal,levelstats.maxVal,levelstats.minVal,levelstats.val_noPeaks);
	debug_string(NORMAL,szBuf,false);
	memcpy_ram2ram(ps,&levelstats,sizeof(levelstats));
	MB7040_resetStats();
	
}

void MB7040_resetStats(void)
{
	memset(g_measureBuffer, 0, MEASUREBUFFERSIZE * sizeof(g_measureBuffer[0]));
	g_index = 0;
}