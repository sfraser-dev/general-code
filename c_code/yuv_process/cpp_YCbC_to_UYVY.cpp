#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main(int argc, char**argv) {
	FILE *finY, *finB, *finR, *fout;
	unsigned char *inputY=NULL, *inputB=NULL, *inputR=NULL, *output=NULL;;
	int pelsY=1920, linesY=1035;
	int sizeY=pelsY*linesY;
	int pelsChroma=960, linesChroma=1035;
	int sizeChroma=pelsChroma*linesChroma;
	int sizeOut=pelsY*linesY*2;
	char inputnameY[80];
	char inputnameR[80];
	char inputnameB[80];
	//char *inputnameY, inputnameB, inputnameR;
	char *outputname="outUYVY.yuv";
	int status;
	int i;
	int y, b, r;
	int marker;
	int format=2;	// 1=yuy2  2=uyvy
	int chopSize;
	int frame;
	int noFrames=600;

	// process the arguments
	for (i = 1; i < argc; i++){
        	if (*(argv[i]) == '-'){
            		switch (*(++argv[i])){
                		case 'y':
                    			strcpy(inputnameY, argv[++i]);
                    			break;
				case 'b':
                    			strcpy(inputnameB, argv[++i]);
                    			break;
				case 'r':
                    			strcpy(inputnameR, argv[++i]);
                    			break;
                		default:
		                    	printf("Unknown option %c!\n", *argv[i]);
                		    	return -1;
            		}
		}
	}

	inputY	= new unsigned char [sizeY];
	inputB	= new unsigned char [sizeChroma];
	inputR	= new unsigned char [sizeChroma];
	output	= new unsigned char [sizeOut];

	system("del outUYVY.yuv");

	// open Y	
	if ((finY=fopen(inputnameY,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputnameY);
		exit(-1);
	}
	rewind(finY);
	// open R	
	if ((finR=fopen(inputnameR,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputnameR);
		exit(-1);
	}
	rewind(finR);
	// open B	
	if ((finB=fopen(inputnameB,"rb")) == NULL){
		printf("Unable to open file %s!\n", inputnameB);
		exit(-1);
	}
	rewind(finB);
	// open output file
	if ((fout = fopen(outputname, "ab")) == NULL){
		printf("Error in opening file %s!\n", outputname);
		exit(-1);
	}

	// loop over frames
	for(frame=0; frame<noFrames; frame++){
		printf("frame=%d\n",frame);
		if ((status = fread(inputY, 1, sizeY, finY)) != sizeY){
			fclose(finY);
			return 0;
		}
		if ((status = fread(inputB, 1, sizeChroma, finB)) != sizeChroma){
			fclose(finB);
			return 0;
		}
		if ((status = fread(inputR, 1, sizeChroma, finR)) != sizeChroma){
			fclose(finR);
			return 0;
		}
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

		if ((status = fwrite(output, 1, sizeOut, fout)) != sizeOut){
			printf("Error in writing to file %s!\n", outputname);
			exit(-1);
		}	
	}
	
	fclose(finY);
	finY=NULL;
	fclose(finR);
	finR=NULL;
	fclose(finB);
	finB=NULL;
	fclose(fout);
	fout=NULL;

	return 1;

}


