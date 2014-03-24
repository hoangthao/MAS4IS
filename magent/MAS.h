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
#ifndef MAS_H_
#define MAS_H_

#include "Agent.h"
#include "../core/Info.h"

class MAS {
public:
	Agent* aglist;
	Info* info;
	MAS() {
		aglist = 0;
		info = 0;
	}
	~MAS() {
		free(aglist);
		free(info);
	}
};

#endif /* MAS_H_ */
