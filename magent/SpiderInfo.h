/*
 Multi-agent Image Segmentation (MAgIS)
 Copyright (C) 2013  Nguyen Hoang Thao (hoangthao.ng@gmail.com)

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see http://www.gnu.org/licenses/.
 */

#ifndef SPIDERINFO_H_
#define SPIDERINFO_H_

#include "../core/Info.h"

class SpiderInfo: public Info {
public:
	float backprobability;
	float pAttractSelf;
	float pAttractOther;
	float pSaturation;
	float constantWeight;

	float* selectivity;
	float* mean;

	int* dls;
//	ushort* nbSilk;
	byte *nbSilkColony;
	int nbDl;

	int *scut;
	int size2;
};

#endif /* SPIDERINFO_H_ */
