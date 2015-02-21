
// ================================================================
// histo_stretch.cpp - Histogram Stretching program.
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void histo_stretch(imageP, int, int, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to create histogram stretch image.
//
int
main(int argc, char** argv)
{
	int	t1, t2;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 5) {
		cerr << "Usage: histo_stretch infile first_value second_value outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// t1 is lower of 2 inputs, while t2 is higher of 2 inputs
	if(atoi(argv[2]) > atoi(argv[3])) {
		t2 = atoi(argv[2]);
		t1 = atoi(argv[3]);
	}
	else {
		t1 = atoi(argv[2]);
		t2 = atoi(argv[3]);
	}
	if(t2 > 255) {
		cerr << "Highest input cannot exceed 255\n";
		exit(1);
	}

	// quantize image and save result in file
	histo_stretch(I1, t1, t2, I2);
	IP_saveImage(I2, argv[4]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histo_stretch:
//
// Quantize I1 into n uniform intervals and save the output in I2.
// 

void
histo_stretch(imageP I1, int t1, int t2, imageP I2)
{
	int i, total, scale, Hmax, Hmin;
	uchar *in, *out, G[MXGRAY], H[MXGRAY];
	in  = I1->image;	// input  image buffer
	out = I2->image;	// output image buffer

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "histo_stretch: Insufficient memory\n";
		exit(1);
	}

	// compute histogram
	for(i=0; i<MXGRAY; i++) H[i] = 0;	// clear histogram
	for(i=0; i<total;  i++) H[in[i]]++;	// eval  histogram

	// figure out the min and max of range for negative t1 and t2
	Hmax = 0;		Hmin = MXGRAY;
	if(t1 < 0 || t2 < 0) {
		for(i=0; i<MXGRAY; i++) {
			if(t1 < 0 && H[i] < Hmin)
				Hmin = H[i];
			if(t2 < 0 && H[i] > Hmax)
				Hmax = H[i];
		}
	}
	else {
		Hmin = t1;		Hmax = t2;
	}

	// Test to see if min and max works
	//cerr << Hmin << " - " << Hmax << "\n";

	// final lookup table
	for(i=0; i<MXGRAY; i++) {
		G[i] = ((MXGRAY-1) * (H[i] - Hmin))/(Hmax - Hmin);
		cerr << i << ": " << G[i] << "\n";
	}

	// iterate over all pixels
	for(i=0; i<total; i++)
		out[i] = G[ in[i] ];
}