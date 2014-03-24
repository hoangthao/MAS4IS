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
#ifndef ANTSEGMENTATION_H_
#define ANTSEGMENTATION_H_

#include "AgentSegmentation.h"
#include "../core/Config.h"
#include "../core/Environment.h"

class AntSegmentation: public AgentSegmentation {
public:

	AntSegmentation(Config *_conf, Environment *_env);

	void exportImage(Environment *_result);
	void initialize();
	void runNStep();
	virtual void initAgents();
	void getSobel();

	void statistic(Environment *result, Environment *gt, const char* fn);
};

#endif /* ANTSEGMENTATION_H_ */
