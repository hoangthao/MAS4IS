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
#include "SpiderSegmentation.h"
#include "Spider.h"
#include "SpiderInfo.h"

void SpiderSegmentation::statistic(Environment *result, Environment *gt,
		const char* fn) {

	ofstream file;
	file.open(fn);
	if (!file) {
		cerr << "Cannot save the files statistic for spider algorithm" << endl;
		return;
	}

	int size = result->x * result->y * result->z;
	file << "number voxels: " << size << endl;

	map<int, map<int, int> > stat;

	for (int i = 0; i < 10; i++) {
		map<int, int> rs;
		stat[i] = rs;
	}
	for (int j = 0; j < size; j++) {
		stat[gt->voxels[j]][result->voxels[j]]++;
	}

	for (int i = 0; i < 10; i++) {
		map<int, int> rs = stat[i];
		file << "group " << i << endl;
		for (map<int, int>::iterator iter = rs.begin(); iter != rs.end();
				++iter) {
			if (iter->first == 0) {
				file << "\t(no visit) : " << iter->second << " voxels" << endl;
			} else {
				file << "\t(value=" << iter->first << ") : " << iter->second
						<< " voxels" << endl;
			}

		}
	}
	file.close();

}

SpiderSegmentation::SpiderSegmentation(Config *_conf, Environment *_env) :
		AgentSegmentation(_env, _conf) {
	data->info = new SpiderInfo();
}

void SpiderSegmentation::detectHistogram() {

	env->computeHistogram();

	vector<int> maxlist;
	vector<int> maxTmp;

	int count = 0;
	int *marked = new int[env->grayScale];
	do {
		std::fill_n(marked, env->grayScale, 0);
		count = 0;
		maxTmp.clear();
		do {
			int maxIdx = -1;
			int maxValue = -1;
			for (int i = 0; i < env->grayScale; i++) {
				if (env->histogram[i] > maxValue && marked[i] == 0) {
					maxIdx = i;
					maxValue = env->histogram[i];
				}
			}

			marked[maxIdx] = 1;

			maxTmp.push_back(maxIdx);
			count++;
			for (int i = maxIdx + 1; i < env->grayScale; i++) {
				if (env->histogram[i] <= env->histogram[i - 1]
						&& marked[i] == 0) {
					marked[i] = 1;
				} else {
					break;
				}
			}
			for (int i = maxIdx - 1; i >= 0; i--) {
				if (env->histogram[i] <= env->histogram[i + 1]
						&& marked[i] == 0) {
					marked[i] = 1;
				} else {
					break;
				}
			}
			int i;
			for (i = 0; i < env->grayScale; i++) {
				if (marked[i] == 0) {
					break;
				}
			}
			if (i == env->grayScale)
				break;
		} while (true);

		cout << "number of peak: " << count << endl;
		//smooth
		if (count > data->info->numberColony) {
			std::fill_n(marked, env->grayScale, 0);

			marked[0] = (env->histogram[0] + env->histogram[1]) / 2;
			for (int i = 1; i < env->grayScale - 1; i++) {
				marked[i] = (env->histogram[i - 1] + env->histogram[i]
						+ env->histogram[i + 1]) / 3;
			}
			marked[env->grayScale - 1] = (env->histogram[env->grayScale - 1]
					+ env->histogram[env->grayScale - 2]) / 2;

			for (int i = 0; i < env->grayScale; i++) {
				env->histogram[i] = marked[i];
			}
			maxlist.clear();
			for (size_t i = 0; i < maxTmp.size(); i++) {
				maxlist.push_back(maxTmp[i]);
			}
		}
	} while (count > data->info->numberColony);

	free(marked);

	while (maxlist.size() > data->info->numberColony) {
		maxlist.pop_back();
	}

	for (size_t i = 0, size = maxlist.size(); i < size - 1; i++) {
		for (size_t j = i + 1; j < size; j++) {
			if (maxlist[j] < maxlist[i]) {
				int swap = maxlist[i];
				maxlist[i] = maxlist[j];
				maxlist[j] = swap;
			}
		}
	}

	SpiderInfo *si = (SpiderInfo*) data->info;

	float* refLevel = new float[data->info->numberColony];
	si->selectivity = new float[data->info->numberColony];

	cout << "max-points: ";
	for (size_t i = 0, size = maxlist.size(); i < size; i++) {
		cout << maxlist[i] << " ";
		refLevel[i] = maxlist[i];
	}
	cout << endl;

	int begin, end, min, minIdx;
	vector<int> minlist;
	minlist.push_back(0);
	for (int i = 0; i < data->info->numberColony - 1; i++) {
		begin = maxlist[i];
		end = maxlist[i + 1];
		min = env->histogram[begin];
		minIdx = begin;
		for (int j = begin; j < end; j++) {
			if (env->histogram[j] < min) {
				min = env->histogram[j];
				minIdx = j;
			}
		}
		minlist.push_back(minIdx);
	}
	minlist.push_back(env->grayScale - 1);
	cout << "min-points: ";
	for (size_t i = 0, size = minlist.size(); i < size; ++i) {
		cout << minlist[i] << " ";
	}
	cout << '\n';
	cout << "range: ";
	for (int i = 0; i < data->info->numberColony; i++) {
		si->selectivity[i] = minlist[i + 1] - minlist[i];
		cout << si->selectivity[i] << " ";
	}
	cout << '\n';

	cout << "mean: ";
	si->mean = new float[data->info->numberColony];
	for (int i = 0; i < data->info->numberColony; i++) {
		int sum = 0;
		for (int j = minlist[i]; j < minlist[i + 1]; j++) {
			sum += j;
		}
		si->mean[i] = sum / si->selectivity[i];
		cout << (int) si->mean[i] << " ";
	}
	cout << '\n';
}

void SpiderSegmentation::initAgents() {
	cout << "init agent cpu" << endl;

	data->info->numberAgentPerColony = atoi(
			conf->infos["spider_agent_per_colony"].c_str());
//	data->aglist = (Spider*) malloc(data->info->numberAgentPerColony*data->info->numberColony * sizeof(Spider));
	data->aglist = new Spider[data->info->numberAgentPerColony
			* data->info->numberColony];
	int idx = 0;
	for (int i = 0; i < data->info->numberColony; i++) {
		for (int j = 0; j < data->info->numberAgentPerColony; j++) {
			data->aglist[idx].colony = i;
			Posision pos(rand() % env->x, rand() % env->y, rand() % env->z);
			data->aglist[idx].posision = pos;
			data->aglist[idx].lastposision = data->aglist[idx].posision;
			idx++;
		}
	}

}

void SpiderSegmentation::initDragline() {
	cout << "initDragline" << endl;

	SpiderInfo *si = (SpiderInfo*) data->info;

	si->nbDl = atoi(conf->infos["spider_nbDl"].c_str());
	cout << "number colony:" << data->info->numberColony << endl;
	clock_t t = clock();
	int size = env->x * env->y * env->z;

	si->dls = (int*) malloc(size * si->nbDl * sizeof(int));
	if (si->dls == NULL) {
		cout << "si->dls FAIL" << endl;
		exit(1);
	}
	fill_n(si->dls, size * si->nbDl, -1);

	si->nbSilkColony = (byte*) malloc(
			size * si->nbDl * data->info->numberColony * sizeof(byte));
	if (si->nbSilkColony == NULL) {
		cout << "si->nbSilkColony FAIL" << endl;
		exit(1);
	}
	fill_n(si->nbSilkColony, size * si->nbDl * data->info->numberColony, 0);

	cout << "Time consuming for initializing dragline is "
			<< (float) (clock() - t) / CLOCKS_PER_SEC << endl;
}

void SpiderSegmentation::initialize() {

	SpiderInfo *si = (SpiderInfo*) data->info;

	data->info->numberColony = atoi(
			conf->infos["spider_number_colony"].c_str());
	si->backprobability = atof(conf->infos["backprobability"].c_str());
	si->pAttractSelf = atof(conf->infos["pAttractSelf"].c_str());
	si->pAttractOther = atof(conf->infos["pAttractOther"].c_str());
	si->pSaturation = atof(conf->infos["pSaturation"].c_str());
	si->constantWeight = atof(conf->infos["constantWeight"].c_str());

	detectHistogram();
	initAgents();
	initDragline();
}

void SpiderSegmentation::runNStep() {
	cout << "simulate cpu" << endl;

	int numberIterator = atoi(conf->infos["spider_iterator"].c_str());
	int size = data->info->numberColony * data->info->numberAgentPerColony;
	int next = 0;
	SpiderInfo *si = (SpiderInfo*) data->info;
	si->neigh = new int[26];
	si->scut = new int[si->nbDl];
	for (int i = 0; i < numberIterator; i++) {
		if (i % 100 == 0)
			cout << "time spider: " << i << endl;
		for (int j = 0; j < size; j++) {
			data->aglist[j].perception(env, data->info);
			data->aglist[j].decision(env, data->info, next);
			data->aglist[j].action(env, data->info, next);
		}
	}

}

void SpiderSegmentation::exportImage(Environment *_result) {

	SpiderInfo *si = (SpiderInfo*) data->info;

	int size = env->x * env->y * env->z;
	_result->voxels = (byte*) malloc(size * sizeof(byte));
	if (_result->voxels == NULL) {
		cout << "error memory" << endl;
		return;
	}
	_result->x = env->x;
	_result->y = env->y;
	_result->z = env->z;

	int tmp;
	int countDl = 0;
	int max = 0, idm = 0;
	bool isHaveDl = false;
	unsigned short dlColony[data->info->numberColony];

	int *notVisit = (int*) malloc(size * sizeof(int));
	int idxNotVisit = 0;

	ofstream logfile;
	logfile.open("log.txt");
	if (!logfile) {
		cerr << "Cannot save the files statistic for spider algorithm" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++) {
		_result->voxels[i] = 0;
		//_result->voxels[i] = 255;
		fill_n(dlColony, data->info->numberColony, 0);
		isHaveDl = false;
		for (int j = 0; j < si->nbDl; j++) {
			tmp = i * si->nbDl + j;
//			if (si->nbSilk[tmp]==0) continue;
			int nbSilk = 0;
			for (int h = 0; h < data->info->numberColony; h++) {
				nbSilk += si->nbSilkColony[tmp * data->info->numberColony + h];
			}
			if (nbSilk == 0)
				continue;
			isHaveDl = true;
			for (int k = 0; k < data->info->numberColony; ++k) {
				dlColony[k] += si->nbSilkColony[tmp * data->info->numberColony
						+ k];
			}
		}
		if (isHaveDl) {
			countDl++;
			max = dlColony[0];
			idm = 0;
			for (int k = 1; k < data->info->numberColony; k++) {
				if (dlColony[k] > max) {
					max = dlColony[k];
					idm = k;
				}
			}
			_result->voxels[i] = si->mean[idm];
		} else {
			notVisit[idxNotVisit++] = i;
		}

		logfile << i << "[";
		for (int j = 0; j < data->info->numberColony; j++) {
			logfile << dlColony[j] << ",";
		}
		logfile << "]" << endl;
	}

	logfile.close();

	cout << "number draglines:" << countDl << endl;
	cout << "size:" << size << endl;
	cout << "number not visit:" << idxNotVisit << endl;

	if (conf->infos["postprocess"] == "on") {
		postProcess(idxNotVisit, notVisit, _result);
	}

	free(notVisit);
}

void SpiderSegmentation::postProcess(int idxNotVisit, int *notVisit,
		Environment *_result) {
	cout << "post processing" << endl;

	int index, px, py, pz, tmp, max;
	int *neigh = new int[26];
	;
	int sizeNeigh = 0;
	for (int i = 0; i < idxNotVisit; i++) {
		if (i % 1000 == 0)
			cout << i << endl;
		sizeNeigh = 0;
		index = notVisit[i];
		pz = index / (env->x * env->y);
		py = index % (env->x * env->y) / env->x;
		px = index % (env->x * env->y) % env->x;
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
					tmp = _result->getIdx(m + px, n + py, p + pz);
					if (_result->voxels[tmp] == 0)
						continue;
					neigh[sizeNeigh++] = tmp;
				}
			}
		}
		//		file << "("<< px<<","<<py <<","<< pz<<")" << sizeNeigh << ":" ;
		if (sizeNeigh < 13) {
			//			file << endl;
			continue;
		}
		map<int, int> mapp;
		int key = 0;
		//mapp[0]= 0;
		//		for (int k=1;k<data->info->numberColony;k++){
		//			mapp[(int)si->mean[idm]]= 0;
		//		}
		for (int j = 0; j < sizeNeigh; j++) {
			key = (int) _result->voxels[neigh[j]];
			//			file << key << ",";
			//			if (key!=0){
			mapp[key]++;
			//			}
		}
		max = -1;
		byte vv = 0;
		for (map<int, int>::iterator ii = mapp.begin(); ii != mapp.end();
				++ii) {
			if (ii->second > max) {
				max = ii->second;
				vv = ii->first;
			}
		}
		_result->voxels[index] = vv;
		//		file << "->" << (int)vv << endl;
	}
	//	file.close();

}

