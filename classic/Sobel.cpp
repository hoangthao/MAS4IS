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
#include "Sobel.h"

void Sobel::statistic(Environment *result, Environment *gt, const char* fn) {
	ofstream file;
	file.open(fn);
	if (!file) {
		cerr << "Cannot save the files statistics sobel" << endl;
		return;
	}
	map<int, int> nbCount;
	int size = env->x * env->y * env->z;
	file << "number voxels: " << size << endl;
	for (int j = 0; j < size; j++) {
		nbCount[result->voxels[j]]++;
	}

	int grp = 0;
	for (map<int, int>::iterator iter = nbCount.begin(); iter != nbCount.end();
			++iter) {
		file << "group " << grp << " (intensity=" << iter->first << ") : "
				<< iter->second << " voxels" << endl;
		grp++;
	}

	file << "\ncompare to ground truth image" << endl;

	vector<string> regions = split(conf->infos["gray_ground_truth"], ',');
	map<int, map<int, int> > stat;

	for (int i = 0; i < regions.size(); i++) {
		map<int, int> rs;
		stat[i] = rs;
	}
	for (int j = 0; j < size; j++) {
		stat[gt->voxels[j]][result->voxels[j]]++;
	}

	for (int i = 0; i < regions.size(); i++) {
		map<int, int> rs = stat[i];
		file << "group " << i << endl;
		for (map<int, int>::iterator iter = rs.begin(); iter != rs.end();
				++iter) {
			file << "\t(intensity=" << iter->first << ") : " << iter->second
					<< " voxels" << endl;
		}
	}

	file.close();
}

void Sobel::run() {
	int xFilter[3][3][3] = { { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } }, { {
			-2, 0, 2 }, { -4, 0, 4 }, { -2, 0, 2 } }, { { -1, 0, 1 },
			{ -2, 0, 2 }, { -1, 0, 1 } } };
	int yFilter[3][3][3] = { { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } }, { {
			2, 4, 2 }, { 0, 0, 0 }, { -2, -4, -2 } }, { { 1, 2, 1 },
			{ 0, 0, 0 }, { -1, -2, -1 } } };
	int zFilter[3][3][3] = { { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } }, { { 0,
			0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { -1, -2, -1 },
			{ -2, -4, -2 }, { -1, -2, -1 } } };

	byte tmp = 0;
	int gx, gy, gz;

	for (int k = 0; k < env->z; k++) {
		for (int j = 0; j < env->y; j++) {
			for (int i = 0; i < env->x; i++) {
				gx = gy = gz = 0;
				for (int o = -1; o <= 1; o++) {
					if (k + o < 0 || k + o >= env->z)
						continue;
					for (int n = -1; n <= 1; n++) {
						if (j + n < 0 || j + n >= env->y)
							continue;
						for (int m = -1; m <= 1; m++) {
							if (i + m < 0 || i + m >= env->x)
								continue;
							tmp = env->getValue(i + m, j + n, k + o);
							gx += xFilter[o + 1][n + 1][m + 1] * tmp;
							gy += yFilter[o + 1][n + 1][m + 1] * tmp;
							gz += zFilter[o + 1][n + 1][m + 1] * tmp;
						}
					}
				}
				tmp = sqrt(gx * gx + gy * gy + gz * gz) / 1.732; // sqrt(3) = 1.73205
				result[k * env->x * env->y + j * env->x + i] = tmp;
			}
		}
	}
}

