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

#include "Ant.h"
#include "AntInfo.h"

void Ant::perception(Environment* env, Info *info) {
	AntInfo *ai = (AntInfo*) info;

	ai->size1 = 0;
	int idxPos = 0;
	int idxLP = env->getIdx(lastposision.x, lastposision.y, lastposision.z);
	for (int p = -1; p <= 1; p++) {
		if (p + posision.z < 0 || p + posision.z >= env->z)
			continue;
		for (int n = -1; n <= 1; n++) {
			if (n + posision.y < 0 || n + posision.y >= env->y)
				continue;
			for (int m = -1; m <= 1; m++) {
				if (m + posision.x < 0 || m + posision.x >= env->x)
					continue;
				if (m == n && n == p && p == 0)
					continue;
				idxPos = env->getIdx(m + posision.x, n + posision.y,
						p + posision.z);

				if (idxPos != idxLP) {
					ai->neigh[ai->size1] = idxPos;
					ai->size1++;
				}
			}
		}
	}
}

void Ant::decision(Environment* env, Info *info, int &next) {

	AntInfo *ai = (AntInfo*) info;
	mapx probability[26];
	int proIdx = 0;
	int randIdx = 0;

	int idxPos = env->getIdx(posision.x, posision.y, posision.z);

	float totalWeightDec = 0;
	float weightDec = 0;
	for (int k = 0; k < ai->size1; k++) {
		//computeWeightMove(p)
		float wMove = 1;
		int nz = ai->neigh[k] / (env->x * env->y);
		int ny = ai->neigh[k] % (env->x * env->y) / env->x;
		int nx = ai->neigh[k] % (env->x * env->y) % env->x;

		if (ny > posision.y)
			wMove = ai->c1;
		else if (ny == posision.y && nx == posision.x)
			wMove = ai->c2;
		else if (nx == posision.x && ny < posision.y)
			wMove = ai->c3;
		else if (ny == posision.y)
			wMove = ai->c3; //1/4
		else
			wMove = ai->c4; //1/8

		//computeWeightGrad(p)
		float wGrad = computeWeightGrad(env, nx, ny, nz, ai)
				/ (255 * ai->size1);

		//computeWeightVarv(position,p)
		float wVarv = 0;
		if (env->voxels[ai->neigh[k]] == env->voxels[idxPos])
			wVarv = 1;
		else
			wVarv = 1
					/ (float) abs(
							env->voxels[ai->neigh[k]] - env->voxels[idxPos]);

		//computeWeightPhi(p)
		float wPhi = 0;
		wPhi = ai->pheronomes[ai->neigh[k]] + ai->highRanking * wVarv * wGrad;
		ai->wPhiArr[proIdx].key = ai->neigh[k];
		ai->wPhiArr[proIdx].value = ai->highRanking * wVarv * wGrad;

		weightDec = wMove * wGrad * wPhi;

		probability[proIdx].key = ai->neigh[k];
		probability[proIdx].value = weightDec;
		totalWeightDec += weightDec;
		proIdx++;
	}

	float maxV = 0, tmp;
	randIdx = 0;
	for (int k = 0; k < proIdx; k++) {
		if (ai->pheronomesAddmore[probability[k].key] != 0)
			continue;
		tmp = probability[k].value / totalWeightDec;
		if (tmp > maxV) {
			randIdx = 0;
			ai->randomnext[randIdx] = probability[k].key;
			randIdx++;
			maxV = tmp;
		} else if (tmp == maxV) {
			ai->randomnext[randIdx] = probability[k].key;
			randIdx++;
		}
	}
	if (randIdx == 0) {
//		cout << "no move" << endl;
		next = -1;
		return;
	}
	next = rand() % randIdx;
}

void Ant::action(Environment* env, Info *info, int next) {
	if (next == -1)
		return;

	AntInfo *ai = (AntInfo*) info;
	lastposision = posision;

	int pz = ai->randomnext[next] / (env->x * env->y);
	int py = ai->randomnext[next] % (env->x * env->y) / env->x;
	int px = ai->randomnext[next] % (env->x * env->y) % env->x;
	Posision nextPosision(px, py, pz);

	posision = nextPosision;
	ai->pheronomesAddmore[ai->randomnext[next]] = ai->wPhiArr[next].value;

}

float Ant::computeWeightGrad(Environment *env, int px, int py, int pz,
		AntInfo *info) {
	int neighX[26], neighY[26], neighZ[26];
	int size1 = 0;
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
				neighX[size1] = m + px;
				neighY[size1] = n + py;
				neighZ[size1] = p + pz;

				size1++;
			}
		}
	}

	float max = 0, tmp = 0;
	for (int i = 0; i < size1; i++) {
		tmp = info->sobel[neighX[i] + neighY[i] * env->x
				+ neighZ[i] * env->x * env->y];
		if (tmp > max) {
			max = tmp;
		}
	}
	return max;
//	return max/total;
}

