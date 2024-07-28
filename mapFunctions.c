#include "mapFunctions.c"
#include "instruction.h"
#include "register.h"


char * operandB(unsigned char size, instInfo* info){

	//this operand requires the xop/vex vvvvfield, which was already read before the opcode(stored in info).
	
	unsigned char byte = getXOPVEXQuadVReal(info);
	
	//operand of B implies general purpose register, size is provided.
	return registers[getRegisterArrayIndex(byte, control, size)];
}


char * operandC(unsigned char size, unsigned char * opByte, instInfo* info){

	//mod byte is expected, which immediatly follows the op byte
	unsigned char byte = *(opByte +1);
	byte = getModRegFull(byte,info);
	
	//operand of C implies control register, size is provided.
	return registers[getRegisterArrayIndex(byte, control, size)];
}

char * operandD(unsigned char size, unsigned char * opByte, instInfo* info){

	//mod byte is expected, which immediatly follows the op byte
	unsigned char byte = *(opByte +1);
	byte = getModRegFull(byte,info);
	
	//operand of D implies debug register, size is provided.
	return registers[getRegisterArrayIndex(byte, debug, size)];
}

void operandE(unsigned char size){



}

char * operandG(unsigned char size, unsigned char * opByte, instInfo* info){

	
	//mod byte is expected, which immediatly follows the op byte
	unsigned char byte = *(opByte +1);
	byte = getModRegFull(byte,info);
	
	//operand of G implies general purpose register, size is provided.
	return registers[getRegisterArrayIndex(byte, genPurpose, size)];
}

char * operandH(unsigned char size, instInfo* info){

	//this operand requires the xop/vex vvvvfield, which was already read before the opcode(stored in info).
	
	unsigned char byte = getXOPVEXQuadVReal(info);
	
	//operand of H implies xmm or ymm register(not sure which one to choose yet), size is provided.
	return registers[getRegisterArrayIndex(byte, xmm, size)];
}

char * operandN(unsigned char size, unsigned char * opByte, instInfo* info){

	//this operand requires the mod.rm field, which will be in the next byte.
	
	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);
	
	unsigned char mod = getModMod(byte);
	if(mod != 0x3) return "-"; // mod field must be 11b in this operand
	
	//operand of N implies mmx register, size is provided(although size should always be 0 since mmx type registers are only 64 bit).
	return registers[getRegisterArrayIndex(fullRm, mmx, size)];
}




