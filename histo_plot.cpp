// ================================================================
// thr.c - Threshold program.
// Copyright (C) 2012 by George Wolberg
//
// Written by: George Wolberg, 2012
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void histo_plot(imageP, imageP, int flag);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to threshold image.
//
int
main(int argc, char** argv)
{
	int	flag;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 4) {
		cerr << "histo_plot: histo_plot infile outfile flag\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;

	// read lower and upper thresholds
	flag = atoi(argv[3]);

	// threshold image and save result in file
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
//// 

void
histo_plot(imageP I1, imageP I2, int flag)
{
	int	MAXGRAY = 256, total,i, j, width, height;
	uchar *in, *out, *cursor, H[256];

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = MAXGRAY;
	I2->height = MAXGRAY;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "histo_plot: Insufficient memory\n";
		exit(1);
	}
	width = I2->width;
	height = I2->height;

	// init lookup table
	for(i=0;i<MAXGRAY;i++) H[i] = 0;	
		
	// visit all input pixels and apply lut to threshold
	in  = I1->image;	// input  image buffer	
	out = I2->image;	// output image buffer
	
	for(i=0;i<total;i++) H[in[i]]++;
/* Flag handling goes here. Good sKeleton but not correct scaling method.
	if(flag){
		int max = H[0];
		for(i=0;i<MAXGRAY;++i){
			if(H[i]>max)
				max=H[i];
		}
		int scale = max - height;
		for(i=0;i<MAXGRAY;++i){
			if(H[i]>scale)
				H[i]=H[i]-scale;
			else
				H[i]=0;
		}
	}
*/	
	uchar *origin = out + (height-1) * width;
	cursor = origin;
	for(j=0; j < width;){
	cerr << (int)H[j] << endl;	
		for(i=0; i < H[j];){			
			*cursor = 200;
			cursor -= width;
			i++;
		}
		j++;
		cursor = origin;
		cursor += j;
	}
}