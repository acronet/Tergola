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



#ifndef MB7040_H_
#define MB7040_H_

typedef union {
	int16_t wval;
	int8_t bval[2];
} MB7040VAL;

typedef struct {
	int16_t val;
	int16_t val_noPeaks;
	int16_t adcVal;
	int16_t maxVal;
	int16_t minVal;
	int16_t medianVal;
} LG_MB7040_STATS;

void MB7040_init(void);
void MB7040_triggerReading(void);
void MB7040_getStats(LG_MB7040_STATS * const );
void MB7040_resetStats(void);
#endif /* MB7040_H_ */