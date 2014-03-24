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
#include "../core/misc.h"
#include "Region.h"

void RegionGW::statistic(Environment *result, Environment *gt, const char* fn) {

	ofstream file;
	file.open(fn);
	if (!file) {
		cerr << "Cannot save the files statistics region" << endl;
		return;
	}
	int size = env->x * env->y * env->z;
	file << "number voxels: " << size << endl;

	char* marked = (char*) malloc(size * sizeof(char));
	fill_n(marked, size, 0);
	char* markedCan = (char*) malloc(size * sizeof(char));
	fill_n(markedCan, size, 0);
	int* candidate = (int*) malloc(size * sizeof(int));
	int idxCan = 0;
	int crr;
	int numberVoxel = 0;

	for (int i = 0; i < size; i++) {
		if (marked[i] == 0) {
			idxCan = 0;
			candidate[idxCan++] = i;
			markedCan[i] = 1;

			numberVoxel = 1;

			while (idxCan != 0) {
				crr = candidate[--idxCan];
				marked[crr] = 1;
				int pz = crr / (env->x * env->y);
				int py = crr % (env->x * env->y) / env->x;
				int px = crr % (env->x * env->y) % env->x;
				for (int p = -1; p <= 1; p++) {
					if (p + pz < 0 || p + pz >= env->z)
						continue;
					for (int n = -1; n <= 1; n++) {
						if (n + py < 0 || n + py >= env->y)
							continue;
						for (int m = -1; m <= 1; m++) {
							if (m + px < 0 || m + px >= env->x)
								continue;
							if (m == n && n == p && p == 0)
								continue;
							int idx = env->getIdx(m + px, n + py, p + pz);
							if (marked[idx] == 0 && markedCan[idx] == 0
									&& result->voxels[crr]
											== result->voxels[idx]) {
								//file << idx << ",";
								numberVoxel++;

								candidate[idxCan++] = idx;
								markedCan[idx] = 1;
							}
						}
					}
				}
			}
			if (numberVoxel > 10)
				file << (int) result->voxels[i] << ":" << numberVoxel << endl;
		}
	}

	free(marked);
	free(markedCan);

	file.close();
}
void RegionGW::initialize() {
	threshold = atoi(conf->infos["region_threshold"].c_str());
}

void RegionGW::run() {
	int size = env->x * env->y * env->z;

	char* marked = (char*) malloc(size * sizeof(char));
	fill_n(marked, size, 0);
	char* markedCan = (char*) malloc(size * sizeof(char));
	fill_n(markedCan, size, 0);
	int* candidate = (int*) malloc(size * sizeof(int));
	int idxCan = 0;
	int crr;
	for (int i = 0; i < size; i++) {
		if (marked[i] == 0) {
			idxCan = 0;
			candidate[idxCan++] = i;
			markedCan[i] = 1;
			result[i] = env->voxels[i];
			while (idxCan != 0) {
				crr = candidate[--idxCan];
				marked[crr] = 1;
				int pz = crr / (env->x * env->y);
				int py = crr % (env->x * env->y) / env->x;
				int px = crr % (env->x * env->y) % env->x;
				for (int p = -1; p <= 1; p++) {
					if (p + pz < 0 || p + pz >= env->z)
						continue;
					for (int n = -1; n <= 1; n++) {
						if (n + py < 0 || n + py >= env->y)
							continue;
						for (int m = -1; m <= 1; m++) {
							if (m + px < 0 || m + px >= env->x)
								continue;
							if (m == n && n == p && p == 0)
								continue;
							int idx = env->getIdx(m + px, n + py, p + pz);
							if (marked[idx] == 0 && markedCan[idx] == 0
									&& (abs(env->voxels[i] - env->voxels[idx])
											< threshold)) {
								result[idx] = result[i];
								candidate[idxCan++] = idx;
								markedCan[idx] = 1;
							}
						}
					}
				}
			}
		}

	}

	free(marked);
	free(markedCan);
}

