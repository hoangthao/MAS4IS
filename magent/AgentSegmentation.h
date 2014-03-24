/*
 * MAS.h
 *
 *  Created on: Oct 8, 2013
 *      Author: thao
 */

#ifndef AGENTSEGMENTATION_H_
#define AGENTSEGMENTATION_H_

#include "../core/Segmentation.h"
#include "../core/Environment.h"
#include "../core/Config.h"
#include "MAS.h"
//#include "Agent.h"
//#include "Info.h"

class AgentSegmentation: public Segmentation {
public:

//	Agent* aglist;
//	Info* info;
	MAS *data;

	AgentSegmentation(Environment *_env, Config* _conf):Segmentation(_env,_conf){
//		aglist=0;
//		info=0;
		data=new MAS();
	}
	~AgentSegmentation(){}
	inline void run(){
		runNStep();
	}
	virtual void exportImage(Environment *_result)=0;
	virtual void initialize()=0;
	virtual void runNStep()=0;
	virtual void initAgents()=0;

	virtual void statistic(Environment *result, Environment *gt, const char* fn)=0;
};


#endif /* AGENTSEGMENTATION_H_ */
