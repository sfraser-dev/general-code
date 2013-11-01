#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv) {
	FILE *fin, *fout;
	unsigned char *inputY=NULL, *inputB=NULL, *inputR=NULL, *output=NULL;;
	int pelsY=1920, linesY=1035;
	int sizeY=pelsY*linesY;
	int pelsChroma=960, linesChroma=1035;
	int sizeChroma=pelsChroma*linesChroma;
	int sizeOut=pelsY*linesY*2;
	//char inputname[80];
	char *inputname;
	char *outputname="outCpp.yuv";
	int status;
	int i;
	int y, b, r;
	int marker;
	int format=2;	// 1=yuy2  2=uyvy
	int chopSize;

	inputY	= new unsigned char [sizeY];
	inputB	= new unsigned char [sizeChroma];
	inputR	= new unsigned char [sizeChroma];
	output	= new unsigned char [sizeOut];

	// process the arguments
	/*
	for (i = 1; i < argc; i++){
	        if (*(argv[i]) == '-'){
           	    	switch (*(++argv[i])){
                		case 'n':
                    			maxFrameNumber = atoi(argv[++i]);
                    			break;
				case 'o':
                    			strcpy(outputname, argv[++i]);
                    			break;
                		default:
                		    	printf("Illegal option %c!\n", *argv[i]);
					printf("Usage: concat2.exe -n <noOfFrames> -o <output planar YUV 420 file>\n");
    					printf("Concatenates <noOfFrames-1> frames of\n\n");
					printf("Motion_Graphics.000_planar420.yuv\n"); 
					printf("Motion_Graphics.001_planar420.yuv\n"); 
					printf("Motion_Graphics.002_planar420.yuv\n"); 
					printf(".\n");
					printf(".\n");
					printf(".\n");
					printf("Motion_Graphics.<noOfFrames-1>_planar420.yuv\n"); 
					printf("\ninto a single planar YUV 420 file.\n\n");
				    	return -1;
            	    	}
        	}
    	}
	*/


	// read in Y	
	inputname="w.yyy";
	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);
	if ((status = fread(inputY, 1, sizeY, fin)) != sizeY){
		fclose(fin);
		return 0;
	}
	fclose(fin);
	fin=NULL;

	
	// read in B	
	inputname="w.bbb";
	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);
	if ((status = fread(inputB, 1, sizeChroma, fin)) != sizeChroma){
		fclose(fin);
		return 0;
	}
	fclose(fin);
	fin=NULL;
	

	// read in R	
	inputname="w.rrr";
	if ((fin=fopen(inputname,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputname);
		exit(-1);
	}
	rewind(fin);
	if ((status = fread(inputR, 1, sizeChroma, fin)) != sizeChroma){
		fclose(fin);
		return 0;
	}
	fclose(fin);
	fin=NULL;

	// reaarange
	marker=0;
	y=0; b=0; r=0;
	for (i=0; i<sizeOut;i++){
		if (format==1) { // YUY2
			switch (marker) {
				case 0:
					output[i] = inputY[y++]; 
					marker=1;
					break;
				case 1:
					output[i] = inputB[b++];
					marker=2;
					break;
				case 2: 
					output[i] = inputY[y++];
					marker=3;
					break;
				case 3:
					output[i] = inputR[r++];
					marker=0;
					break;
				default: 
					printf("Switch marker error!\n");
					break;
			}
		}
		else if (format==2) { // UYVY
			switch (marker) {
				case 0:
					output[i] = inputB[b++]; 
					marker=1;
					break;
				case 1:
					output[i] = inputY[y++];
					marker=2;
					break;
				case 2: 
					output[i] = inputR[r++];
					marker=3;
					break;
				case 3:
					output[i] = inputY[y++];
					marker=0;
					break;
				default: 
					printf("Switch marker error!\n");
					break;
			}
		}

	}


	// write output file
	if ((fout = fopen(outputname, "wb")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}
	if ((status = fwrite(output, 1, sizeOut, fout)) != sizeOut){
		printf("Error in writing to file %s!\n", outputname);
		exit(-1);
	}	
	fclose(fout);
	fout=NULL;

	// write output file
	outputname="inY.yuv";
	if ((fout = fopen(outputname, "wb")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}
	if ((status = fwrite(inputY, 1, sizeY, fout)) != sizeY){
		printf("Error in writing to file %s!\n", outputname);
		exit(-1);
	}	
	fclose(fout);
	fout=NULL;

	// write output file
	outputname="inR.yuv";
	if ((fout = fopen(outputname, "wb")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}
	if ((status = fwrite(inputR, 1, sizeChroma, fout)) != sizeChroma){
		printf("Error in writing to file %s!\n", outputname);
		exit(-1);
	}	
	fclose(fout);
	fout=NULL;
	
	// write output file
	outputname="inB.yuv";
	if ((fout = fopen(outputname, "wb")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}
	if ((status = fwrite(inputB, 1, sizeChroma, fout)) != sizeChroma){
		printf("Error in writing to file %s!\n", outputname);
		exit(-1);
	}	
	fclose(fout);
	fout=NULL;
/*
	for (frame = 0; frame < maxFrameNumber; frame++){
		if (frame < 10){
			prefix="00";
		}
		else if (frame < 100){
			prefix="0";
		}
		else {
			prefix="";
		}
		sprintf(inputname,"Motion_Graphics.%s%d.yuv",prefix,frame);
		//sprintf(inputname,"Motion_Graphics.000_planar420.yuv");
		printf("%d %s\n",frame,inputname);

		if (inputname == NULL || input == NULL){
			printf("Null pointer in call to read unsigned char!\n");
			exit(-1);
		}

		if ((fin=fopen(inputname,"rb")) == NULL){
			printf("Unable to open file %s!\n", inputname);
			exit(-1);
		}
		rewind(fin);

		if ((status = fread(input, 1, size, fin)) != size){
			fclose(fin);
			return 0;
		}
		fclose(fin);
		fin=NULL;
	
	    
		memcpy(output, input, size);
		
	
		if ((status = fwrite(output, 1, size, fout)) != size){
			printf("Error in writing to file %s!\n", outputname);
			exit(-1);
		}		
	}

	fclose(fout);	
*/
	return 1;
}


