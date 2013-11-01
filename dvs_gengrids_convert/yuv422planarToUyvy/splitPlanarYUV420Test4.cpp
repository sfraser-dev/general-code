#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv) {
        //char *inputname="cheer_ntsc.720x480.420planar.yuv"
        //char *inputname="cheer_uyvy.yuv";
        //char *inputname="nhk.uyvy.d1.720x486.yuv";
        //char *inputname="girls_1920x1080_1378frames.uyvy";
        //char *inputname="/home/stewart/Videos/claire_176x144_i420_10frms.yuv";
        //char *inputname="/home/stewart/Videos/gengrids_720x480_i420_615frms.yuv";
        //char *inputname="/home/stewart/Videos/gengrids_1280x720_i420_615frms.yuv";
        //char *inputname="/home/stewart/Videos/gengrids_1920x1080_i420_615frms.yuv";
	const char *inputname="/home/stewart/Videos/gengrids_720x480_planar422_615frms.yuv";
	const char *outputname="zzOut.yuv";
        //int pels=176, lines=144;
	int pels=720, lines=480;
        //int pels=1280, lines=720;
        //int pels=1920, lines=1080;
	int frameSize=pels*lines*2; 	// size of a single frame
	int maxNoOfFrames=500;
	FILE *fin, *fout;
	unsigned char *in_arr=NULL;
	unsigned char *out_arr=NULL;
	unsigned char **out=NULL;
	unsigned char *y=NULL;
	unsigned char *u=NULL;
	unsigned char *v=NULL;
	unsigned char *p=NULL;
	int status;
	int i, j, k;

	in_arr = new unsigned char [frameSize];
	out_arr = new unsigned char [frameSize];
	out = new unsigned char *[maxNoOfFrames];   // out[maxNoOfFrames][frameSize]
	for (i=0; i<maxNoOfFrames; i++) out[i] = new unsigned char [frameSize];

	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);

	for (i=0; i<maxNoOfFrames; i++) {
		// read a frame from the input file
		printf("reading frame %d\n",i);
		if ((status = fread(in_arr, 1, frameSize, fin)) != frameSize){
			fclose(fin);
			return 0;
		}

		// copy the frame read in_arr to out_arr
		// memcpy(out_arr, in_arr, frameSize);
		
		// point to the different colour components
		y = in_arr;
		u = y + (pels*lines);
		v = u + ((pels*lines)/2);
		p = out_arr;

		/*
		// change out_arr from planar422 to packed422 (yuy2)
      		for (j=0; j<lines; j++) {
			for (k=0; k<(pels/2); k++) {
				*p++ = *y++;
				*p++ = *u++;
				*p++ = *y++;
				*p++ = *v++;
			}
		}
		*/
		// change out_arr from planar422 to packed422 (uyvy)
      		for (j=0; j<lines; j++) {
			for (k=0; k<(pels/2); k++) {
				*p++ = *u++;
				*p++ = *y++;
				*p++ = *v++;
				*p++ = *y++;
			}
		}

		// copy the frame read (in_arr) to 2d array out[][]	
		memcpy(out[i], out_arr, frameSize);
	}
   

        // open file to write 2d array out to
	if ((fout = fopen(outputname, "wb")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}

        // write out 2d array
	for(i=0; i<maxNoOfFrames; i++) {
		printf("writing frame %i\n",i);
		if((status = fwrite(out[i], 1, frameSize, fout)) != frameSize){
			printf("Error in writing to file %s!\n", outputname);
			exit(-1);
		}	
	}

	// tidy up
	fclose(fin);
	fin=NULL;
	fclose(fout);
	fout=NULL;
	delete [] in_arr;
       	delete [] out_arr;
	for(i=0; i<maxNoOfFrames; i++) delete [] out[i];
	delete [] out;
	return 1;
}


