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
#include "AntSegmentation.h"
#include "Ant.h"
#include "AntInfo.h"
#include "../classic/Sobel.h"

void AntSegmentation::statistic(Environment *result, Environment *gt,
		const char* fn) {

	ofstream file;
	file.open(fn);
	if (!file) {
		cerr << "Cannot save the files statistics ant" << endl;
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

AntSegmentation::AntSegmentation(Config *_conf, Environment *_env) :
		AgentSegmentation(_env, _conf) {
	data->info = new AntInfo();
}

void AntSegmentation::initAgents() {
	cout << "init agent cpu" << endl;

	data->info->numberAgentPerColony = atoi(
			conf->infos["ant_agent_per_colony"].c_str());
	data->aglist = new Ant[data->info->numberAgentPerColony
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

void AntSegmentation::getSobel() {
	clock_t t;
	___clock(t);

	AntInfo *ai = (AntInfo*) data->info;
	int size = env->x * env->y * env->z;
	ai->sobel = (byte*) malloc(size * sizeof(byte));

	Sobel *m = new Sobel(env, conf);
	m->initialize();
	m->run();
	memcpy(ai->sobel, m->result, size);
	free(m);
	___clock(t, "Time compute sobel for ant is ");
}

void AntSegmentation::initialize() {

	AntInfo *ai = (AntInfo*) data->info;

	data->info->numberColony = atoi(conf->infos["ant_number_colony"].c_str());

	ai->highRanking = atof(conf->infos["high_ranking"].c_str());
	ai->evaporatePeromone = atof(conf->infos["evaporate_peromone"].c_str());
	ai->c1 = atof(conf->infos["c1"].c_str());
	ai->c2 = atof(conf->infos["c2"].c_str());
	ai->c3 = atof(conf->infos["c3"].c_str());
	ai->c4 = atof(conf->infos["c4"].c_str());

	int size = env->x * env->y * env->z;
	ai->pheronomes = (float*) malloc(size * sizeof(float));
	fill_n(ai->pheronomes, size, 0);
	initAgents();

	getSobel();

}

void AntSegmentation::runNStep() {

	cout << "simulate cpu" << endl;

	int size = data->info->numberColony * data->info->numberAgentPerColony;
	int next = 0;

	AntInfo *ai = (AntInfo*) data->info;
	ai->neigh = new int[26];

	int sizeEnv = env->x * env->y * env->z;
	ai->pheronomesAddmore = (float*) malloc(sizeEnv * sizeof(float));

	int numberIterator = atoi(conf->infos["ant_iterator"].c_str());
	for (int i = 0; i < numberIterator; i++) {
		cout << "time ant: " << i << endl;
		fill_n(ai->pheronomesAddmore, sizeEnv, 0);
		for (int j = 0; j < size; j++) {
			data->aglist[j].perception(env, data->info);
			data->aglist[j].decision(env, data->info, next);
			data->aglist[j].action(env, data->info, next);
		}
		for (int j = 0; j < sizeEnv; j++) {
			ai->pheronomes[j] += ai->pheronomesAddmore[j] / numberIterator;
			ai->pheronomes[j] = (1 - ai->evaporatePeromone) * ai->pheronomes[j];
		}
	}

}

void AntSegmentation::exportImage(Environment *_result) {

	AntInfo *ai = (AntInfo*) data->info;

	int size = env->x * env->y * env->z;

	_result->voxels = (byte*) malloc(size * sizeof(byte));
	if (_result->voxels == NULL) {
		cout << "error memory" << endl;
		return;
	}
	_result->x = env->x;
	_result->y = env->y;
	_result->z = env->z;
	int count = 0;

	float max = 0;
	float avg = 0;
	for (int i = 0; i < size; i++) {
		avg += ai->pheronomes[i];
		if (ai->pheronomes[i] > max) {
			max = ai->pheronomes[i];
		}
	}
	avg /= size;
	cout << "max:" << max << endl;
	cout << "avg:" << avg << endl;

	for (int i = 0; i < size; i++) {
		if (ai->pheronomes[i] > 0) {
			_result->voxels[i] = (ai->pheronomes[i] / max) * 255;
			count++;
		} else {
			_result->voxels[i] = 0;
		}
	}
	cout << "count pheronome: " << count << endl;

}

