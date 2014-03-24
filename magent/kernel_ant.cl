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

typedef unsigned char byte;
typedef unsigned short ushort;

typedef struct _antinfo {
	int numberColony;
	int numberAgentPerColony;
	float highRanking;
	float evaporatePeromone;
	float c1, c2, c3, c4;
} antinfo;

typedef struct _ant {
	int posision, lastposision;
	byte colony;
} ant;

typedef struct _map {
	int key;
	float value;
} mapx;

int env_getIdx(int x, int y, int z, int dx, int dy, int dz) {
	return dz * x * y + dy * x + dx;
}

float computeWeightGrad(int px,int py, int pz,int x, int y, int z, __global byte* sobel) {
	int neighX[26],neighY[26],neighZ[26];
	int size1=0;
	for (int p = -1; p <= 1; p++) {
		if (p + pz < 0 || p + pz >= z) continue;
		for (int n = -1; n <= 1; n++) {
			if (n + py < 0 || n + py >= y) continue;
			for (int m = -1; m <= 1; m++) {
				if (m + px < 0 || m + px >= x) continue;
				if (m == n && n == p && p == 0) continue;
				neighX[size1] = m + px;
				neighY[size1] = n + py;
				neighZ[size1] = p + pz;

				size1++;
			}
		}
	}

	float max=0,tmp=0;
	for (int i=0; i < size1; i++) {
		tmp = sobel[neighX[i] + neighY[i]*x + neighZ[i]*x*y];
		if (tmp>max) {
			max =tmp;
		}
	}
	return max;
}

__kernel
void run(__global ant *sp, __global float *pheronomes, __global float *pheronomesAdd, int size,
		antinfo info, __global byte *rands, __global byte *voxels, int x, int y, int z,
		__global byte *sobel) {

	int j = get_global_id(0);
	if (j>=size) return;
	int neigh[26];
	int size1=0;
	//perception
	int pos = sp[j].posision;

	int pz = pos / (x*y);
	int py = pos % (x*y) / x;
	int px = pos % (x*y) % x;
	for (int p = -1; p <= 1; p++) {
		if (p + pz < 0 || p + pz >= z) continue;
		for (int n = -1; n <= 1; n++) {
			if (n + py < 0 || n + py >= y) continue;
			for (int m = -1; m <= 1; m++) {
				if (m + px < 0 || m + px >= x) continue;
				if (m == n && n == p && p == 0) continue;
				int tmpId = env_getIdx(x,y,z, m + px, n + py, p + pz);
				if (sp[j].lastposision!=tmpId) {
					neigh[size1] = tmpId;
					size1++;
				}
			}
		}
	}

	mapx wPhiArr[26];
	mapx probability[26];

	int proIdx=0;
	float totalWeightDec=0;
	float weightDec=0;
	for (int k=0;k < size1; k++) {
		if (sp[j].lastposision==neigh[k]) continue;
		if (pheronomesAdd[k]!=0) continue;
		//computeWeightMove(p)
		float wMove = 1;
		int nz = neigh[k] / (x*y);
		int ny = neigh[k] % (x*y) / x;
		int nx = neigh[k] % (x*y) % x;
		if (ny > py) wMove = info.c1;
		else if (ny == py && nx == px) wMove = info.c2;
		else if (nx == px && ny < py) wMove = info.c3;
		else if (ny == py) wMove = info.c3;
		else wMove = info.c4;

		//computeWeightGrad(p)
		float wGrad = computeWeightGrad(nx,ny,nz,x,y,z,sobel)/(255*size1);

		//computeWeightVarv(position,p)
		float wVarv = 0;
		if (voxels[neigh[k]] == voxels[pos]) wVarv = 1;
		else wVarv = 1/(float)abs(voxels[neigh[k]] - voxels[pos]);

		//computeWeightPha(p)
		float wPhi = 0;
		wPhi = pheronomes[neigh[k]] + info.highRanking * wVarv * wGrad;
		wPhiArr[proIdx].key=neigh[k];
		wPhiArr[proIdx].value=info.highRanking * wVarv * wGrad;

		weightDec = wMove * wGrad * wPhi;

		probability[proIdx].key=neigh[k];
		probability[proIdx].value=weightDec;
		totalWeightDec += weightDec;
		proIdx++;
	}

	float maxV=0,tmp;
	int randomnext[26];
	int randIdx=0;
	for (int k=0;k<proIdx;k++) {
		if (pheronomesAdd[probability[k].key]!=0) continue;
		tmp = probability[k].value/totalWeightDec;
		if (tmp > maxV) {
			randIdx=0;
			randomnext[randIdx] = probability[k].key;
			randIdx++;
			maxV=tmp;
		} else if (tmp == maxV) {
			randomnext[randIdx] = probability[k].key;
			randIdx++;
		}
	}

	if (randIdx==0) {
		return;
	}

	int posNext=rands[j]%randIdx;

	//action
	sp[j].lastposision = sp[j].posision;
	sp[j].posision = randomnext[posNext];

	pheronomesAdd[randomnext[posNext]] = wPhiArr[posNext].value;
}

