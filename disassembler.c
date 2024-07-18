
#define maxInstructionLength 15 //bytes

#define opSizePre = 0x66;
#define addrSizePre = 0x67;
#define segCSPre = 0x2E;
#define segDSPre = 0x3E;
#define segESPre = 0x26;
#define segFSPre =  0x64;
#define segGSPre = 0x65;
#define segSSPre = 0x36;
#define  lockPre = 0xF0;
#define repPre = 0xF3;
#define repezPre = 0xF3; //repe or repz
#define repnezPre = 0xF2; // repne or repnz

//opSizePre, addrSizePre, segmentPre(0/1-6), repPre(0/1-3)
char legacyPrefixInfo[5] = {0,0,0,0,0};

//assumes this is given a 15 byte string. a legal instruction might only make up part of this string, or all of it.
int legacyPrefixFSM(char* instructionCandidate){

	char bytesRead = 0;
	char* currentPos = instructionCandidate;
	char morePrefixesPossible = 1;
	while(morePrefixesPossible){
		
		char currByte = *currentPos;
		 
		switch(currByte){
		
			case opSizePre:
				morePrefixesPossible = !legacyPrefixInfo[0];
				legacyPrefixInfo[0] = 1;
				break;
			case addrSizePre:
				morePrefixesPossible = !legacyPrefixInfo[1];
				legacyPrefixInfo[1] = 1;
				break;
			case segCSPre:
				morePrefixesPossible = !legacyPrefixInfo[2];
				legacyPrefixInfo[2] = 1;
				break;
			case segDSPre:
				morePrefixesPossible = !legacyPrefixInfo[2];
				legacyPrefixInfo[2] = 2;
				break;
			case segESPre:
				morePrefixesPossible = !legacyPrefixInfo[2];
				legacyPrefixInfo[2] = 3;
				break;
			case segFSPre:
				morePrefixesPossible = !legacyPrefixInfo[2];
				legacyPrefixInfo[2] = 4;
				break;
			case segGSPre:
				morePrefixesPossible = !legacyPrefixInfo[2];
				legacyPrefixInfo[2] = 5;
				break;
			case segSSPre:
				morePrefixesPossible = !legacyPrefixInfo[2];
				legacyPrefixInfo[2] = 6;
				break;
			case repPre:
				morePrefixesPossible = !legacyPrefix[3];
				legacyPrefixInfo[3] = 1;
				break;
			case repezPre:
				morePrefixesPossible = !legacyPrefix[3];
				legacyPrefixInfo[3] = 2;
				break;
			case repenzPre:
				morePrefixesPossible = !legacyPrefix[3];
				legacyPrefixInfo[3] = 3;
				break;
			case default:
				//unrecognized legacy prefix
				morePrefixesPossible = 0;
		}			
		


		currentByte = currentByte + 1;
		bytesRead++;
		//total legacy prefixes cannot exceed 5, can only use one legacy prefix from each group;
		if(bytesRead >= 4)morePrefixesPossible = 0;
	}

	
	return 0;


}

int main(int argc, char** argv){












}
