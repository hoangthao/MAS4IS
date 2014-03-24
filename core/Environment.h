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

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "misc.h"

class Environment {
public:
	int x, y, z, grayScale;
	byte *voxels;
	bool createEnvironment(const char* fn, int dx, int dy, int dz, int gs);
	inline int getIdx(int dx, int dy, int dz) {
		return dz * x * y + dy * x + dx;
	}
	inline byte getValue(int dx, int dy, int dz) {
		return voxels[dz * x * y + dy * x + dx];
	}
	inline void setvalue(int dx, int dy, int dz, byte t) {
		voxels[dz * x * y + dy * x + dx] = t;
	}
	bool writeSlice(const char* fn, int slice);
	bool writeRawb(const char* fn);
	void tostring(int idx, const char* cc);
	void copyEnvironment(Environment *c, int start, int nbSlice);

	int *histogram;
	void computeHistogram();
	void writeHistogram();

	void getNeighbors(int idx, int *nei, int &size);
};

#endif /* ENVIRONMENT_H_ */

