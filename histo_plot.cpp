// ================================================================
// histo_plot.cpp - Histogram Plot program.
//
// Written by: Donald Villarreal
//	           Joseph Wagner
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void histo_plot(imageP, imageP, int flag);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine for histo_plot.
//
int
main(int argc, char** argv)
{
	int	flag;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 4) {
		cerr << "Usage: histo_plot infile outfile flag\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds

	flag = atoi(argv[3]);
    if(flag < 0 || flag > 1) {
        cerr << "Flag must be '0' or '1'";
        exit(1);
    }

	// create histo_plot image and save result in file
	histo_plot(I1, I2, flag);
	IP_saveImage(I2, argv[2]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// histo_plot:
//
// Plot histogram of image
// if flag then scale histogram otherwise truncate. I2 is histogram output.
//

void
histo_plot(imageP I1, imageP I2, int flag)
{
	int	total, i, j, width, height, Hmax, H[256];
	double scale, Havg;
	uchar *in, *out, *cursor;

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = MXGRAY;		I2->height = MXGRAY;		I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "histo_plot: Insufficient memory\n";
		exit(1);
	}
	// height/width/average
	width = I2->width;		height = I2->height;		Havg = total/256.0;

	// init lookup table
	for(i=0;i<MXGRAY;i++) H[i] = 0;	
		
	// visit all input pixels and apply lut to threshold
	in  = I1->image;		out = I2->image;		Hmax = 0;

	// Calculate Histogram
	for(i=0;i<total;i++) H[in[i]]++;
	for(i=0;i<MXGRAY; i++) {
		if(H[i] > Hmax) Hmax = H[i];
	}

	// Flag = 1, max point is placed at 255
	if(flag) scale = 255.0/Hmax;

	// Flag = 0, average of histogram is placed at 128
	else scale = 128.0/Havg;

	// Draw histogram plot
	for(i=0; i < width; i++) {
		H[i] = (int) (scale * H[i]);
		if(H[i] >= height) {
			for(j=0; j < height; j++) out[i+width*(width-j-1)] = MaxGray;
		}
		else {
			for(j=0; j<H[i]; j++) out[i+width*(width-j-1)] = MaxGray;
			for(   ; j<height; j++) out[i+width*(width-j-1)] = 0;
		}
	}
}