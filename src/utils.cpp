/*
 * Cpp source code file for miscellaneous functions and procedures
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



#include <sys/time.h>
#include <iostream>
#include <sstream>
#include "utils.h"


char input[1024];

void usage(char ** argv){
	fprintf(stderr, "Usage: %s -i input [-m method(s, b or [k])] [-a] name\n", argv[0]);
	fprintf(stderr, " Output = tsderror \n");
	exit(EXIT_FAILURE);
}

void parseArguments( int argc, char ** argv ){
	int inputOk = 0;

	int opt;

	while ((opt = getopt(argc, argv, "i:")) != -1) {
		switch (opt) {
			case 'i':
				strncpy( input, optarg, 1024);
				inputOk = 1;
				break;
			default: /* '?' */
				usage(argv);
		}
	}
	if(!inputOk){
		usage(argv);
	}

}

int colorFunc( int value ) {
	int ret =  231;

	if(value <= 0){
		ret = 240;

	}else if(value <= 5){
		ret = 16+(value);

	}else if(value <= 10){
		ret = 21+6*(value-5);

	}else if(value <= 15){
		ret = 51-(value-10);

	}else if(value <= 20){
		ret = 46+36*(value-15);

	}else if(value <= 25){
		ret = 226-6*(value-20);

	}else if(value <= 30){
		ret = 196+(value-25);

	}else if(value <= 35){
		ret = 201+6*(value-30);

	}

	return ret;
}

inline void printLine(int n, const char lineChar){
	for ( int i = 0; i < n; i++){
		fprintf(stderr, "%c", lineChar);
	}
}

inline void printLineLn(int n, const char lineChar){
	printLine(n, lineChar);
	fprintf(stderr, "\n");
}

void printMessage( int level, const char * inString,  ... ){

	#define LINESIZE 80
	va_list vArguments;
	static char * tmpString;
	static unsigned int size = LINESIZE;
	unsigned int n, pos = 0, color = 15;
	char lineChar;
	float paragraph;
	short important = 0;

	while( (inString[pos] == ' ') && (pos < strlen(inString)) ) pos++;
	if(inString[pos] == '!'){
		important = 1;
		pos++;
	}
	if(inString[pos]=='-'){
		pos++;
		lineChar = '-';
	}else{
		lineChar = ' ';
	}

	// Get full message string
	tmpString = (char *) realloc ( tmpString , size + 1);
	va_start( vArguments, inString);
	n = vsnprintf(tmpString, size, &inString[pos], vArguments );

	if ( ( n<0 ) || ( n > size ) ){
		tmpString = (char *) realloc ( tmpString , n + 1);
		n = vsnprintf(tmpString, n + 1, &inString[pos], vArguments );
	}
	va_end ( vArguments );


	// Setup centralization/paragraph
	switch( level ){
		case 0:
			color = 208;
			lineChar = '-';
			paragraph = ( LINESIZE - ( n + 2 ) )/(float)2;
			break;
		case 1:
			color = 208;
			lineChar = '-';
			paragraph = ( LINESIZE - ( n + 2 ) )/(float)2;
			break;
		case 2:
			color = 223;
			lineChar = '-';
			paragraph = level*2;
			break;
		default:
			paragraph = level*2;
			break;
	}
	if (important){
		color = 196;
	}

	// Start printing 
	// Box Head Line
	if (level<1) fprintf(stderr, "\n\033[0m");
	if (level<1)printLineLn(LINESIZE, lineChar);

	printLine(ceil(paragraph), lineChar);
	fprintf(stderr, " \033[38;5;%dm%s\033[0m ",color, tmpString);

	// Box Tail Line
	if (level<2)printLine(paragraph, lineChar);
	fprintf(stderr, "\n");

	// Box Tail Line
	if (level<1)printLineLn(LINESIZE, lineChar);
	if (level<1) fprintf(stderr, "\n");

	fflush(stderr);
}

double timeval_subtract(struct timeval *t2, struct timeval *t1){
	double diff = (t2->tv_sec + (double)t2->tv_usec / 1000000) - (t1->tv_sec + (double)t1->tv_usec / 1000000 );

	return (diff);
}

const char *  timeString(double time){
	static char buffer[128];
	long int itime = time;
	if (time < 60){
		sprintf(buffer, "%.1lfs", time);
	}else{
		if (time < 60*60){
			sprintf(buffer, "%2.0ldm %2.0lds", itime/60, itime % 60);
		}else{
			if (time < 24*60*60){
				sprintf(buffer, "%2.0ldh %2.0ldm", itime/60/60, (itime % (60*60))/60);
			}else{
				sprintf(buffer, "%.0ldd %2.0ldh", itime/24/60/60, (itime % (24*60*60))/60/60 );
			}
		}
	}
	return buffer;
}

struct kalman_state {
	double q; //process noise covariance
	double r; //measurement noise covariance
	double x; //value
	double p; //estimation error covariance
	double k; //kalman gain
};

struct kalman_state kState0;

struct kalman_state kalman_init(double q, double r, double p, double intial_value){
	kalman_state result;
	result.q = q;
	result.r = r;
	result.p = p;
	result.x = intial_value;

	return result;
}

void kalman_update(struct kalman_state* state, double measurement){
	//prediction update
	//omit x = x
	state->p = state->p + state->q;

	//measurement update
	state->k = state->p / (state->p + state->r);
	state->x = state->x + state->k * (measurement - state->x);
	state->p = (1 - state->k) * state->p;
}
	  
double smoothElapTime(double val){
	static int hPos = 0;
	static int started = 0;

	#ifdef OPENMP_FOUND
	const static int histLength = max (16, 2 * omp_get_num_threads());
	#else
	const static int histLength = 16;
	#endif

	static double * history = new double[histLength];

	if(started <= 1){
		for(int k = 0; k < histLength; ++k){
			history[k] = val;
		}
		started++;
	}

	history[hPos] = val;
	hPos = (hPos + 1 ) % histLength;

	val = 0 ;
	for(int k = 0; k < histLength; ++k){
		val += history[k];
	}
	val /= histLength;

	return val;
}

void progress(int max, int i, int rate){

	float percent = (i + 1) / (float) max ;
	char tmpString [128];
	char tmpString1 [128];
	char tmpString2 [128];
	static struct timeval time, timeOld;	
	static int started = 0;

	if( ((i+1) % ( max / min( max, rate ) )) == 0 ){ 
		gettimeofday(&time, NULL);
		double elapTime = timeval_subtract(&time, &timeOld);
		if ( started == 0 ) {
			elapTime = 0;
			started++;
		}

		elapTime = smoothElapTime(elapTime);

		double eta = elapTime * min( max, rate )  * (1 - percent);
		timeOld = time;

		// No color string
		snprintf(tmpString1, 128, "  %6d/%-6d  (%3.1f%%) ETA:%s     ",  i+1, max, 100*percent, timeString(eta));
		// String with color
		snprintf(tmpString, 128, "  %6d/%-6d  (\033[38;5;%dm%3.1f%%\033[0m) ETA:%s     ",  i+1, max, colorFunc(40*percent),100*percent, timeString(eta));
		unsigned int j;
		for (j = 0; j < strlen(tmpString1); ++j){
			snprintf(&tmpString2[j], 128, "\b"); 
		}
		snprintf(&tmpString2[j], 128-j, "%s", tmpString); 
		fprintf(stderr, "%s",tmpString2); 
		fflush(stderr);
	}
}

void asciiPlot(double * vals, int size, int height, int width){
	double maxVal = -999999999;
	double minVal = 999999999;
	size = min (size, width);
	for (int i = 0 ; i < size ; ++i){
		maxVal = max(maxVal, vals[i]);
		minVal = min(minVal, vals[i]);
	}
	//maxVal *= 1.01;
	//minVal *= 0.97;
	minVal = 0;

	int charsPerColumn = max (1, int((double)width / size));
	width = charsPerColumn * size;

	cerr << endl;

	for (int i = 0; i < height; ++i){
		int oldColor = 0;
		for (int j = 0; j < width; ++j){
			int column = j / charsPerColumn;
			double normVal = (vals[column] - minVal)/(maxVal - minVal);
			int color = colorFunc(4 + 21 * normVal);
			if (oldColor != color){
				cerr << "\033[38;5;" << color << "m";
				oldColor = color;
			}

			if( (column < size) &&  
					((height * normVal) >= (height - i)) ){
				// _,=#
				int rest = 4 * ( height * normVal - (height - i));
				switch (rest){
					case 0:
						if(normVal == 1)
							cerr << "#";
						else
							cerr << " ";
						break;
					case 1:
						cerr << "_";
						break;
					case 2:
						cerr << ",";
						break;
					case 3:
						cerr << "=";
						break;
					default:
						cerr << "#";
						break;
				}//  */
			}else{
				cerr << " ";
			}
		} 
		// Y axis labels 
		cerr << "\033[0;m";
		if ( i % 2 == 0){
			cerr << "| " <<  (maxVal - minVal) * ((double)height - i) / height;
		}else{
			cerr << "|" ;
		}

		cerr << endl;
	}
	cerr << "\033[0;m";
	
	// Horizontal line
	for (int j = 0; j < width; ++j){
		cerr << "_";
	}
	cerr << "| " << minVal << endl;

	/*for (int j = 0; j < size; ++j){
		cerr << (vals[j] - minVal)/(maxVal - minVal) << " ";
	}
	cerr << endl;// */

	// X axis labels
	for (int j = 0; j < width; ++j){
		int column = j / charsPerColumn;
		if( (column < size) && 
				( (j % charsPerColumn) == 0 ) ){
			ostringstream ss;
			//ss << " " << vals[column];
			//cerr << " " << vals[column];
			ss << column << " ";
			cerr << column << " ";
			j += ss.str().size() - 1;
		} else {
			cerr << " ";
		}
	}
	cerr << endl << endl;
}
