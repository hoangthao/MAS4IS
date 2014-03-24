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
#include "Config.h"

istream& operator >>(istream& ins, map<string, string>& d) {
	string s, key, value;
	vector<string> x;
	// For each (key, value) pair in the file
	while (getline(ins, s)) {
		string::size_type begin = s.find_first_not_of(" \f\t\v");

		// Skip blank lines
		if (begin == string::npos)
			continue;

		// Skip commentary
		if (string("#;").find(s[begin]) != string::npos)
			continue;

		x = split(s, '=');

		// Insert the properly extracted (key, value) pair into the map
		d[x[0]] = x[1];
	}

	return ins;
}

bool Config::readFile(const char* fn) {
	ifstream f(fn);
	if (!f) {
		cerr << "Cannot open the file config" << fn << endl;
		return false;
	}
	f >> infos;
	f.close();
	return true;
}

