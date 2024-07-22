#include "instruction.h"

char getOpSizeInfo(instInfo info) return (info.legPreInfo & 0x01);
char getAddrSizeInfo(instInfo info) return ((info.legPreInfo & 0x02) >> 1);
char getSegmentInfo(instInfo info) return ((info.legPreInfo & 0x1C) >> 2);
char getLockInfo(instInfo info) return ((info.legPreInfo & 0x20) >> 5);
char getSeqInfo(instInfo info) return ((info.legPreInfo & 0xC0) >> 6);

void setOpSizeInfo(instInfo* info, int value) info->legPreInfo |= (value & 0x01);
void setAddrSizeInfo(instInfo* info, int value) info->legPreInfo |= ((value << 1) & 0x02);
void setSegmentInfo(instInfo* info, int value) info->legPreInfo |= ((value << 2) & 0x1C);
void setLockInfo(instInfo* info, int value) info->legPreInfo |= (value << 5) & 0x20);
void setSeqInfo(instInfo* info, int value) info->legPreInfo |= (value << 6) & 0xC0);


//assumes this is given a 15 byte string. a legal instruction might only make up part of this string, or all of it.
char legacyPrefixFSM(char* instructionCandidate, instInfo* info){

	char bytesRead = 0;
	char* currentPos = instructionCandidate;
	char morePrefixesPossible = 1;
	while(morePrefixesPossible){
		
		char currByte = *currentPos;
		 
		switch(currByte){
		
			case opSizePre:
				morePrefixesPossible = !getOpSizeInfo(*info);
				setOpSizeInfo(info,1);
				break;
			case addrSizePre:
				morePrefixesPossible = !getAddrSizeInfo(*info);
				setAddrSizeInfo(info,1);
				break;
			case segCSPre:
				morePrefixesPossible = !getSegmentInfo(*info);
				setSegmentInfo(info,1);
				break;
			case segDSPre:
				morePrefixesPossible = !getSegmentInfo(*info);
				setSegmentInfo(info,2);
				break;
			case segESPre:
				morePrefixesPossible = !getSegmentInfo(*info);
				setSegmentInfo(info,3);
				break;
			case segFSPre:
				morePrefixesPossible = !getSegmentInfo(*info);
				setSegmentInfo(info,4);
				break;
			case segGSPre:
				morePrefixesPossible = !getSegmentInfo(*info);
				setSegmentInfo(info,5);
				break;
			case segSSPre:
				morePrefixesPossible = !getSegmentInfo(*info);
				setSegmentInfo(info,6);
				break;
			case lockPre:
				//lock is mutually exclusive with repeaters
				morePrefixesPossible = !getLockInfo(*info) || !getRepInfo(*info);
				setLockInfo(info,1);
				break;
			case repPre:
				morePrefixesPossible = !getRepInfo(*info) || !getLockInfo(*info);
				setRepInfo(info, 1);
				break;
			case repezPre:
				morePrefixesPossible = !getRepInfo(*info) || !getLockInfo(*info);
				setRepInfo(info, 2);
				break;
			case repenzPre:
				morePrefixesPossible = !getRepInfo(*info) || !getLockInfo(*info);
				setRepInfo(info, 3);
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

char isRexPrefix(char byte) return ((byte & 0xF0) == 0x40);

char getRexB(instInfo info) return info.rexInfo & 0x01;
char getRexX(instInfo info) return (info.rexInfo & 0x02) >> 1;
char getRexR(instInfo  info) return (info.rexInfo & 0x04) >> 2;
char getRexW(instInfo info) return (info.rexInfo & 0x08) >> 3;

void setRexByte(instInfo info, char rexByte) info->rexInfo = (rexByte & 0x0F);


char normalEscapeSequenceFSM(char* currentPos){

	char currByte = *currentPos;
	
	
	switch(currByte){
	
		case 0x0F:
			//3DNow! escape, nothing really to do now
			break;
		case 0x38:
			//0F_38 opcode map
			break;
		case 0x3A:
			//OF_3A opcode map
			break;
		
		case default:
			//if its not any other escape sequence above, it is still possible this is an opcode in the secondary opcode map.
			//secondary opcode map
			break;
	
	}
	
	return 0;

}


char getMapSelect(char byte) return byte & 0x1F;

void setXOPVexInfoThreeByte(instInfo * info, char isXop, unsigned char firstByte, unsigned char secondByte){
	
	unsigned short int top = ((unsigned short int)secondByte) << 6;
	unsigned short int middle = (((unsigned short int) firstByte) & 0xE0) >> 3;
	unsigned short int bottom = (isXop & 0x01) | 0x02;
	
	info->xopVexInfo = bottom | middle | top;
	 
}
void setVexInfoTwoByte(instInfo * info, unsigned char firstByte){

	unsigned short int bottom = 0x0;
	unsigned short int middle = (firstByte & 0x80) >> 3;
	unsigned short int top = ((unsigned short int)firstByte & 0x7F) << 5);
	info->xopVexInfo = bottom | middle | top;
	
}


char queryXOPTables(char opByte, char mapSelect){

	switch(mapSelect){
	
		case 0x08:
			//xop 08 table
			break;
		case 0x09:
			//xop 09 table
			break;
		case 0x0A:
			//xop 0A table
			break;
		case default:
			//at this point, the selecter needed to be one of above 3. this means this is an illegal path.
			return -1; 
	
	}
	
	return 0;

}

char queryVEXTables(char opByte, char mapSelect){

	switch(mapSelect){
	
		case 0x01:
			//vex 01 table
			break;
		case 0x02:
			//vex 02 table
			break;
		case 0x03:
			//vex 03 table
			break;
		case default:
			//at this point, the selecter needed to be one of above 3. this means this is an illegal path.
			return -1; 
	
	}
	
	return 0;

}

char parseXOPVEXSequence(char isXOP, char isThreeByte, char* currentPos, instInfo* info){


	if(isXOP){
		char firstByte = *currentPos;
		char secondByte = *(currentPos + 1);
		char opByte = *(currentPos + 2);
		char mapSelect = getMapSelect(firstByte);
		
		setXOPVexInfoThreeByte(info, 1, firstByte, secondByte);
		queryXOPTables(opByte, mapSelect);
	
	
	}
	else{
	
	
		if(isThreeByte){
			char firstByte = *currentPos;
			char secondByte = *(currentPos + 1);
			char opByte = *(currentPos + 2);
			char mapSelect = getMapSelect(firstByte);
		
			setXOPVexInfoThreeByte(info, 0, firstByte, secondByte);
			queryVEXTables(opByte, mapSelect);
		
		
		}
		else{
		
			char firstByte = *currentPos;
			char opByte = *(currentPos + 1);
			setVexInfoTwoByte(info, firstByte);
			//using the vex two byte sequence path implies a vex map select of 0x01.
			char mapSelect = 1;
			queryVEXTables(opByte, mapSelect);
		
		
		}
	
	}




}

char secondaryPrefixFSM(char * currentPos, instInfo* info){

	//rex prefix + primary opcode
	//rex prefix + 0F escape
	// no rex prefix + primary opcode
	//no rex prefix + 0F escape
	//no rex prefix + vex prefix
	// no rex prefix + xop prefix
	
	char currChar = *currentPos;
	if(isRexPrefix(currChar)){
	
		setRexByte(info,currChar);
		
		currChar = *(currentPos +1);
		if(currChar == 0x0F){
			//escape char, enter secondary opcode maps(non vex or xop)
			normalEscapeSequenceFSM(currentPos + 2);
		
		}
		//otherwise, has to be an opcode from the primary opcode table. cant have xop or vex after a rex prefix, this will be reflected in an invalid value for those prefixes in the primary opcode table.
		else{
		
			
		
		
		}
	
	}
	else{
		switch(currChar){
		
			case 0xC5:
				//vex 2 byte sequence OR THIS COULD BE AN LES PRIMARY OPCODE. WILL NEED TO LOOK AHEAD 
				parseXOPVEXSequence(0, 0, currentPos + 1, info)
			
			case 0xC4:
				//vex 3 byte sequence OR THIS COULD BE AN LDS PRIMARY OPCODE. WILL NEED TO LOOK AHEAD 
				parseXOPVEXSequence(0, 1, currentPos + 1, info)
			
			case 0x8F:
				//xop 3 byte sequence
				parseXOPVEXSequence(1, 1, currentPos + 1, info)
			
			case 0x0F:
				//0F escape
				normalEscapeSequenceFSM(currentPos + 1);
				
			case default:
				//could be primary opcode on its own, or might be an illegal instruction.
			
			break;
			
		
		}
	
	
	
	}







}



























