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
#include "Otsu.h"

void Otsu::statistic(Environment *result, Environment *gt, const char* fn) {

	ofstream file;
	file.open(fn);
	if (!file) {
		cerr << "Cannot save the files statistics_otsu" << endl;
		return;
	}
	int nbCount[nbLevel];
	fill_n(nbCount, nbLevel, 0);
	int size = env->x * env->y * env->z;
	file << "number voxels: " << size << endl;

	char* marked = (char*) malloc(size * sizeof(char));
	char* markedCan = (char*) malloc(size * sizeof(char));
	int* candidate = (int*) malloc(size * sizeof(int));
	int idxCan = 0;
	int crr;
	int numberVoxel = 0;

	fill_n(marked, size, 0);
	fill_n(markedCan, size, 0);

	for (int i = 0; i < size; i++) {
		if (marked[i] == 0) {
			idxCan = 0;
			candidate[idxCan++] = i;
			markedCan[i] = 1;

			numberVoxel = 1;
			file << (int) result->voxels[i] << ":";

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
			file << numberVoxel << endl;
		}
	}

	file << "ground truth" << endl;

	idxCan = 0;
	numberVoxel = 0;
	fill_n(marked, size, 0);
	fill_n(markedCan, size, 0);

	for (int i = 0; i < size; i++) {
		if (marked[i] == 0) {
			idxCan = 0;
			candidate[idxCan++] = i;
			markedCan[i] = 1;
			numberVoxel = 1;
			file << (int) gt->voxels[i] << ":";

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
									&& gt->voxels[crr] == gt->voxels[idx]) {
								//file << idx << ",";
								numberVoxel++;
								candidate[idxCan++] = idx;
								markedCan[idx] = 1;
							}
						}
					}
				}
			}
			file << numberVoxel << endl;
		}
	}

	free(marked);
	free(markedCan);

	file.close();

}

void Otsu::initialize() {
	nbLevel = atoi(conf->infos["number_level_threshold"].c_str());
}

void Otsu::run() {
	int size = env->x * env->y * env->z;

	int L = env->grayScale;
	int *histogram = new int[L];
	std::fill_n(histogram, L, 0);
	for (int i = 0; i < env->x * env->y * env->z; i++) {
		histogram[env->voxels[i]]++;
	}

	float P[L][L], S[L][L], H[L][L];
	P[0][0] = S[0][0] = H[0][0] = histogram[0];
	for (int i = 1; i < L; i++) {
		P[0][i] = P[0][i - 1] + histogram[i];
		S[0][i] = S[0][i - 1] + i * histogram[i];
		if (P[0][i] == 0)
			H[0][i] = 0;
		else
			H[0][i] = S[0][i] * S[0][i] / P[0][i];
	}
	for (int i = 1; i < L; i++) {
		for (int j = 1; j < L; j++) {
			P[i][j] = P[0][j] - P[0][i - 1];
			S[i][j] = S[0][j] - S[0][i - 1];
			if (P[i][j] == 0)
				H[i][j] = 0;
			else
				H[i][j] = S[i][j] * S[i][j] / P[i][j];
		}
	}

	//int* t = new int[nbLevel+1];
	t = new int[nbLevel + 1];
	std::fill_n(t, nbLevel, 0);

	t[0] = 0;
	float maxSig = 0;
	switch (nbLevel) {
	case 2:
		for (int i = 1; i < L - nbLevel; i++) // t1
				{
			float Sq = H[1][i] + H[i + 1][255];
			if (maxSig < Sq) {
				t[1] = i;
				maxSig = Sq;
			}
		}
		break;
	case 3:
		for (int i = 1; i < L - nbLevel; i++) // t1
			for (int j = i + 1; j < L - nbLevel + 1; j++) // t2
					{
				float Sq = H[1][i] + H[i + 1][j] + H[j + 1][255];
				if (maxSig < Sq) {
					t[1] = i;
					t[2] = j;
					maxSig = Sq;
				}
			}
		break;
	case 4:
		for (int i = 1; i < L - nbLevel; i++) // t1
			for (int j = i + 1; j < L - nbLevel + 1; j++) // t2
				for (int k = j + 1; k < L - nbLevel + 2; k++) // t3
						{
					float Sq = H[1][i] + H[i + 1][j] + H[j + 1][k]
							+ H[k + 1][255];
					if (maxSig < Sq) {
						t[1] = i;
						t[2] = j;
						t[3] = k;
						maxSig = Sq;
					}
				}
		break;
	case 5:
		for (int i = 1; i < L - nbLevel; i++) // t1
			for (int j = i + 1; j < L - nbLevel + 1; j++) // t2
				for (int k = j + 1; k < L - nbLevel + 2; k++) // t3
					for (int m = k + 1; m < L - nbLevel + 3; m++) // t4
							{
						float Sq = H[1][i] + H[i + 1][j] + H[j + 1][k]
								+ H[k + 1][m] + H[m + 1][255];
						if (maxSig < Sq) {
							t[1] = i;
							t[2] = j;
							t[3] = k;
							t[4] = m;
							maxSig = Sq;
						}
					}
		break;
	}

	t[nbLevel] = 255;

	for (int i = 0; i < nbLevel; ++i) {
		cout << "level " << i << ":" << t[i];
		int count = 0;
		for (int j = 0; j < size; ++j) {
			if (env->voxels[j] >= t[i] && env->voxels[j] < t[i + 1]) {
				result[j] = t[i];
				count++;
			}
		}
		cout << "->" << count << endl;
	}

	free(histogram);
	//free(t);
}

