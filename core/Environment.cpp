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

#include "misc.h"
#include "Environment.h"

bool Environment::createEnvironment(const char* fn, int dx, int dy, int dz,
		int gs) {

	x = dx;
	y = dy;
	z = dz;
	grayScale = gs;

	ifstream file;
	file.open(fn, ios::binary);
	if (!file) {
		cerr << "Cannot open the file image MRI 3D" << fn << endl;
		return false;
	}
	int length = x * y * z, idx = 0, size = sizeof(byte);
	voxels = new byte[length];
	unsigned char c;
//	if (from!=0)
//		file.seekg(from*dx*dy,ios::beg);

	while (idx < length) {
		file.read((char*) &c, size);
		voxels[idx++] = c;
	}

	file.close();

	return true;
}

bool Environment::writeSlice(const char* fn, int slice) {
	ofstream file;
	file.open(fn);
	if (!file) {
		cerr << "Cannot save the files image MRI 3D" << fn << endl;
		return false;
	}
	file << "P2\n";
	file << x << "  " << y << "\n";
	file << 255 << "\n";
	int begin = slice * x * y;
	int tmp;
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			tmp = voxels[begin++];
			file << tmp << " ";
//			cout << tmp << " ";
		}
		file << "\n";
//		cout << "\n";
	}
	file.close();
	return true;
}

bool Environment::writeRawb(const char* fn) {
	ofstream file;
	file.open(fn, ios::binary);
	if (!file) {
		cerr << "Cannot save the files image MRI 3D" << fn << endl;
		return false;
	}
	int tmp;
	int begin = 0;
	for (int k = 0; k < z; k++) {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				tmp = voxels[begin];
				file.write((char*) &tmp, sizeof(char));
				begin++;
			}
		}
	}
	file.close();
	return true;
}

void Environment::tostring(int idx, const char* cc) {
	int pz = idx / (x * y);
	int py = idx % (x * y) / x;
	int px = idx % (x * y) % x;
	cout << "(" << px << "," << py << "," << pz << ":" << (int) voxels[idx]
			<< ")" << cc;
}

void Environment::copyEnvironment(Environment *c, int start, int nbSlice) {
	c->x = x;
	c->y = y;
	c->z = nbSlice;
	c->grayScale = grayScale;

	int index = x * y * start;
	c->voxels = (byte*) malloc(x * y * nbSlice * sizeof(byte));
	int idx = 0;
	for (int k = 0; k < nbSlice; k++)
		for (int j = 0; j < y; j++)
			for (int i = 0; i < x; i++)
				c->voxels[idx++] = voxels[index++];

}

void Environment::computeHistogram() {
	histogram = new int[grayScale];
	std::fill_n(histogram, grayScale, 0);

	for (int i = 0; i < x * y * z; i++) {
		histogram[voxels[i]]++;
	}
}
void Environment::writeHistogram() {
	cout << "histogram :" << endl;
	for (int i = 0; i < grayScale; i++) {
		if (histogram[i] > 0)
			cout << i << ":" << histogram[i] << endl;
	}
	cout << "end histogram :" << endl;
}

void Environment::getNeighbors(int idx, int *nei, int &size) {
	size = 0;
	int pz = idx / (x * y);
	int py = idx % (x * y) / x;
	int px = idx % (x * y) % x;
	for (int p = -1; p <= 1; p++) {
		if (p + pz < 0 || p + pz >= z)
			continue;
		for (int n = -1; n <= 1; n++) {
			if (n + py < 0 || n + py >= y)
				continue;
			for (int m = -1; m <= 1; m++) {
				if (m + px < 0 || m + px >= x)
					continue;
				if (m == n && n == p && p == 0)
					continue;
				nei[size++] = getIdx(m + px, n + py, p + pz);
			}
		}
	}
}

