#include "instruction.h"
#include <string.h>
#include <math.h>
#include "register.h"
#include <stdlib.h>
#include <stdio.h>

char getOpSizeInfo(instInfo* info) {return (info->legPreInfo & 0x01);}
char getAddrSizeInfo(instInfo* info) {return ((info->legPreInfo & 0x02) >> 1);}
char getSegmentInfo(instInfo* info) {return ((info->legPreInfo & 0x1C) >> 2);}
char getLockInfo(instInfo* info) {return ((info->legPreInfo & 0x20) >> 5);}
char getRepInfo(instInfo* info) {return ((info->legPreInfo & 0xC0) >> 6);}

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
				morePrefixesPossible = !getOpSizeInfo(info);
				setOpSizeInfo(info,1);
				break;
			case addrSizePre:
				morePrefixesPossible = !getAddrSizeInfo(info);
				setAddrSizeInfo(info,1);
				break;
			case segCSPre:
				morePrefixesPossible = !getSegmentInfo(info);
				setSegmentInfo(info,1);
				break;
			case segDSPre:
				morePrefixesPossible = !getSegmentInfo(info);
				setSegmentInfo(info,2);
				break;
			case segESPre:
				morePrefixesPossible = !getSegmentInfo(info);
				setSegmentInfo(info,3);
				break;
			case segFSPre:
				morePrefixesPossible = !getSegmentInfo(info);
				setSegmentInfo(info,4);
				break;
			case segGSPre:
				morePrefixesPossible = !getSegmentInfo(info);
				setSegmentInfo(info,5);
				break;
			case segSSPre:
				morePrefixesPossible = !getSegmentInfo(info);
				setSegmentInfo(info,6);
				break;
			case lockPre:
				//lock is mutually exclusive with repeaters
				morePrefixesPossible = !getLockInfo(info) || !getRepInfo(info);
				setLockInfo(info,1);
				break;
			case repPre:
				morePrefixesPossible = !getRepInfo(info) || !getLockInfo(info);
				setRepInfo(info, 1);
				break;
			case repnezPre:
				morePrefixesPossible = !getRepInfo(info) || !getLockInfo(info);
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

unsigned char getRexB(instInfo* info) {return info->rexInfo & 0x01;}
unsigned char getRexX(instInfo* info) {return (info->rexInfo & 0x02) >> 1;}
unsigned char getRexR(instInfo*  info) {return (info->rexInfo & 0x04) >> 2;}
unsigned char getRexW(instInfo* info) {return (info->rexInfo & 0x08) >> 3;}

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

unsigned char getXOPVEXR(instInfo* info){return (info->xopVexInfo & 0x10) >> 4;}
unsigned char getXOPVEXB(instInfo* info){return (info->xopVexInfo & 0x04) >> 2;}
unsigned char getXOPVEXQuadV(instInfo* info){return ( info->xopVexInfo & 0x0F00) >> 8;}
//xop/vex vvvv field is encoded in inverted one's complement form.
unsigned char getXOPVEXQuadVReal(instInfo* info){return ~getXOPVEXQuadV(info);}


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
unsigned char getModRegFull(unsigned char byte, instInfo* info){

	unsigned char modReg = getModReg(byte);
	//need to add logic to check if correct circumstancess are present for extension: doesnt always get extended. only 64 bit mode
	unsigned char regExtension = getRexR(info) || !getXOPVEXR(info);
	unsigned char fullReg = (regExtension << 3) || modReg;
	return fullReg;
}

unsigned char getModRm(unsigned char byte){return (0x07 & byte);}
unsigned char getModRMFull(unsigned char byte, instInfo* info){

	unsigned char modRm = getModRm(byte);
	//need to add logic to check if correct circumstancess are present for extension: doesnt always get extended. only 64 bit mode
	unsigned char rmExtension = getRexB(info) || !getXOPVEXB(info);
	unsigned char fullRm = (rmExtension << 3) || modRm;
	return fullRm;
}



unsigned char getSIBScale(unsigned char byte){return (0xC0 &byte) >> 6; }

char  getSIBScaleChar(unsigned char scale){


	int convertedNum = pow(2, scale);
	return convertedNum + 48;//ascii val of '0'
		
}

unsigned char getSIBIndex(unsigned char byte){return (0x38 & byte) >> 3;}
unsigned char getSIBIndexFull(unsigned char byte, instInfo * info){

	unsigned char index = getSIBIndex(byte);
	//need to add logic to check if correct circumstancess are present for extension: doesnt always get extended. only 64 bit mode
	unsigned char indExtension = getRexX(info);
	unsigned char fullIndex = (indExtension << 3) || index;
	return fullIndex;

}

unsigned char getSIBBase(unsigned char byte){return (0x07 & byte);}
unsigned char getSIBBaseFull(unsigned char byte, instInfo * info){

	unsigned char base = getSIBBase(byte);
	//need to add logic to check if correct circumstancess are present for extension: doesnt always get extended. only 64 bit mode
	unsigned char baseExtension = getRexB(info);
	unsigned char fullBase = (baseExtension << 3) || base;
	return fullBase;

}



//register is either found in the mod.reg field, or mod.rm field.
//gets direct or memory operand from either reg or rm field, depending on which location is forced. Enforces mod.mod field rules(mod must be 11b when direct, not 11b when memory)
void getModRMOperandForced(unsigned char isDirectOperand, unsigned char regType, unsigned char regSize, unsigned char isFromRM, unsigned char * modBytePtr, instInfo* info, char * buffer){

	unsigned char modByte = *modBytePtr;
	unsigned char registerKey;
	unsigned char modVal;
	
	if(isDirectOperand){
	
		if(isFromRM){
			modVal = getModMod(modByte);
			if(modVal != modDirect) strcpy(buffer, "-"); //if direct register operand using rm field, mod.mod field must be 11b.
			registerKey = getModRMFull(modByte, info);
		}	
		else registerKey = getModRegFull(modByte, info); 
		
		strcpy(buffer, registers[getRegisterArrayIndex(registerKey, regType, regSize)]);
	
	}
	else{
	
		if(isFromRM){
			modVal = getModMod(modByte);
			if(modVal == modDirect) strcpy(buffer, "-");//if memory operand using rm field, mod.mod field must not be 11b.
			else getModRMOperandUnforced(modBytePtr, regSize, regType, info, buffer);
		
		}
		else strcpy(buffer, "-"); //getting a memory operand from the mod.reg field is not allowed/defined.
		
	}
		
	
	
}


//used for immediate or displacement value reading
//immediate/displacement values are powers of 2 up to the value 8(only allowed in 64 bit).
char * getNextBytesAsString(void * currBytePtr, unsigned char scale){

	
	if(scale >= 4) return NULL;
	long long int number = 0;
	
	switch(scale){
	
		case 0:
			number = *((char * ) currBytePtr);
			break;
		case 1:
			number = *((short int *) currBytePtr);
			break;
		case 2:
			number = *((int *) currBytePtr);
			break;
		case 3:
			number = *((long long int *) currBytePtr);
			break;
	}
	
	int length = snprintf( NULL, 0, "%lli", number);
	char* str = malloc( length + 1 );
	snprintf( str, length + 1, "%lli", number );
	
	return str;
	

}

//16 bit addressing
void getModMemoryOperand16Bit(unsigned char modVal, unsigned char * modBytePtr, instInfo* info, char * buffer){


	unsigned char modByte = *modBytePtr;
	unsigned char modRMUnExtended = getModRM(modByte);
	unsigned char modRMExtended = getModRMFull(modByte, info);

	char* operandArr[8] = {"BX + SI","BX + DI","BP + SI","BP + DI","SI","DI","BP","BX"};
	
	
	strcpy(buffer,"[");
	
	switch(modVal){
	
		case 0x0:
			if(modRMUnExtended == 0x6){
			
				strcat(buffer,getNextBytesAsString(modBytePtr + 1, 1)); 
			}
			else{
				strcat(buffer,operandArr[modRMUnExtended]);
			}
			
			break;
		case 0x1:
			strcat(buffer,operandArr[modRMUnExtended]);
			strcat(buffer," + ");
			strcat(buffer,getNextBytesAsString(modBytePtr + 1, 0)); 
			
			
		case 0x2:
			strcat(buffer, operandArr[modRMUnExtended]);
			strcat(buffer," + ");
			strcat(buffer,getNextBytesAsString(modBytePtr + 1, 1)); 
			break;

		
	}
	strcat(buffer,"]");

}


//32 or 64 bit addressing
void getModMemoryOperandLong(unsigned char modVal, unsigned char * modBytePtr, instInfo * info, unsigned char regSize, char * buffer){

	unsigned char modByte = *modBytePtr;
	unsigned char modRMUnExtended = getModRM(modByte);
	unsigned char modRMExtended = getModRMFull(modByte, info);
	
	if(modVal == 0x0 && modRMUnExtended == 0x5){
		
		//get rip + disp32	
		strcpy(buffer,"[");
		//cat rip
		strcat(buffer," + ");
		strcat(buffer,getNextBytesAsString(modBytePtr + 1, 2)); 
		strcat(buffer,"]");
		
	}
	
	
	if(modRMUnExtended == 4){
	
		getSIBOperand((modBytePtr + 1), modVal, regSize, info, buffer);
	
	}
	else{
		//memory accesses using registers always use the general purpose regs.
		strcpy(buffer,"[");
		strcat(buffer,registers[getRegisterArrayIndex(modRMExtended, genPurpose, regSize)]);
		if(modVal == 0x1){
			strcat(buffer," + ");
			strcat(buffer,getNextBytesAsString(modBytePtr + 1, 0)); 
		}
		else if(modVal == 0x2){
			strcat(buffer," + ");
			strcat(buffer,getNextBytesAsString(modBytePtr + 1, 2)); 
		
		}
		strcat(buffer,"]");
	
	}
	
	
	
}



// get the register or memory operand based on the modRM byte. Unforced refers to the fact the modRm.mod field automatically determines if the operand is direct or memory based.
void getModRMOperandUnforced(unsigned char * modBytePtr, unsigned char regSize, unsigned char regType, instInfo* info, char * buffer){


	unsigned char modByte = *modBytePtr;
	unsigned char modVal = getModMod(modByte);
		
	if(modVal == modDirect){
		
		unsigned char modRMExtended = getModRMFull(modByte, info);
		strcpy(buffer, registers[getRegisterArrayIndex(modRMExtended, regType, regSize)]);
	}
	else{
		
		if(regSize < 2) getModMemoryOperand16Bit(modVal, modBytePtr, info, buffer);
		else getModMemoryOperandLong(modVal, modBytePtr, info, regSize, buffer);
		

	}
	
} 


//models the SIB tables found at https://wiki.osdev.org/X86-64_Instruction_Encoding
//output stored in buffer, make sure it has room! good amount is probably 15 chars.
//sibBytePtr should be pointing to the sib byte, addresses after sib byte may point to displacement bytes.
void getSIBOperand(unsigned char * sibBytePtr, unsigned char modValue, unsigned char regSize, instInfo* info, char * buffer){

	
	unsigned char sibByte = *sibBytePtr;
	unsigned char scale =  getSIBScale(sibByte);
	unsigned char index = getSIBIndexFull(sibByte,info);
	unsigned char base = getSIBBaseFull(sibByte, info);
	unsigned char baseShort = getSIBBase(sibByte);
	
	char * baseReg = registers[getRegisterArrayIndex(base, genPurpose, regSize)]; // confirm this size!
	char * indexReg = registers[getRegisterArrayIndex(index, genPurpose, regSize)];// confirm this size!
	
	char scaleString[2] = {getSIBScaleChar(scale), 0};
	
	strcpy(buffer, "[");
	
	switch(modValue){
	
	
		case 0x0:
		
			if(baseShort != 5){
			
				if(index == 4) strcat(buffer, baseReg);
				else{
					strcat(buffer, baseReg);
					strcat(buffer, " + (");
					strcat(buffer, indexReg);
					strcat(buffer, " * ");
					strcat(buffer, scaleString);
					strcat(buffer,")");
				}
			
			}
			else{
				
				char * disp32 = getNextBytesAsString(sibBytePtr + 1, 2);
				if(index == 4){
				
					strcat(buffer,disp32);
				
				}
				else{

					strcat(buffer, "(");
					strcat(buffer, indexReg);
					strcat(buffer, " * ");
					strcat(buffer, scaleString);
					strcat(buffer, ") + ");
					strcat(buffer, disp32);
				}

			}
			break;
			
		case 0x1:
		
			char * disp8 = getNextBytesAsString(sibBytePtr + 1, 0);
			
			if(index == 4){
			
				strcat(buffer, baseReg);
				strcat(buffer," + ");
				strcat(buffer,disp8);
			}
			else{

				strcat(buffer, baseReg);
				strcat(buffer, " + (");
				strcat(buffer, indexReg);
				strcat(buffer, " * ");
				strcat(buffer, scaleString);
				strcat(buffer,")");
				strcat(buffer," + ");
				strcat(buffer,disp8);
				
			}
			
			break;
			
		case 0x2:
		
			char * disp32 = getNextBytesAsString(sibBytePtr + 1, 2);
			
			if(index == 4){

				strcat(buffer, baseReg);
				strcat(buffer," + ");
				strcat(buffer,disp32);

			}
			else{

				strcat(buffer, baseReg);
				strcat(buffer, " + (");
				strcat(buffer, indexReg);
				strcat(buffer, " * ");
				strcat(buffer, scaleString);
				strcat(buffer,")");
				strcat(buffer," + ");
				strcat(buffer,disp32);
			}
			
			break;
		
	
	}

	strcat(buffer,"]");
	
}

































