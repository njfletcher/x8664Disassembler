#include "instruction.h"





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
			
			case 0x0F:
			//0F escape
			
			case default:
			//could be primary opcode on its own, or might be an illegal instruction.
			
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

	instInfo info;
	
	char instruction[15];
	legacyPrefixFSM(instruction, &info);










}
