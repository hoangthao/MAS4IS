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

#include "Spider.h"
#include "SpiderInfo.h"

void Spider::perception(Environment* env, Info *info) {

	SpiderInfo *si = (SpiderInfo*) info;

	si->size1 = 0;
	si->size2 = 0;
	//perception
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
				si->neigh[si->size1] = env->getIdx(m + posision.x,
						n + posision.y, p + posision.z);
				si->size1++;
			}
		}
	}
	int idxPos = env->getIdx(posision.x, posision.y, posision.z);
	for (int k = 0; k < si->nbDl; k++) {
		if (si->dls[idxPos * si->nbDl + k] != -1) {
			si->scut[si->size2] = si->dls[idxPos * si->nbDl + k];
			si->size2++;
		}
	}
}

void Spider::decision(Environment* env, Info *info, int &next) {
	SpiderInfo *si = (SpiderInfo*) info;
	mapx probability[26 + si->nbDl];
	int proIdx = 0;
	int randomnext[26 + si->nbDl];
	int randIdx = 0;
	float totalWeight = 0;
	int idxPos = env->getIdx(posision.x, posision.y, posision.z);
	int idxLP = env->getIdx(lastposision.x, lastposision.y, lastposision.z);
	for (int k = 0; k < si->size1; k++) {
		if (idxLP == si->neigh[k])
			continue;
		probability[proIdx].key = si->neigh[k];
		probability[proIdx].value = si->constantWeight;
		totalWeight += si->constantWeight;
		proIdx++;
	}

	for (int m = 0; m < si->size2; m++) {
		if (idxLP == si->scut[m])
			continue;
		float ws = 0;
		int tmp = 0;
		for (int k = 0; k < si->nbDl; k++) {
			tmp = idxPos * si->nbDl + k;
			if (si->dls[tmp] == si->scut[m]) {
				float n1 = si->nbSilkColony[tmp * info->numberColony + colony];
				float sum = 0;
				for (int h = 0; h < info->numberColony; h++) {
					sum += si->nbSilkColony[tmp * info->numberColony + h];
				}
				ws = si->pAttractSelf * min(n1, si->pSaturation);
				ws += si->pAttractOther * min(sum - n1, si->pSaturation);
				int index = isAccess(si->neigh, si->size1, si->scut[m]);
				if (index != -1) {
					probability[index].value += ws;
				} else {
					probability[proIdx].key = si->scut[m];
					probability[proIdx].value = ws;
					proIdx++;
				}
				totalWeight += ws;
				break;
			}
		}
	}

	float maxV = 0, tmp;
	randIdx = 0;
	for (int k = 0; k < proIdx; k++) {
		tmp = probability[k].value / totalWeight;
		if (tmp > maxV) {
			randIdx = 0;
			randomnext[randIdx] = probability[k].key;
			randIdx++;
			maxV = tmp;
		} else if (tmp == maxV) {
			randomnext[randIdx] = probability[k].key;
			randIdx++;
		}
	}
	next = randomnext[rand() % randIdx];
}

void Spider::action(Environment* env, Info *info, int next) {
	SpiderInfo *si = (SpiderInfo*) info;
	float gau = gaussian(env->voxels[next], si->mean[colony],
		si->selectivity[colony]/2);

	int idxPos = env->getIdx(posision.x, posision.y, posision.z);

	int pz = next / (env->x * env->y);
	int py = next % (env->x * env->y) / env->x;
	int px = next % (env->x * env->y) % env->x;
	Posision nextPosision(px, py, pz);

	if (gau > 0.01) {
		bool found = false;
		int tmp = 0;
		for (int k = 0; k < si->nbDl; k++) {
			tmp = idxPos * si->nbDl + k;
			if (si->dls[tmp] == next) {
				si->nbSilkColony[tmp * info->numberColony + colony]++;
//				si->nbSilk[tmp]++;
				found = true;
				break;
			}
		}
		if (!found) {
			for (int k = 0; k < si->nbDl; k++) {
				tmp = idxPos * si->nbDl + k;
				if (si->dls[tmp] == -1) {
					si->dls[tmp] = next;
//					si->nbSilk[tmp]++;
					si->nbSilkColony[tmp * info->numberColony + colony]++;
					break;
				}
			}
		}

		found = false;
		for (int k = 0; k < si->nbDl; k++) {
			tmp = next * si->nbDl + k;
			if (si->dls[tmp] == idxPos) {
				si->nbSilkColony[tmp * info->numberColony + colony]++;
//				si->nbSilk[tmp]++;
				found = true;
				break;
			}
		}
		if (!found) {
			for (int k = 0; k < si->nbDl; k++) {
				tmp = next * si->nbDl + k;
				if (si->dls[tmp] == -1) {
					si->dls[tmp] = idxPos;
//					si->nbSilk[tmp]++;
					si->nbSilkColony[tmp * info->numberColony + colony]++;
					break;
				}
			}
		}

		lastposision = nextPosision;
	}

	float r = (float) rand() / 2147483647;
	if (r > si->backprobability) {
		posision = nextPosision;
	}

}

int Spider::isAccess(int *neigh, int size1, int p) {
	for (int i = 0; i < size1; i++) {
		if (neigh[i] == p)
			return i;
	}
	return -1;
}

