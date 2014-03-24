/*
 * misc.h
 *
 *  Created on: Oct 2, 2013
 *      Author: thao
 */

#ifndef MISC_H_
#define MISC_H_

#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h> //rand atoi=convert int, atof=convert float, atol=convert long
#include <string.h> //memcpy
#include <fstream> //ifstream
#include <sstream> //stringstream
#include <map>
#include <vector>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/opencl.h>
#include <CL/cl.hpp>


using namespace std;

typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned short ushort;

typedef struct _map{
    int key;
    float value;
} mapx;

float gaussian(int value, float mean, float varDis);
void generateRandomInt(int *rands, int size);
void generateRandomByte(byte *rands, int size);

void oclDeviceQuery();
string getCLresultMsg(cl_int error);
void checkError(cl_int err);

void ___clock(clock_t &t);
void ___clock(clock_t t,const char* info);

vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);



#endif /* MISC_H_ */
