
// ================================================================
// thr.c - Threshold program.
// Copyright (C) 2012 by George Wolberg
//
// Written by: George Wolberg, 2012
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void qntz(imageP, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to threshold image.
//
int
main(int argc, char** argv)
{
	int	n;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 4) {
		cerr << "Usage: qntz infile n outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	n = atoi(argv[2]);

	// threshold image and save result in file
	qntz(I1, n, I2);
	IP_saveImage(I2, argv[3]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// qntz:
//
// Quantize I1 into n uniform intervals and save the output in I2.
// The following rule is used for thresholding:
// 

void
qntz(imageP I1, int n, imageP I2)
{
	int MAXGRAY = 256;
	int	 intervalSize, total, currInterval, i;
	uchar *in, *out, lut[256];

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "qntz: Insufficient memory\n";
		exit(1);
	}

	/*intervalSize = MAXGRAY/n;
	currInterval = intervalSize;
	i = 0;

	// init lookup table
	
	while(currInterval <= 256){
		for(;i<=currInterval; i++){ lut[i] = currInterval-intervalSize/2; cerr << "grey value: "<< i << " =" << (int)lut[i] << endl;} 
			currInterval += intervalSize; 
	}
	*/

	int scale = MAXGRAY/n;
	for(i=0;i<MAXGRAY;i++){
		lut[i] = scale * (int) i/scale;
	}
	

	// visit all input pixels and apply lut to threshold
	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer
	for(i=0; i<total; i++) out[i] = lut[ in[i] ];
}

