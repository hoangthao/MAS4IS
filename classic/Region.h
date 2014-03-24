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
#ifndef REGION_H_
#define REGION_H_

#include "ClassicSegmentation.h"

class RegionGW: public ClassicSegmentation {
public:
	int threshold;
	RegionGW(Environment *_env, Config *_conf) :
			ClassicSegmentation(_env, _conf) {
		threshold = 0;
	}
	void initialize();
	~RegionGW() {
	}
	void run();
	void statistic(Environment *result, Environment *gt, const char* fn);
};

#endif /* REGION_H_ */
