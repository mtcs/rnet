/*
 * Cpp source code file for execution configuration reading
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


#include <cstdlib>
#include <math.h>
#include <getopt.h>
#include <iostream>
#include <sstream>
#include <cstring>

#include "CMakeConfig.h"
#include "config.h"
#include "logo.h"

using namespace std;

void printLicense(){
	cerr << "RNet Copyright (C) 2013 Matheus Caldas Santos" << endl;
	cerr << "This program comes with ABSOLUTELY NO WARRANTY." << endl;
	cerr << "This is free software, and you are welcome to redistribute it" << endl;
	cerr << "under certain conditions; see <http://www.gnu.org/licenses/> for details." << endl;
}

void Config::help(){
	cerr << endl;
        cerr << "RNet " << RNET_VERSION_MAJOR << "." << RNET_VERSION_MINOR << " (" << CMAKE_BUILD_TYPE  << ")" << endl;
	cerr << "Compiled for " << CMAKE_SYSTEM_NAME << " by " << COMPILER << endl;
	cerr << endl;
	logo();
	cerr << endl;
	cerr << "\033[1mUsage: " << PROGNAME << " -n [# of nodes] <-v> ...\033[21m" << endl;
	cerr << endl;
	cerr << "RNet is a realistic random network generator. It generates a social network from" << endl;
	cerr << "some statistical parameters. It deliberately assign communities to each node to " << endl;
	cerr << "achieve realistic social behavior." << endl;
	cerr << endl;
	cerr << "  \033[1mBASIC PARAMETERS\033[21m " << endl;
	cerr << endl;
	cerr << "  -n --numnodes #              Number of nodes/vertices in the network. [REQUIRED]" << endl;
	cerr << "  -m --commlink #              Probability of a node to be connected to a node " << endl;
	cerr << "                               inside its community. (Default: 0.95)" << endl; 
	cerr << "  -r --randomlink #            Probability of a node to be connected to a random " << endl;
	cerr << "                               link node. (Default: 0.05)" << endl;
	cerr << "  -a --commassign <FILE>       Output to a specified file the community assigned" << endl;
	cerr << "                               to each node. (Default: NULL)" << endl;
	cerr << "  -l --inlineOutput            Inline output doesn't save the graph in memmory " << endl;
	cerr << "                               before outputing edge list, so it saves memmory." << endl;
	cerr << "                               (Default: False)" << endl;
	cerr << "  -v --verbose                 Verbose output with progression and distributions." << endl;
	cerr << "                               (Default: False)" << endl;
	cerr << endl;
	cerr << endl;
	cerr << "  \033[1mDISTRIBUTIONS PARAMETERS\033[21m - Distribution name followed by a comma " <<endl;
	cerr << "  and its parameters also separated by commas. Ex.: -o normal,100,5.8 " << endl;
	cerr << "     ______________________________________________________________________" << endl;
	cerr << "    |Available Distributions | Distribution Parameters                     |" << endl;
	cerr << "    |----------------------------------------------------------------------|" << endl;
	cerr << "    |powerlaw                | power constant(float), maximum value(float) |" << endl;
	cerr << "    |normal                  | mean(integer), standard deviation(float)    |" << endl;
	cerr << "    |________________________|_____________________________________________|" << endl;
	cerr << endl;
	cerr << "  -o --outdist	#       Outdegree distribution and arguments. " << endl;
	cerr << "                       (Default: powerlaw,2.10,n/(5*log(n))" << endl;
	cerr << "  -i --indist #        Indegree distribution and arguments. " << endl;
	cerr << "                       (Default: powerlaw,1.1,NULL)" << endl;
	cerr << "  -c --comdist #       Community size distribution and arguments. " << endl; 
	cerr << "                       (Default: powerlaw,1.9,n/5)" << endl;
	cerr << "" << endl;
	cerr << "Mandatory or optional arguments to long options are also mandatory or optional" << endl;
	cerr << "for any corresponding short options." << endl;
	cerr << endl;
	cerr << "Project website: www.github.com/mtcs/rnet" << endl;
	//cerr << "Report bugs to matheus@..." << endl;
}

Distribution Config::parseDist(char * distStr, float & pow, int & max, int & mean, float & sd){
	stringstream ss(distStr);

	string token;

	getline(ss, token, ',');


	if (! token.compare("normal")){
		if (! getline(ss, token, ',')){
			help();
			exit(1);
		}
		mean = atoi(token.data());

		if (! getline(ss, token, ',')){
			help();
			exit(1);
		}
		sd = atof(token.data());

		return NORMAL;
	}
	if (! token.compare("powerlaw")){

		if (! getline(ss, token, ',')){
			help();
			exit(1);
		}
		pow = atof(token.data());

		if (! getline(ss, token, ',')){
			help();
			exit(1);
		}
		max = atof(token.data());

		return POWERLAW;
	}
	// use powerlaw by default
	return POWERLAW;
}

Config::Config(int argc, char ** argv){

	outDist = POWERLAW;
	inDist = POWERLAW;
	comDist = POWERLAW;

	numNodes = 0; // Number of nodes
	comPerc = 0.95; // Community linking probability
	rand = 0.05; // Random out connection probability

	outP = 2.10; // Out Degree Power
	inP = 1.1; // In Degree Power
	comP = 1.8; // Community size Power

	outMax = 0;
	inMax = 0;
	comMax = 0;

	outMean = 0;
	inMean = 0;
	comMean = 0;

	comAssignFile = "";
	inlineOutput = false;
	verbose = false;

		static struct option long_options[] = {
			{"numnodes",	required_argument, 	0,  'n' },
			{"randomlink",	required_argument, 	0,  'r' },
			{"commlink",	required_argument, 	0,  'm' },

			{"outdist",	required_argument, 	0,  'o' },
			{"indist",	required_argument, 	0,  'i' },
			{"comdist",	required_argument, 	0,  'c' },

			{"comassign",	required_argument,     	0,  'a' },
			{"inlineout",	no_argument,     	0,  'l' },
			{"verbose",	no_argument,       	0,  'v' }
		};

	int c;

	while (1) {
		int option_index = 0;

		c = getopt_long(argc, argv, "n:r:m:o:i:c:a:lv",
				long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
			case 'n':
				numNodes = atoi(optarg);
				break;

			case 'r':
				rand = atof(optarg);
				break;

			case 'm':
				comPerc = atof(optarg);
				break;

			case 'o':
				outDist = parseDist(optarg, outP, outMax, outMean, outSD);
				break;

			case 'i':
				inDist = parseDist(optarg, inP, inMax, inMean, inSD);
				break;

			case 'c':
				comDist = parseDist(optarg, comP, comMax, comMean, comSD);
				break;


			case 'a':
				comAssignFile = optarg;
				break;

			case 'l':
				inlineOutput = true;
				break;

			case 'v':
				verbose = true;
				break;
			}
	}

	if ( numNodes <= 20 ){
		help();
		exit(1);
	}

	if (outMax == 0) outMax = numNodes / ( 5 * log(numNodes));
	if (comMax == 0) comMax = numNodes / 3;

	if ( outMean == 0 ) { outMean = outMax/2; outSD = outMean/5; }
	if ( inMean == 0 ) {inMean = outMax/2; inSD = inMean/5; }
	if ( comMean == 0 ) {comMean = numNodes/100; comSD = comMean/5; }

}

Config::Config(){
	Config(0, NULL);
}
