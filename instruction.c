#include "instruction.h"

char getOpSizeInfo(instInfo info) {return (info.legPreInfo & 0x01);}
char getAddrSizeInfo(instInfo info) {return ((info.legPreInfo & 0x02) >> 1);}
char getSegmentInfo(instInfo info) {return ((info.legPreInfo & 0x1C) >> 2);}
char getLockInfo(instInfo info) {return ((info.legPreInfo & 0x20) >> 5);}
char getRepInfo(instInfo info) {return ((info.legPreInfo & 0xC0) >> 6);}

void setOpSizeInfo(instInfo* info, int value) {info->legPreInfo |= (value & 0x01);}
void setAddrSizeInfo(instInfo* info, int value) {info->legPreInfo |= ((value << 1) & 0x02);}
void setSegmentInfo(instInfo* info, int value) {info->legPreInfo |= ((value << 2) & 0x1C);}
void setLockInfo(instInfo* info, int value) {info->legPreInfo |= ((value << 5) & 0x20);}
void setRepInfo(instInfo* info, int value) {info->legPreInfo |= ((value << 6) & 0xC0);}


//assumes this is given a 15 byte string. a legal instruction might only make up part of this string, or all of it.
char legacyPrefixFSM(unsigned char* instructionCandidate, instInfo* info){

	unsigned char bytesRead = 0;
	unsigned char* currentPos = instructionCandidate;
	unsigned char morePrefixesPossible = 1;
	while(morePrefixesPossible){
		
		unsigned char currByte = *currentPos;
		 
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
			case repnezPre:
				morePrefixesPossible = !getRepInfo(*info) || !getLockInfo(*info);
				setRepInfo(info, 2);
				break;
			default:
				//unrecognized legacy prefix
				morePrefixesPossible = 0;
		}			
		


		currentPos = currentPos + 1;
		bytesRead++;
		//total legacy prefixes cannot exceed 5, can only use one legacy prefix from each group;
		if(bytesRead >= 4)morePrefixesPossible = 0;
	}

	
	return 0;


}

char isRexPrefix(unsigned char byte) {return ((byte & 0xF0) == 0x40);}

unsigned char getRexB(instInfo info) {return info.rexInfo & 0x01;}
unsigned char getRexX(instInfo info) {return (info.rexInfo & 0x02) >> 1;}
unsigned char getRexR(instInfo  info) {return (info.rexInfo & 0x04) >> 2;}
unsigned char getRexW(instInfo info) {return (info.rexInfo & 0x08) >> 3;}

void setRexByte(instInfo* info, unsigned char rexByte) {info->rexInfo = (rexByte & 0x0F);}


char normalEscapeSequenceFSM(unsigned char* currentPos){

	unsigned char currByte = *currentPos;
	
	
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
		
		default:
			//if its not any other escape sequence above, it is still possible this is an opcode in the secondary opcode map.
			//secondary opcode map
			break;
	
	}
	
	return 0;

}


char getMapSelect(unsigned char byte) {return byte & 0x1F;}

void setXOPVexInfoThreeByte(instInfo * info, unsigned char isXop, unsigned char firstByte, unsigned char secondByte){
	
	unsigned short int top = ((unsigned short int)secondByte) << 6;
	unsigned short int middle = (((unsigned short int) firstByte) & 0xE0) >> 3;
	unsigned short int bottom = (isXop & 0x01) | 0x02;
	
	info->xopVexInfo = bottom | middle | top;
	 
}
void setVexInfoTwoByte(instInfo * info, unsigned char firstByte){

	unsigned short int bottom = 0x0;
	unsigned short int middle = (firstByte & 0x80) >> 3;
	unsigned short int top = (((unsigned short int)firstByte & 0x7F) << 5);
	info->xopVexInfo = bottom | middle | top;
	
}


char queryXOPTables(unsigned char opByte, unsigned char mapSelect){

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
		default:
			//at this point, the selecter needed to be one of above 3. this means this is an illegal path.
			return -1; 
	
	}
	
	return 0;

}

char queryVEXTables(unsigned char opByte, unsigned char mapSelect){

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
		default:
			//at this point, the selecter needed to be one of above 3. this means this is an illegal path.
			return -1; 
	
	}
	
	return 0;

}

unsigned char getXOPVEXR(instInfo * info){return (info->xopVexInfo & 0x10) >> 4;}
unsigned char getXOPVEXB(instInfo * info){return (info->xopVexInfo & 0x04) >> 2;}


char parseXOPVEXSequence(unsigned char isXOP, unsigned char isThreeByte, unsigned char* currentPos, instInfo* info){


	if(isXOP){
		unsigned char firstByte = *currentPos;
		unsigned char secondByte = *(currentPos + 1);
		unsigned char opByte = *(currentPos + 2);
		unsigned char mapSelect = getMapSelect(firstByte);
		
		setXOPVexInfoThreeByte(info, 1, firstByte, secondByte);
		queryXOPTables(opByte, mapSelect);
	
	
	}
	else{
	
	
		if(isThreeByte){
			unsigned char firstByte = *currentPos;
			unsigned char secondByte = *(currentPos + 1);
			unsigned char opByte = *(currentPos + 2);
			unsigned char mapSelect = getMapSelect(firstByte);
		
			setXOPVexInfoThreeByte(info, 0, firstByte, secondByte);
			queryVEXTables(opByte, mapSelect);
		
		
		}
		else{
		
			unsigned char firstByte = *currentPos;
			unsigned char opByte = *(currentPos + 1);
			setVexInfoTwoByte(info, firstByte);
			//using the vex two byte sequence path implies a vex map select of 0x01.
			unsigned char mapSelect = 1;
			queryVEXTables(opByte, mapSelect);
		
		
		}
	
	}




}

char secondaryPrefixFSM(unsigned char * currentPos, instInfo* info){

	//rex prefix + primary opcode
	//rex prefix + 0F escape
	// no rex prefix + primary opcode
	//no rex prefix + 0F escape
	//no rex prefix + vex prefix
	// no rex prefix + xop prefix
	
	unsigned char currChar = *currentPos;
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
				parseXOPVEXSequence(0, 0, currentPos + 1, info);
			
			case 0xC4:
				//vex 3 byte sequence OR THIS COULD BE AN LDS PRIMARY OPCODE. WILL NEED TO LOOK AHEAD 
				parseXOPVEXSequence(0, 1, currentPos + 1, info);
			
			case 0x8F:
				//xop 3 byte sequence
				parseXOPVEXSequence(1, 1, currentPos + 1, info);
			
			case 0x0F:
				//0F escape
				normalEscapeSequenceFSM(currentPos + 1);
				
			default:
				//could be primary opcode on its own, or might be an illegal instruction.
			
			break;
			
		
		}
	
	
	
	}







}






unsigned char getModMod(unsigned char byte){return (0xC0 &byte) >> 6; }
unsigned char getModReg(unsigned char byte){return (0x38 & byte) >> 3;}
unsigned char getModRm(unsigned char byte){return (0x07 & byte);}


char parseModRMByte(unsigned char byte, instInfo* info){


	unsigned char modMod = getModMod(byte);
	unsigned char modReg = getModReg(byte);
	unsigned char modRm = getModRm(byte);
	
	unsigned char regExtension = getRexR(info) || !getXOPVEXR(info);
	unsigned char fullReg = (regExtension << 3) || modReg;
	
	unsigned rmExtension = getRexB(info) || !getXOPVEXB(info);
	unsigned char fullRm = (rmExtension << 3) || modRm;




}


unsigned char getSIBScale(unsigned char byte){return (0xC0 &byte) >> 6; }
unsigned char getSIBIndex(unsigned char byte){return (0x38 & byte) >> 3;}
unsigned char getSIBBase(unsigned char byte){return (0x07 & byte);}


char parseSIBByte(unsigned char byte, instInfo* info){


	unsigned char sibScale = getSIBScale(byte);
	unsigned char sibIndex = getSIBIndex(byte);
	unsigned char sibBase = getSIBBase(byte);
	
	unsigned char indexExtension = getRexX(info);
	unsigned char fullIndex = (indexExtension << 3) || sibIndex;
	
	unsigned baseExtension = getRexB(info);
	unsigned char fullBase = (baseExtension << 3) || sibBase;




}



























