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
#ifndef CLASSICSEGMENTATION_H_
#define CLASSICSEGMENTATION_H_

#include "../core/Segmentation.h"
#include "../core/Environment.h"

class ClassicSegmentation: public Segmentation {
public:
	byte *result;
	ClassicSegmentation(Environment *_env, Config *_conf) :
			Segmentation(_env, _conf) {
		result = new byte[env->x * env->y * env->z];
	}
	~ClassicSegmentation() {
		free(result);
	}
	virtual void initialize()=0;
	virtual void run()=0;
	inline void exportImage(Environment *_result) {
		_result->x = env->x;
		_result->y = env->y;
		_result->z = env->z;
		_result->voxels = new byte[env->x * env->y * env->z];
		memcpy(_result->voxels, result, env->x * env->y * env->z);
	}
	virtual void statistic(Environment *result, Environment *gt,
			const char* fn)=0;
};

#endif /* CLASSICSEGMENTATION_H_ */
