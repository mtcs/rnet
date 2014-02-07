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
	cerr << "RNet Copyright (C) 2013 Matheus Caldas Santos\n";
	cerr << "This program comes with ABSOLUTELY NO WARRANTY.\n";
	cerr << "This is free software, and you are welcome to redistribute it\n";
	cerr << "under certain conditions; see <http://www.gnu.org/licenses/> for details.\n";
}

void Config::help(){
	cerr << "\n";
        cerr << "RNet " << RNET_VERSION_MAJOR << "." << RNET_VERSION_MINOR << " (" << CMAKE_BUILD_TYPE  << ")\n";
	cerr << "Compiled for " << CMAKE_SYSTEM_NAME << " by " << COMPILER << endl;
	cerr << "\n";
	logo();
	cerr << "\n";
	cerr << "\033[1mUsage: " << PROGNAME << " -n [# of nodes] <-v> ...\033[21m\n";
	cerr << "\n";
	cerr << "RNet is a realistic random network generator. It generates a social network from\n";
	cerr << "some statistical parameters. It deliberately assign communities to each node to \n";
	cerr << "achieve realistic social behavior.\n";
	cerr << "\n";
	cerr << "  \033[1mBASIC PARAMETERS\033[21m \n";
	cerr << "\n";
	cerr << "  -n --numnodes #              Number of nodes/vertices in the network. [REQUIRED]\n";
	cerr << "  -m --commlink #              Probability of a node to be connected to a node \n";
	cerr << "                               inside its community. (Default: 0.95)\n"; 
	cerr << "  -r --randomlink #            Probability of a node to be connected to a random \n";
	cerr << "                               link node. (Default: 0.05)\n";
	cerr << "  -a --commassign <FILE>       Output to a specified file the community assigned\n";
	cerr << "                               to each node. (Default: NULL)\n";
	cerr << "  -l --inlineOutput            Inline output doesn't save the graph in memmory \n";
	cerr << "                               before outputing edge list, so it saves memmory.\n";
	cerr << "                               (Default: False)\n";
	cerr << "  -d --adjlist                 Output in adjacency list format\n";
	cerr << "  -v --verbose                 Verbose output with progression and distributions.\n";
	cerr << "                               (Default: False)\n";
	cerr << "\n";
	cerr << "\n";
	cerr << "  \033[1mDISTRIBUTIONS PARAMETERS\033[21m - Distribution name followed by a comma \n";
	cerr << "  and its parameters also separated by commas. Ex.: -o normal,100,5.8 \n";
	cerr << "     ______________________________________________________________________\n";
	cerr << "    |Available Distributions | Distribution Parameters                     |\n";
	cerr << "    |----------------------------------------------------------------------|\n";
	cerr << "    |powerlaw                | power constant(float), maximum value(float) |\n";
	cerr << "    |normal                  | mean(integer), standard deviation(float)    |\n";
	cerr << "    |________________________|_____________________________________________|\n";
	cerr << "\n";
	cerr << "  -o --outdist	#       Outdegree distribution and arguments. \n";
	cerr << "                       (Default: powerlaw,2.10,n/(5*log(n))\n";
	cerr << "  -i --indist #        Indegree distribution and arguments. \n";
	cerr << "                       (Default: powerlaw,1.1,NULL)\n";
	cerr << "  -c --comdist #       Community size distribution and arguments. \n"; 
	cerr << "                       (Default: powerlaw,1.9,n/5)\n";
	cerr << "\n";
	cerr << "Mandatory or optional arguments to long options are also mandatory or optional\n";
	cerr << "for any corresponding short options.\n";
	cerr << "\n";
	cerr << "Project website: www.github.com/mtcs/rnet\n";
	//cerr << "Report bugs to matheus@...\n";
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
	outputAdjList = false;

		static struct option long_options[] = {
			{"numnodes",	required_argument, 	0,  'n' },
			{"randomlink",	required_argument, 	0,  'r' },
			{"commlink",	required_argument, 	0,  'm' },

			{"outdist",	required_argument, 	0,  'o' },
			{"indist",	required_argument, 	0,  'i' },
			{"comdist",	required_argument, 	0,  'c' },

			{"adjlist",	required_argument,     	0,  'd' },
			{"comassign",	required_argument,     	0,  'a' },
			{"inlineout",	no_argument,     	0,  'l' },
			{"verbose",	no_argument,       	0,  'v' }
		};

	int c;

	while (1) {
		int option_index = 0;

		c = getopt_long(argc, argv, "n:r:m:o:i:c:a:ldv",
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


			case 'd':
				outputAdjList = true;
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
