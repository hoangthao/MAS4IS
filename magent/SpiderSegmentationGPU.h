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

#ifndef SPIDERSEGMENTATIONGPU_H_
#define SPIDERSEGMENTATIONGPU_H_

#include "SpiderSegmentation.h"
#include "../core/misc_ext.h"
#include "SpiderInfo.h"

class SpiderSegmentationGPU: public SpiderSegmentation {
public:
	spider *sp;
	void initAgents();
	void runNStep();
	SpiderSegmentationGPU(Config *_conf, Environment *_env) :
			SpiderSegmentation(_conf, _env) {
		sp = 0;
	}

	void copy(SpiderInfo *from, spiderinfo *to) {
		to->backprobability = from->backprobability;
		to->constantWeight = from->constantWeight;
		to->numberAgentPerColony = from->numberAgentPerColony;
		to->numberColony = from->numberColony;
		to->pAttractOther = from->pAttractOther;
		to->pAttractSelf = from->pAttractSelf;
		to->pSaturation = from->pSaturation;
	}

};

#endif /* SPIDERSEGMENTATIONGPU_H_ */
