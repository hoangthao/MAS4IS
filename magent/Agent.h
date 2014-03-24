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

#ifndef AGENT_H_
#define AGENT_H_

#include "../core/misc.h"
#include "../core/Environment.h"
#include "../core/Info.h"

class Posision {
public:
	byte x, y, z;
	Posision(byte _x, byte _y, byte _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	~Posision() {
	}
};

class Agent {
public:
	Agent() :
			posision(0, 0, 0), lastposision(0, 0, 0), colony(0) {
	}
	virtual ~Agent() {
	}
	Posision posision, lastposision;
	byte colony;
	virtual void perception(Environment* env, Info *info)=0;
	virtual void decision(Environment* env, Info *info, int &next)=0;
	virtual void action(Environment* env, Info *info, int next)=0;
};

#endif /* AGENT_H_ */
