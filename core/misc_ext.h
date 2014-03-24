/*
 * misc_ext.h
 *
 *  Created on: Oct 7, 2013
 *      Author: thao
 */

#ifndef MISC_EXT_H_
#define MISC_EXT_H_

typedef struct _spiderinfo{
	int numberColony;
	int numberAgentPerColony;
	float backprobability;
	float pAttractSelf;
	float pAttractOther;
	float pSaturation;
	float constantWeight;
} spiderinfo;

typedef struct _spider{
	int posision, lastposision;
	byte colony;
} spider;

typedef struct _antinfo{
	int numberColony;
	int numberAgentPerColony;
	float highRanking;
	float evaporatePeromone;
	float c1,c2,c3,c4;
} antinfo;

typedef struct _ant{
	int posision, lastposision;
	byte colony;
} ant;


#endif /* MISC_EXT_H_ */
