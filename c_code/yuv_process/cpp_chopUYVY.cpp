#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv) {
	FILE *fin, *fout;
	unsigned char *input=NULL, *output=NULL;;
	int pels=1920, lines=1035;
	int sizeIn=pels*lines*2; 	// size of input frame
	int sizeOut=1920*1024*2;	// size of output frame, crop to 1024 rows/lines
	//char inputname[80];
	char *inputname="outUYVY.yuv";	// output from YCbCr->UYVY
	char *outputname="outChop.yuv";	
	int status;
	int frame;
	int startFrame=100;
	int noFrames=600;

	system("del outChop.yuv");

	input	= new unsigned char [sizeIn];
	output	= new unsigned char [sizeOut];

	// open input file
	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);

	// open output file
	if ((fout = fopen(outputname, "ab")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}

	for (frame=0; frame < noFrames; frame++){
		printf("frame=%d\n",frame);
		// read a full UYVY frame
		if ((status = fread(input, 1, sizeIn, fin)) != sizeIn){
			fclose(fin);
			return 0;
		}

		// start from frame "startFrame", ie: chop off first (0->startFrame-1) frames
		if (frame >= startFrame){	
			// copy 1st 1024 rows/lines ... cropping
			memcpy(output, input, sizeOut);

			// write cropped frame to output
			if ((status = fwrite(output, 1, sizeOut, fout)) != sizeOut){
				printf("Error in writing to file %s!\n", outputname);
				exit(-1);
			}	
		}
	}
	
	fclose(fin);
	fin=NULL;
	fclose(fout);
	fout=NULL;

	return 1;
}


