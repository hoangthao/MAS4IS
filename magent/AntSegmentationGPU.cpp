/*
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

#include "AntSegmentationGPU.h"

void AntSegmentationGPU::initAgents() {
	cout << "init agent gpu" << endl;

	data->info->numberAgentPerColony = atoi(
			conf->infos["ant_agent_per_colony"].c_str());
	sp = (ant*) malloc(
			data->info->numberAgentPerColony * data->info->numberColony
					* sizeof(ant));
	int idx = 0;
	for (int i = 0; i < data->info->numberColony; i++) {
		for (int j = 0; j < data->info->numberAgentPerColony; j++) {
			sp[idx].colony = i;
			sp[idx].posision = env->getIdx(rand() % env->x, rand() % env->y,
					rand() % env->z);
			sp[idx].lastposision = sp[idx].posision;
			idx++;
		}
	}
}

void AntSegmentationGPU::runNStep() {
	cout << "simulate gpu" << endl;

	AntInfo *ai = (AntInfo*) data->info;

	int size = data->info->numberColony * data->info->numberAgentPerColony;
	byte *rands = (byte*) malloc(size * sizeof(byte));
	cl_int err = CL_SUCCESS;
	try {

		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		cl_context_properties properties[] = { CL_CONTEXT_PLATFORM,
				(cl_context_properties) (platforms[0])(), 0 };
		cl::Context context(CL_DEVICE_TYPE_GPU, properties);
		std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

		std::ifstream sourceFile("./magent/kernel_ant.cl");
		std::string sourceCode(std::istreambuf_iterator<char>(sourceFile),
				(std::istreambuf_iterator<char>()));
		cl::Program::Sources source(1,
				std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
		cl::Program program_ = cl::Program(context, source);
		program_.build(devices);
		cl::Kernel kernel(program_, "run", &err);

		size_t nbRandomSize = size * sizeof(byte);
		size_t nbAntSize = size * sizeof(ant);
		size_t nbPheronomesSize = env->x * env->y * env->z * sizeof(float);
		size_t nbPheronomesAddSize = env->x * env->y * env->z * sizeof(float);
		size_t nbVoxelsSize = env->x * env->y * env->z * sizeof(byte);

		cl::Buffer bufferA = cl::Buffer(context, CL_MEM_READ_ONLY,
				nbRandomSize);
		cl::Buffer bufferB = cl::Buffer(context, CL_MEM_READ_WRITE, nbAntSize);
		cl::Buffer bufferC = cl::Buffer(context, CL_MEM_READ_ONLY,
				nbPheronomesSize);
		cl::Buffer bufferD = cl::Buffer(context, CL_MEM_READ_WRITE,
				nbPheronomesAddSize);
		cl::Buffer bufferE = cl::Buffer(context, CL_MEM_READ_ONLY,
				nbVoxelsSize);

		cl::Buffer bufferF = cl::Buffer(context, CL_MEM_READ_ONLY,
				nbVoxelsSize);

		cl::CommandQueue queue(context, devices[0], 0, &err);

		float *pheronomesAdd = (float*) malloc(nbPheronomesAddSize);

		queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, nbAntSize, sp);
		queue.enqueueWriteBuffer(bufferE, CL_TRUE, 0, nbVoxelsSize,
				env->voxels);
		queue.enqueueWriteBuffer(bufferF, CL_TRUE, 0, nbVoxelsSize, ai->sobel);

		kernel.setArg(0, bufferB);
		kernel.setArg(3, size);
		antinfo infogpu;
		copy(ai, &infogpu);
		kernel.setArg(4, infogpu);
		kernel.setArg(6, bufferE);
		kernel.setArg(7, env->x);
		kernel.setArg(8, env->y);
		kernel.setArg(9, env->z);
		kernel.setArg(10, bufferF);

		cl::NDRange local(64);
		cl::NDRange global(ceil(size / (float) local[0]) * local[0]);
		clock_t t = clock();

		cl::Event event;
		int numberIterator = atoi(conf->infos["ant_iterator"].c_str());
		for (int i = 0; i < numberIterator; i++) {
			//cout << "time ant gpu " << i << endl;
			if (i % 100 == 0)
				cout << "time ant gpu: " << i << endl;
			generateRandomByte(rands, size);
			fill_n(pheronomesAdd, env->x * env->y * env->z, 0);
			queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, nbRandomSize, rands);
			queue.enqueueWriteBuffer(bufferC, CL_TRUE, 0, nbPheronomesSize,
					ai->pheronomes);
			queue.enqueueWriteBuffer(bufferD, CL_TRUE, 0, nbPheronomesAddSize,
					pheronomesAdd);
			kernel.setArg(1, bufferC);
			kernel.setArg(2, bufferD);
			kernel.setArg(5, bufferA);
			queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local,
					NULL, &event);
			event.wait();
			queue.enqueueReadBuffer(bufferD, CL_TRUE, 0, nbPheronomesAddSize,
					pheronomesAdd);
			for (int j = 0; j < env->x * env->y * env->z; j++) {
				ai->pheronomes[j] += pheronomesAdd[j] / numberIterator;
				ai->pheronomes[j] = (1 - ai->evaporatePeromone)
						* ai->pheronomes[j];
			}
		}

		cout << "Time consuming for computing kernel ant is "
				<< (float) (clock() - t) / CLOCKS_PER_SEC << endl;
	} catch (cl::Error &err) {
		cout << err.what() << "(" << getCLresultMsg(err.err()) << ")" << endl;
	}
	free(rands);
}
