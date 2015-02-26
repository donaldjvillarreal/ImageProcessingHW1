
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

	t1 = atoi(argv[2]);
	t2 = atoi(argv[3]);

	if(t1 > 255 || t2 > 255 || t1 < -1 || t2 < -1) {
		cerr << "Values must be between -1 and 255\n";
		exit(1);
	}

	if(t1 > 0 && t2 > 0 && t1 >= t2) {
		cerr << "The first value must be less than the second value\n";
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

void
histo_stretch(imageP I1, int t1, int t2, imageP I2)
{
	int i, total, scale, Hmax, Hmin, G[MXGRAY], H[MXGRAY];
	uchar *in, *out;
	in  = I1->image;	// input  image buffer

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
	Hmin = 0;
	Hmax = MXGRAY;
	if(t1 < 0) {
		for(i=1; i<MXGRAY; i++) {
			if(H[i] > 0) {
				Hmin = i;
				break;
			}
		}
	}

	else Hmin = t1;

	if(t2 < 0) {
		for(i=MaxGray; i>0; i--) {
			if(H[i] > 0) {
				Hmax = i;
				break;
			}
		}
	}

	else Hmax = t2;

	// final lookup table
	for(i=0; i<MXGRAY; i++) {
		if(i<Hmin) G[i] =0;
		else if(i>Hmax) G[i] = 255;
		else G[i] = (MaxGray * (i - Hmin))/(Hmax - Hmin);
	}

	out = I2->image;	// output image buffer
	// iterate over all pixels
	for(i=0; i<total; i++)
		out[i] = G[ in[i] ];
}