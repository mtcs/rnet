/*
 * Header file for execution configuration reading
 * Copyright (C) 2013 Matheus Caldas Santos
 * This file is part of RNet 0.3

 * RNet 0.3 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * RNet 0.3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>
#include <getopt.h>

#define PROGNAME "rnet"

using namespace std;

enum Distribution{
	POWERLAW,
	NORMAL
};

void printLicense();

class Config {
	public:

		int numNodes; // Number of nodes
		bool inlineOutput;
		string comAssignFile;
		bool verbose;
		bool outputAdjList;

		// Linking probabilities 
		float comPerc; // Community linking probability
		float rand; // Random out connection probability

		Distribution outDist;
		Distribution inDist;
		Distribution comDist;

		// Power Law
		float outP; // Out Degree Power
		float inP; // In Degree Power
		float comP; // Community size Power

		int outMax;
		int inMax;
		int comMax;

		// Normal
		int outMean;
		int inMean;
		int comMean;

		float outSD;
		float inSD;
		float comSD;

		Config(int argc, char ** argv);
		Config();

		void help();

	private :

		Distribution parseDist(char *, float & pow, int &max, int & mean, float & sd);

};

#endif
