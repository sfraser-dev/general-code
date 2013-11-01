#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv) {
	//char *inputname="nhk.uyvy.d1.720x486.yuv";
	//char *inputname="girls_1920x1080_1378frames.uyvy";
	//char *inputname="/home/stewart/Videos/claire_176x144_i420_10frms.yuv";
	//char *inputname="/home/stewart/Videos/gengrids_720x480_uyvy_615frms.yuv";
	//char *inputname="/home/stewart/Videos/gengrids_720x486_uyvy_615frms.yuv";
	//char *inputname="/home/stewart/Videos/gengrids_1280x720_i420_615frms.yuv";
	char *inputname="/home/stewart/Videos/gengrids_1280x720_uyvy_615frms.yuv";
	//char *inputname="/home/stewart/Videos/gengrids_1920x1080_i420_615frms.yuv";
	char *outputnameBase="gengrids_1280x720p";
	//int pels=176, lines=144;
	//int pels=720, lines=480;
	//int pels=720, lines=486;
	int pels=1280, lines=720;
	//int pels=1920, lines=1080;
	int frameSize=pels*lines*2; 	// size of a single frame
	int noOfFrames=3000;
	FILE *fin, *fout;
	unsigned char *in_arr=NULL;
	unsigned char *out_arr=NULL;
	char outputname[80];
	int status;
	int i;
	char* prefix=NULL;

	in_arr = new unsigned char [frameSize];
	out_arr = new unsigned char [frameSize];

	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);

	for (i=0; i<noOfFrames; i++) {
		// read a frame from the input file
		if ((status = fread(in_arr, 1, frameSize, fin)) != frameSize){
	  		fclose(fin);
			return 0;
		}
	        
	        // generate appropriate name and prefix
		if (i < 10){
			prefix="000";
		}
		else if (i < 100){
			prefix="00";
		}
		else if (i < 1000){
			prefix="0";
		}
		else {
			prefix="";
		}
		sprintf(outputname,"%s.%s%d.yuv",outputnameBase,prefix,i);
		printf("%d %s\n",i,outputname);

		// copy the frame read (in_arr) to out_arr	
		memcpy(out_arr, in_arr, frameSize);

		// open output file
		if ((fout = fopen(outputname, "wb")) == NULL){
			printf("Error in opening file %s!\n", outputname);
			exit(-1);
		}

		// write output file
		if ((status = fwrite(out_arr, 1, frameSize, fout)) != frameSize){
			printf("Error in writing to file %s!\n", outputname);
			exit(-1);
		}	
		fclose(fout);
		fout=NULL;
	}
	fclose(fin);
	fin=NULL;

	return 1;
}


