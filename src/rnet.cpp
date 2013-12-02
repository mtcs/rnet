/*
 * Main cpp source code file for RNet
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


#ifdef OPENMP_FOUND
	#include <omp.h>
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>
#include <random>
#include <cstdlib> 

#include "utils.h"
#include "config.h"
#include "logo.h"


using namespace std;

double powerLaw(double y, double min, double max, double n){
	return pow(
			(pow(max + 1.0, n + 1.0) - pow(min, n + 1.0)) * y + pow(min, n + 1.0)
			,
			(1.0/(n + 1.0))
		  );
}

bool reverseSort(pair<int,int> a, pair<int,int> b){
	return (a.second > b.second);
}

void generateDegree(vector<int> &degree, vector<pair<int,int>> &degreeRank, int maxDegree, Config &conf){
	
	default_random_engine randGen(time(NULL));
	uniform_real_distribution<double> unifDist(0.0,1.0);
	normal_distribution<double> normalDist(float(conf.outMean), conf.outSD);
	unsigned int numNodes = degree.size();
	int numEdges = 0;

	for ( unsigned int i = 0; i < numNodes; ++i){
		switch (conf.outDist){
			case POWERLAW:
				degree[i] = 2 * unifDist(randGen) + powerLaw( unifDist(randGen),  1, maxDegree, -conf.outP);
			break;

			case NORMAL:
				degree[i] = 1 + int(normalDist(randGen));
			break;
		}
		//cerr << degree[i] << " ";

		numEdges += degree[i];

		degreeRank[i].first = i;
		degreeRank[i].second = degree[i];

	}
	printMessage(2, "%d Nodes %d Edges (degree-Mean: %.0lf Max:%d)", numNodes, numEdges, (double) numEdges/numNodes, maxDegree);
	sort(degreeRank.begin(), degreeRank.end(), reverseSort);
}

void generateComunities(vector< vector< pair<int,int> > > & comunities, vector<int> &commAssign, vector<int> &degree, Config &conf){
	
	unsigned int numNodes = degree.size();

	default_random_engine randGen(time(NULL));
	uniform_real_distribution<double> unifDist(0.0,1.0);
	binomial_distribution<unsigned int> binDist(numNodes, 0.01);
	normal_distribution<double> normalDist(float(conf.comMean), conf.comSD);

	unsigned int i = 0;
	unsigned int numCommunities = 0;
	vector<bool> nodeInserted(numNodes);

	for (unsigned int i = 0; i < numNodes; ++i){
		nodeInserted[i] = false;
	}

	while (i < numNodes){
		unsigned int newCommunitySize = 0;
		// Choose a new comunity size with a power law
		int k = 0;
		do{
			if ( k >= 10 )
				newCommunitySize = numNodes - i ;
			else {
				switch (conf.comDist){
					case POWERLAW:
						newCommunitySize = 1 + (2 * unifDist(randGen)) + powerLaw( unifDist(randGen),  1, numNodes, -conf.comP);
						//newCommunitySize =  powerLaw( unifDist(randGen),  1, numNodes, -conf.comP);
						break;

					case NORMAL:
						newCommunitySize = 1 + int(normalDist(randGen));
						break;
				//newCommunitySize = 1 + binDist(randGen);
				}
			}
			
			k++;

		}while( (i + newCommunitySize) > numNodes );
		//cerr << newCommunitySize << " ";

		if( (numCommunities + 1) > comunities.size() ){
			comunities.resize(1.2 * numCommunities);
		}
		
		//cerr << newCommunitySize << " ";
		comunities[numCommunities].resize(newCommunitySize);

		// insert nodes into a community
		for (unsigned int k = 0; k < newCommunitySize; ++k ){
			int index;
			do {
				index = numNodes * unifDist(randGen);
			}while(nodeInserted[index] == true);
			
			comunities[numCommunities][k].first = index;
			comunities[numCommunities][k].second = degree[index];
			commAssign[index] = numCommunities;

			nodeInserted[index] = true;
		}
		// Rank node per degree for preferential attachment
		sort(comunities[numCommunities].begin(), comunities[numCommunities].end(), reverseSort);
		

		numCommunities++;
		i += newCommunitySize;
	}
	comunities.resize(numCommunities);
}

void outputCommAssign(Config &conf, vector<int> &ca){

	if (conf.comAssignFile.size() > 0){
		ofstream caOut;

		caOut.open ( conf.comAssignFile,  std::ofstream::out);

		for ( unsigned int i = 0; i < ca.size(); ++i){
			caOut << i << " " << ca[i] << endl;
		}

		caOut.close();
	}
}

template <class T>
T vecMax(vector<T> vec){
	T maxVal = -9999999;
	for (unsigned int i = 0; i < vec.size(); ++i)
		maxVal = max(maxVal, vec[i]);
	return maxVal;
}

template <class T>
void plotVecPdf(vector<T> vec){

	//unsigned int size = vecMax<T>(vec) + 1;
	
	T size = 0;
	for (unsigned int i = 0; i < vec.size(); ++i){
		size = size > vec[i] ? size : vec[i];
	}
	size++;

	vector<double> pdf(size);

	for ( unsigned int i = 0; i < pdf.size(); ++i){
		pdf[i] = 0;
	} // */

	for ( unsigned int i = 0; i < vec.size(); ++i){
		pdf[ vec[i] ] ++ ;
	}

	for ( unsigned int i = 0; i < pdf.size(); ++i){
		pdf[i] = log10(pdf[i]);
	}// */

	asciiPlot(pdf.data(), pdf.size(), 11, 170);
}


int chooseRandomDest(
		unsigned int i, 
		list<int> * edgesP, 
		vector<bool> &nodeChosed,
		vector<int> &degree, 
		vector<pair<int,int>> &degreeRank, 
		vector< pair<int,int> > * commun, 
		float rand, float comPerc, 
		float inP){

	static default_random_engine randGen(time(NULL));
	static uniform_real_distribution<double> unifDist(0.0,1.0);

	unsigned int numNodes = degree.size();
	unsigned int index = 0;

	bool destRand = (unifDist(randGen) < rand ) ? true : false;
	do{
		if (destRand) {
			// Random Attachment
			index = numNodes * unifDist(randGen);
		}else{
			if (unifDist(randGen) < comPerc){
				// Preferential Attachment inside the edge source community
				index = powerLaw( unifDist(randGen),  1, commun->size(), -inP) - 1;
				index = (*commun)[index].first;
			}else{
				// Preferential Attachment
				index = powerLaw( unifDist(randGen),  1, numNodes, -inP) - 1;
				index = degreeRank[index].first;
			}
		}

	//}while( (edgesP->find( index ) != edgesP->end()) || (index == i) || (index >= numNodes) );
	}while( ( nodeChosed[ index ] ) || (index == i) || (index >= numNodes) );

	nodeChosed[index] = true;

	return index;
}

void generateNetwork(
		vector< list<int> > & adjList, 
		vector<int> &inDegree, 
		vector<int> &degree, 
		vector<pair<int,int>> &degreeRank, 
		vector< vector< pair<int,int> > > & comunities, 
		vector<int> &commAssign, 
		Config &config 
		){


	unsigned int numNodes = adjList.size();

	printMessage(3,"Community and random attachment");

	#pragma omp parallel 
	{

		default_random_engine randGen(time(NULL));
		uniform_real_distribution<double> unifDist(0.0,1.0);
		
		#pragma omp for schedule(static,128)
		for ( unsigned int i = 0; i < numNodes; ++i ){
			inDegree[i] = 0;
		}// */

		#pragma omp for schedule(dynamic, 128)
		for ( unsigned int i = 0; i < numNodes; ++i ){
			
			progress(numNodes, i, 100);
			vector<bool> nodeChosed(numNodes);

			for ( int k = 0; k < degree[i] ; ++k){
				unsigned int index = chooseRandomDest(i, &adjList[i], nodeChosed, degree, degreeRank, &comunities[commAssign[i]], config.rand, config.comPerc, config.inP);
				//cerr << i << " " << index << "   "; cerr.flush();

				if( config.inlineOutput ){
					// Update Adjacency list
					adjList[i].push_back(index);
				}else{
					// Or print inline
					#pragma omp critical
					{
						cout << i << " " << index << " " << endl;
					}
				}
				//#pragma omp atomic 
				//inDegree[index]++;
			}
		}// */
	}

	cerr << endl;
}

void printGraph(vector< list<int> > & adjList, int numNodes){

	for ( int i = 0; i < numNodes; ++i){
		list<int> * edgesP = &adjList[i];
		for ( list<int>::iterator it = edgesP->begin(); it != edgesP->end(); ++it ){
			cout << i << " " << *it << " " << endl;
		}
	}// */
}


int main (int argc, char ** argv){

	//int numNodes = argc > 1 ? atoi(argv[1]) : 1000; // Number of nodes
	//int maxDegree = numNodes / ( 5 * log(numNodes));  
	//float outP = argc > 2 ? atof(argv[2]) :  2.10; // Out Degree Power
	//float inP = argc > 3 ? atof(argv[3]) :  1.1; // In Degree Power
	//float comP = argc > 4 ? atof(argv[4]) :  1.9; // Community size Power
	//float comPerc = argc > 5 ? atof(argv[5]) :  0.95; // Community linking probability
	//float neighb = argc > 6 ? atof(argv[6]) : 0.00; // Neighbourhood linking probability
	//float rand = argc > 7 ? atof(argv[7]) : 0.05; // Random out connection probability

	printLicense();

	Config config(argc, argv);

	vector<int> degree (config.numNodes);
	vector<int> inDegree (config.numNodes);
	vector< list<int> > adjList(config.numNodes);
	vector<pair<int,int>> degreeRank(config.numNodes);
	vector< vector< pair<int,int> > > communities(config.numNodes);
	vector<int> commAssign(config.numNodes);

	logo();

	printMessage(1, "Generating Outdegree");
	generateDegree(degree, degreeRank, config.maxDegree, config);

	// Outdegree PDF Chart
	printMessage(2, "OutDegree PDF");
	plotVecPdf<int>(degree);

	printMessage(2, "Generating Communities");
	generateComunities(communities, commAssign, degree, config);

	outputCommAssign(config, commAssign);

	// Outdegree PDF Chart
	vector<int> commSize( communities.size() );
	int sum = 0;
	for (unsigned int i = 0; i < communities.size(); ++i){
		commSize[i] = communities[i].size();
		sum  += commSize[i];
	}// */
	printMessage(2, "Mean Comunity size: %.0lf - %d communities", (double)sum/communities.size(), communities.size());
	plotVecPdf<int>(commSize);

	printMessage(1, "Generating Network");
	generateNetwork(adjList, inDegree, degree, degreeRank, communities, commAssign, config);

	// InDegree PDF Chart
	//printMessage(2, "InDegree PDF");
	//plotVecPdf<int>(inDegree);


	if ( ! config.inlineOutput ){
		//Print edge list output
		printMessage(1, "Output");
		printGraph(adjList, config.numNodes);
	}

}
