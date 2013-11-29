/*
 * Header file for miscellaneous functions and procedures
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


#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <cstdarg>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string>
#include <string.h>
#include <math.h>

using namespace std;

void parseArguments( int argc, char ** argv );

void printMessage( int level, const char * inString,  ... );

int readGraphEdgeList(int *** edges, int ** numEdges, int *numNodes, FILE * inFile);

void progress(int max, int i, int rate);

void asciiPlot(double * vals, int size, int height, int width);

#endif
