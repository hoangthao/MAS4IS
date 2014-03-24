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
#include "SpiderSegmentationGPU.h"

void SpiderSegmentationGPU::initAgents() {

	cout << "init agent gpu" << endl;
	data->info->numberAgentPerColony = atoi(
			conf->infos["spider_agent_per_colony"].c_str());
	sp = (spider*) malloc(
			data->info->numberAgentPerColony * data->info->numberColony
					* sizeof(spider));
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

void SpiderSegmentationGPU::runNStep() {
	cout << "simulate gpu" << endl;

	SpiderInfo *si = (SpiderInfo*) data->info;

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

		std::ifstream sourceFile("./magent/kernel_spider.cl");
		std::string sourceCode(std::istreambuf_iterator<char>(sourceFile),
				(std::istreambuf_iterator<char>()));
		cl::Program::Sources source(1,
				std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
		cl::Program program_ = cl::Program(context, source);
		program_.build(devices);
		cl::Kernel kernel(program_, "run2", &err);

		size_t nbRandomSize = size * sizeof(byte);
		size_t nbSpiderSize = size * sizeof(spider);
		size_t nbDraglineSize = env->x * env->y * env->z * si->nbDl
				* sizeof(int);
//		size_t nbDraglineSilkSize = env->x*env->y*env->z *si->nbDl* sizeof(ushort);
		size_t nbDraglineSilkColonySize = env->x * env->y * env->z * si->nbDl
				* data->info->numberColony * sizeof(byte);

		size_t nbMeansSize = data->info->numberColony * sizeof(float);
		size_t nbSelectSize = data->info->numberColony * sizeof(float);
		size_t nbVoxelsSize = env->x * env->y * env->z * sizeof(byte);

		cl::Buffer bufferA = cl::Buffer(context, CL_MEM_READ_ONLY,
				nbRandomSize);
		cl::Buffer bufferB = cl::Buffer(context, CL_MEM_READ_WRITE,
				nbSpiderSize);
		cl::Buffer bufferC = cl::Buffer(context, CL_MEM_READ_WRITE,
				nbDraglineSize);
//		cl::Buffer bufferD = cl::Buffer(context, CL_MEM_READ_WRITE, nbDraglineSilkSize);
		cl::Buffer bufferE = cl::Buffer(context, CL_MEM_READ_WRITE,
				nbDraglineSilkColonySize);

		cl::Buffer bufferX = cl::Buffer(context, CL_MEM_READ_ONLY, nbMeansSize);
		cl::Buffer bufferY = cl::Buffer(context, CL_MEM_READ_ONLY,
				nbSelectSize);
		cl::Buffer bufferZ = cl::Buffer(context, CL_MEM_READ_ONLY,
				nbVoxelsSize);

		cl::CommandQueue queue(context, devices[0], 0, &err);

		queue.enqueueWriteBuffer(bufferB, CL_TRUE, 0, nbSpiderSize, sp);
		queue.enqueueWriteBuffer(bufferC, CL_TRUE, 0, nbDraglineSize, si->dls);
//		queue.enqueueWriteBuffer(bufferD, CL_TRUE, 0, nbDraglineSilkSize, si->nbSilk);
		queue.enqueueWriteBuffer(bufferE, CL_TRUE, 0, nbDraglineSilkColonySize,
				si->nbSilkColony);

		queue.enqueueWriteBuffer(bufferX, CL_TRUE, 0, nbMeansSize, si->mean);
		queue.enqueueWriteBuffer(bufferY, CL_TRUE, 0, nbSelectSize,
				si->selectivity);
		queue.enqueueWriteBuffer(bufferZ, CL_TRUE, 0, nbVoxelsSize,
				env->voxels);
		kernel.setArg(0, bufferB);
		kernel.setArg(1, bufferC);
//		kernel.setArg(2, bufferD);
		kernel.setArg(2, bufferE);
		kernel.setArg(3, size);
		kernel.setArg(4, si->nbDl);
		spiderinfo infogpu;
		copy(si, &infogpu);
		kernel.setArg(5, infogpu);
		kernel.setArg(6, env->x);
		kernel.setArg(7, env->y);
		kernel.setArg(8, env->z);

		kernel.setArg(10, bufferX);
		kernel.setArg(11, bufferY);
		kernel.setArg(12, bufferZ);
		cl::NDRange local(64);
		cl::NDRange global(ceil(size / (float) local[0]) * local[0]);
		clock_t t = clock();

		cl::Event event;
		int numberIterator = atoi(conf->infos["spider_iterator"].c_str());
		for (int i = 0; i < numberIterator; i++) {
			if (i % 100 == 0)
				cout << "time spider gpu: " << i << endl;
			generateRandomByte(rands, size);
			queue.enqueueWriteBuffer(bufferA, CL_TRUE, 0, nbRandomSize, rands);
			kernel.setArg(9, bufferA);
			queue.enqueueNDRangeKernel(kernel, cl::NullRange, global, local,
					NULL, &event);
			event.wait();
		}
		queue.enqueueReadBuffer(bufferB, CL_TRUE, 0, nbSpiderSize, sp);
		queue.enqueueReadBuffer(bufferC, CL_TRUE, 0, nbDraglineSize, si->dls);
//		queue.enqueueReadBuffer(bufferD, CL_TRUE, 0, nbDraglineSilkSize, si->nbSilk);
		queue.enqueueReadBuffer(bufferE, CL_TRUE, 0, nbDraglineSilkColonySize,
				si->nbSilkColony);
		cout << "Time consuming for computing kernel spider is "
				<< (float) (clock() - t) / CLOCKS_PER_SEC << endl;
	} catch (cl::Error &err) {
		cout << err.what() << "(" << getCLresultMsg(err.err()) << ")" << endl;
	}
	free(rands);

}
