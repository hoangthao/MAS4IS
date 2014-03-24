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

#ifndef SEGMENTATION_H_
#define SEGMENTATION_H_

#include "Environment.h"
#include "Config.h"

class Segmentation {
public:
	Environment *env;
	Config* conf;

	Segmentation(Environment *_env, Config* _conf) {
		env = _env;
		conf = _conf;
	}
	virtual ~Segmentation() {
		free(env);
	}
	virtual void initialize()=0;
	virtual void run()=0;
	virtual void exportImage(Environment *result)=0;
	void excute(Environment *result) {
		initialize();
		run();
		exportImage(result);
	}
	virtual void statistic(Environment *result, Environment *gt,
			const char* fn)=0;
};

#endif /* SEGMENTATION_H_ */
