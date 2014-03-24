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

#define NB_DRAGLINE 15

typedef unsigned char byte;
typedef unsigned short ushort;

typedef struct _spiderinfo {
	int numberColony;
	int numberAgentPerColony;
	float backprobability;
	float pAttractSelf;
	float pAttractOther;
	float pSaturation;
	float constantWeight;
} spiderinfo;

typedef struct _spider {
	int posision, lastposision;
	byte colony;
} spider;

typedef struct _map {
	int key;
	float value;
} mapx;

int env_getIdx(int x, int y, int z, int dx, int dy, int dz) {
	return dz * x * y + dy * x + dx;
}

int isAccess(int *neigh, int size1, int p) {
	for (int i = 0; i < size1; i++) {
		if (neigh[i] == p)
			return i;
	}
	return -1;
}

float gaussian(byte value, float mean, float varDis) {
	float tmp = -(value - mean) * (value - mean) / (2 * varDis * varDis);
	return 1. / (varDis * sqrt(2. * M_PI)) * exp(tmp);
}

__kernel
void run2(__global spider *sp, __global int *dls,__global byte *nbSilkColonies,int size, int nbDl,
		spiderinfo info, int x, int y, int z,
		__global byte *rands,
		__global float *means,
		__global float *selectivities,
		__global byte *voxels
) {

	int j = get_global_id(0);
	if (j>=size) return;
	int neigh[26],scut[26+NB_DRAGLINE];
	int size1=0,size2=0;
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
				neigh[size1] = env_getIdx(x,y,z, m + px, n + py, p + pz);
				size1++;
			}
		}
	}
	for (int k=0;k < nbDl;k++) {
		if (dls[pos*nbDl+k]!=-1) {
			scut[size2] = dls[pos*nbDl+k];
			size2++;
		}
	}

	//decision
	mapx probability[26+NB_DRAGLINE];
	int proIdx=0;
	float totalWeight=0;
	for (int k=0;k < size1; k++) {
		if (sp[j].lastposision==neigh[k]) continue;
		probability[proIdx].key=neigh[k];
		probability[proIdx].value=info.constantWeight;
		totalWeight += info.constantWeight;
		proIdx++;
	}

	for (int m=0; m < size2; m++) {
		if (sp[j].lastposision==scut[m]) continue;
		float ws=0;int tmp=0;
		for (int k=0;k < nbDl;k++) {
			tmp = pos*nbDl+k;
			if (dls[tmp]==scut[m]) {
				float n1 = nbSilkColonies[tmp*info.numberColony+sp[j].colony];
				//float n2 = nbSilks[tmp] - n1;
				float n2=0;
				for (int h=0;h<info.numberColony;h++) {
					n2 += nbSilkColonies[tmp*info.numberColony+h];
				}
				ws = info.pAttractSelf * min(n1,info.pSaturation);
				ws += info.pAttractOther * min(n2-n1,info.pSaturation);
				int index = isAccess(neigh,size1,scut[m]);
				if (index!=-1) {
					probability[index].value +=ws;
				} else {
					probability[proIdx].key=scut[m];
					probability[proIdx].value=ws;
					proIdx++;
				}
				totalWeight += ws;
				break;
			}
		}
	}

	float maxV=0,tmp;
	int randomnext[100];
	int randIdx=0;
	for (int k=0;k<proIdx;k++) {
		tmp = probability[k].value/totalWeight;
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

	int next = randomnext[(rands[j])%randIdx];

	float gau = gaussian(voxels[next],means[sp[j].colony],selectivities[sp[j].colony]/2);

	if (gau > 0.01) {
		bool found=false;
		int tmp=0;
		for (int k=0;k < nbDl;k++) {
			tmp = pos*nbDl+k;
			if (dls[tmp]==next) {
				nbSilkColonies[tmp*info.numberColony+sp[j].colony]=1;
				found=true;
				break;
			}
		}
		if (!found) {
			for (int k=0;k < nbDl;k++) {
				tmp = pos*nbDl+k;
				if (dls[tmp]==-1) {
					dls[tmp]=next;
					nbSilkColonies[tmp*info.numberColony+sp[j].colony]=1;
					break;
				}
			}
		}

		found=false;
		for (int k=0;k < nbDl;k++) {
			tmp = next*nbDl+k;
			if (dls[tmp]==pos) {
				nbSilkColonies[tmp*info.numberColony+sp[j].colony]++;
				found=true;
				break;
			}
		}
		if (!found) {
			for (int k=0;k < nbDl;k++) {
				tmp = next*nbDl+k;
				if (dls[tmp]==-1) {
					dls[tmp] = pos;
					nbSilkColonies[tmp*info.numberColony+sp[j].colony]= 1;
					break;
				}
			}
		}

		sp[j].lastposision = next;
	}

	float r = (float)rands[j]/255;
	if ( r > info.backprobability) {
		sp[j].posision = next;
	}

}

