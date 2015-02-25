// ================================================================
// histo_stretch.cpp - Histogram Stretching program.
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"

using namespace std;

int	histogramMatch(imageP I1, int n, imageP I2);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to apply histogram matching
//
int
main(int argc, char **argv)
{
	imageP I1, I2, histo;
	
	if(argc != 4) {
		cerr << "Usage: histo_match infile n outfile" << endl;
		exit(1);
	}

	I1 = IP_readImage(argv[1]);	// input image
	I2 = NEWIMAGE;			// make struct for output
	histogramMatch(I1, atoi(argv[2]), I2);	// histogram matching
	IP_saveImage(I2, argv[3]);	// save result to file
	IP_freeImage(I1);		// free up input  memory
	IP_freeImage(I2);		// free up output memory

	return 1;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histogramMatch:
//
// Apply histogram matching to I1 by remapping intensities in I1 to
// have histogram specified in histo. Output goes to I2.
//
int
histogramMatch(imageP I1, int n, imageP I2)
{
	int i, p, R;
	int left[MXGRAY], right[MXGRAY];
	int total, Hsum, Havg, h1[MXGRAY], h2[MXGRAY];
	unsigned char *in, *out;
	double scale;

	// total number of pixels in image
	total = (long) I1->height * I1->width;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (unsigned char *) malloc(total);

	in  = I1->image;		// input image buffer
	out = I2->image;		// output image buffer

	for(i=0; i<MXGRAY; i++) h1[i] = 0;	// clear histogram
	for(i=0; i<total; i++)  h1[in[i]]++;	// eval histogram

	// normalize h2 to conform with dimensions of I1
    
	if(n==0){
    	for(i=0; i<MXGRAY; i++){ h2[i] = Havg; }
	}
	else if(n<0){
		n = abs(n);
		for(i=0; i<MXGRAY; i++){ h2[MXGRAY-i] = ROUND(1.0 - pow((double)i/MaxGray, (double)n*MaxGray));} 
	}
	
	else{
		for(i=0; i<MXGRAY; i++){ h2[i] = ROUND(pow((double)i/MaxGray, (double)n*MaxGray)); }	
	}

	if(n != 0){
		for(i=Havg=0; i<MXGRAY; i++){ Havg += h2[i]; }
	}

	scale = (double) total / Havg;
	if(scale != 1) for(i=0; i<MXGRAY; i++) h2[i] *= scale;

	R = 0;
	Hsum = 0;
	// evaluate remapping of all input gray levels;
	// Each input gray value maps to an interval of valid
	// output values. The endpoints of the intervals are
	// left[] and right[] */
	for(i=0; i<MXGRAY; i++) {
		left[i] = R;		// left end of interval
		Hsum += h1[i];		// cum. interval value

		// make interval wider, if necessary
		while(Hsum>h2[R] && R<MXGRAY-1) {
			Hsum -= h2[R];		// adjust Hsum
			R++;			// update
		}

		// init right end of interval
		right[i] = R;
	}

	// clear h1 and reuse it below
	for(i=0; i<MXGRAY; i++) h1[i] = 0;

	// visit all input pixels
	for(i=0; i<total; i++) {
		p = left[in[i]];
		if(h1[p] < h2[p])	// mapping satisfies h2
			out[i] = p;
		else	out[i] = p = left[in[i]] = MIN(p+1, right[in[i]]);
		h1[p]++;
	}
}