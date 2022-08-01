/*
temporalmemory.c
Temporal Memory Command Line

Copyright (c) 2022 Peter Overmann

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the “Software”), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/*

This is a command line wrapper for the elementary Temporal Memory algorithm published in
 https://github.com/PeterOvermann/Writings/blob/main/TriadicMemory.pdf

Temporal Memory processes a temporal stream of sparse distributed representations (SDRs),
at each step predicting the next step, based on items seen before.

This algorithm can be used to predict the next step in a continuous stream of items, or to
learn individual sequences terminated by a zero-value SDR.

This command line tool instantiates a new instance of a Temporal Memory.

An SDR is given by a set of p integers in the range from 1 to n.
Typical values are n = 1000 and p = 10 to 20.

Command line arguments: temporalmemory <n> <p>

Command line usage:
29 129 238 356 451 457 589 620 657 758

Print random SDR:
random

Print version number:
version

Terminate process:
quit

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#include "triadicmemory.h"

		
#define VERSIONMAJOR 1
#define VERSIONMINOR 1


int main(int argc, char *argv[])
	{
	char inputline[10000];
	
	if (argc != 3)
		{
		printf("usage: temporalmemory <n> <p>\n");
		printf("n is the hypervector dimension    (typical value 1000)\n");
		printf("p is the target sparse population (typical value 10 to 20)\n");
		exit(1);
		}
        
	int N, P;  // SDR dimension and target sparse population, received from command line

	sscanf( argv[1], "%d", &N);
	sscanf( argv[2], "%d", &P);
    
    	TemporalMemory *T = temporalmemory_new (N, P);
   
	SDR *inp = sdr_new(N);
	SDR *out = sdr_new(N);
	
	while (	fgets(inputline, sizeof(inputline), stdin) != NULL)
		{
		if (! strcmp(inputline, "quit\n"))
			exit(0);

		if (! strcmp(inputline, "random\n"))
			sdr_print(sdr_random(out, P));

		else if ( strcmp(inputline, "version\n") == 0)
			printf("temporalmemory %d.%d\n", VERSIONMAJOR, VERSIONMINOR);
			
		else // parse input SDR
			{
			if (* sdr_parse(inputline, inp) )
				{
				printf("unexpected input: %s", inputline);
				exit(5);
				}
			sdr_print( temporalmemory(T, inp));
			}
		}
			
	return 0;
	}
	
