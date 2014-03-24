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

#ifndef ANTSEGMENTATIONGPU_H_
#define ANTSEGMENTATIONGPU_H_

#include "AntSegmentation.h"
#include "../core/misc_ext.h"
#include "AntInfo.h"

class AntSegmentationGPU: public AntSegmentation {
public:
	ant *sp;
	void initAgents();
	void runNStep();
	AntSegmentationGPU(Config *_conf, Environment *_env) :
			AntSegmentation(_conf, _env) {
		sp = 0;
	}

	void copy(AntInfo *from, antinfo *to) {
		to->c1 = from->c1;
		to->c2 = from->c2;
		to->c3 = from->c3;
		to->c4 = from->c4;
		to->evaporatePeromone = from->evaporatePeromone;
		to->highRanking = from->highRanking;
		to->numberAgentPerColony = from->numberAgentPerColony;
		to->numberColony = from->numberColony;
	}

};

#endif /* ANTSEGMENTATIONGPU_H_ */
