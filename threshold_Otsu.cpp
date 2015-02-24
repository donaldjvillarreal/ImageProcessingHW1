
// ================================================================
// threshold_Otsu.cpp - Quantization program.
//
// Written by: Donald Villarreal
//             Joseph Wagner
// =====================================================================

#include "IP.h"

using namespace std;

// function prototype
void threshold_Otsu(imageP, imageP);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main:
//
// Main routine to quantize image.
//
int
main(int argc, char** argv)
{
	int	levels;
	imageP	I1, I2;

	// error checking: proper usage
	if(argc != 3) {
		cerr << "Usage: threshold_Otsu infile levels outfile\n";
		exit(1);
	}

	// read input image (I1) and reserve space for output (I2)
	I1 = IP_readImage(argv[1]);
	I2 = NEWIMAGE;


	// quantize image and save result in file
	threshold_Otsu(I1, I2);
	IP_saveImage(I2, argv[2]);

	// free up image structures/memory
	IP_freeImage(I1);
	IP_freeImage(I2);

	return 1;
}


void threshold_Otsu(imageP I1, imageP I2){
	int i, total;
	uchar *in, *out, lut[MXGRAY];

	// total number of pixels in image
	total = I1->width * I1->height;

	// init I2 dimensions and buffer
	I2->width  = I1->width;
	I2->height = I1->height;
	I2->image  = (uchar *) malloc(total);
	if(I2->image == NULL) {
		cerr << "threshold_Otsu: Insufficient memory\n";
		exit(1);
	}
	int total = l1->width * height;

	// histogram
	float p[256]
	for(i=0; i<MXGRAY; i++) p[i] = 0;
	for(i=0; i<total; i++) p[in[i]]++;

	// normalize histogram; each entry represents prob of intensity in image
	for(i=0; i<MXGRAY; i++){
	 	P[i] /= total;

	// compute global sum of i*p[i]
	float sum = 0; 
	for(int i=0; i< MXGRAY;i++){
		sum += (i*p[i]);
	 	float sumB = 0;
	 	float pB = 0;
	 	float pF = 0;
	 	varMax = 0;
	 	threshold = 0;
	 	
	 	for(int t =0; t<MXGRAY; ++t){
	 		pB += p[t];
	 		if(pB == 0) continue; //prob of background
	 		
	 		pF = 1.0 - pB;
	 		if(pF == 0) break; // prob foreground

	 		sumB += (float) (t * p[t];

	 		float mB = sumB/pB; // BG mean
	 		float mF = (sum - sumB)/ pF //foreground mean

	 		//calculate between class variance
	 		float varBetween = pB*pF * (mB-mF) * (mB-mF);

	 		// check if new max found
	 		if(varBetween > varMax){
	 			varMax = varBetween;
	 			threshold = t;
	 		}
	 	}
	}
	cerr << "thresh" << threshold
	// init lookup table
	uchar lut[256];
	for(i=0; i<threshold; i++) lut[i] = 0;
	for(   ; i<256; ++i) lut[i] = 255;
	
	// visit all inputs and apply
	for(i=0;i<total;i++) out[i] = lut[in[i]];
}
// tries all possible thresholds and checks the difference and maximizes that difference
