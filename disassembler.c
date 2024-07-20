
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

//opSizePre, addrSizePre, segmentPre(0/1-6),lockPre, repPre(0/1-3)
char legacyPrefixInfo[5] = {0,0,0,0,0};

//assumes this is given a 15 byte string. a legal instruction might only make up part of this string, or all of it.
char legacyPrefixFSM(char* instructionCandidate){

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
			case lockPre:
				//lock is mutually exclusive with repeaters
				morePrefixesPossible = !legacyPrefixInfo[3] || !legacyPrefixInfo[4];
				legacyPrefixInfo[3] = 1;
				break;
			case repPre:
				morePrefixesPossible = !legacyPrefix[4] || !legacyPrefixInfo[3];
				legacyPrefixInfo[4] = 1;
				break;
			case repezPre:
				morePrefixesPossible = !legacyPrefix[4] || !legacyPrefixInfo[3];
				legacyPrefixInfo[4] = 2;
				break;
			case repenzPre:
				morePrefixesPossible = !legacyPrefix[4] || !legacyPrefixInfo[3];
				legacyPrefixInfo[4] = 3;
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

char isRexPrefix(char byte){

	return ((byte & 0xF0) == 0x40);


}


//rex prefix, 0F escape, vex/xop prefix
char secondaryPrefixInfo[3] = {0,0,0};

//assumes this is given a 15 byte string. a legal instruction might only make up part of this string, or all of it.
char secondaryPrefixFSM(char * currentByte){

	//rex prefix ranges from 40 to 4F
	
	//rex prefix + primary opcode
	//rex prefix + 0F escape
	// no rex prefix + primary opcode
	//no rex prefix + 0F escape
	//no rex prefix + vex prefix
	// no rex prefix + xop prefix
	
	char currChar = *instructionCandidate;
	if(isRexPrefix(currChar)){
	
		secondaryPrefixInfo[0] = 1;
		currChar = *(instructionCandidate + 1)
		if(currChar == 0x0F)secondaryPrefixInfo[1] = 1;
		return 0;
	
	}
	else{
		switch(currChar){
		
			case 0xC5:
			//vex 2 byte sequence
			
			case 0xC4:
			//vex 3 byte sequence
			
			
			case 0x8F:
			//xop 3 byte sequence
			
			case default:
			
			break;
			
		
		
		
		
		
		}
	
	
	
	}







}


int mapSelect(char mapCode){

	switch(mapCode){
	
		case 0:
		//primary opcode
		break;
		case 1:
		//second opcode
		break;
		case 2:
		//0F38 opcode
		break;
		case 3:
		//0F3A opcode
		case 4:
		//vex 1 opcode
		break;
		case 5:
		//vex 2 opcode
		break;
		case 6:
		//vex 3 opcode
		break;
		case 7:
		//xop 8 opcode
		break;
		case 8:
		//xop 9 opcode
		break;
		case 9:
		//xop A opcode
		case default:
			return -1;
	
	}


}

int main(int argc, char** argv){

	char instruction[15];
	legacyPrefixFSM(instruction);










}
