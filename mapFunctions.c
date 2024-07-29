#include "mapFunctions.c"
#include "instruction.h"
#include "register.h"


char * operandB(unsigned char size, instInfo* info){

	unsigned char byte = getXOPVEXQuadVReal(info);	
	//operand of B implies general purpose register
	return registers[getRegisterArrayIndex(byte, genPurpose, size)];
}


char * operandC(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	byte = getModRegFull(byte,info);
	//operand of C implies control register
	return registers[getRegisterArrayIndex(byte, control, size)];
}

char * operandD(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	byte = getModRegFull(byte,info);
	//operand of D implies debug register
	return registers[getRegisterArrayIndex(byte, debug, size)];
}

char * operandE(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);
	
	unsigned char mod = getModMod(byte);
	//this operand specifies either a genPurpose register operand or memory operand. mod.mod field tells which one should be used.
	if(mod == modDirect){
		
		return registers[getRegisterArrayIndex(fullRm, genPurpose, size)];
	}
	else return registers[getRegisterArrayIndex(fullRm, genPurpose, size)];

}

char * operandG(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	byte = getModRegFull(byte,info);
	//operand of G implies general purpose register
	return registers[getRegisterArrayIndex(byte, genPurpose, size)];
}

char * operandH(unsigned char size, instInfo* info){

	unsigned char byte = getXOPVEXQuadVReal(info);
	//operand of H implies xmm or ymm register(not sure which one to choose yet)
	return registers[getRegisterArrayIndex(byte, xmm, size)];
}

char * operandM(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);
	unsigned char mod = getModMod(byte);
	if(mod == modDirect) return "-"; // mod field must be not 11b in this operand
	//operand of M implies a memory operand from the modrm field. memory operands from this field only use the gen purpose registers in the reg table.
	return registers[getRegisterArrayIndex(fullRm, genPurpose, size)];
}


char * operandN(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);	
	unsigned char mod = getModMod(byte);
	if(mod != modDirect) return "-"; // mod field must be 11b in this operand
	//operand of N implies mmx register, size is provided(although size should always be 0 since mmx type registers are only 64 bit).
	return registers[getRegisterArrayIndex(fullRm, mmx, size)];
}

char * operandP(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullReg = getModRegFull(byte,info);
	//operand of P implies mmx register, size is provided(although size should always be 0 since mmx type registers are only 64 bit).
	return registers[getRegisterArrayIndex(fullReg, mmx, size)];
}

char * operandQ(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);
	unsigned char mod = getModMod(byte);
	//this operand specifies either an mmx register operand or memory operand. mod.mod field tells which one should be used.
	if(mod == modDirect){
		
		return registers[getRegisterArrayIndex(fullRm, mmx, size)];
	}
	else return registers[getRegisterArrayIndex(fullRm, genPurpose, size)];
	
}

char * operandR(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);
	unsigned char mod = getModMod(byte);
	if(mod != modDirect) return "-"; // mod field must be 11b in this operand
	//operand of R implies general purpose register
	return registers[getRegisterArrayIndex(fullRm, genPurpose, size)];
}

char * operandS(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullReg = getModRegFull(byte,info);	
	//operand of S implies segment register
	return registers[getRegisterArrayIndex(fullReg, seg, size)];
}

char * operandU(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);
	unsigned char mod = getModMod(byte);
	if(mod != modDirect) return "-"; // mod field must be 11b in this operand
	//operand of U implies xmm or ymm register(not sure which yet)
	return registers[getRegisterArrayIndex(fullRm, xmm, size)];
}

char * operandV(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullReg = getModRegFull(byte,info);
	//operand of V implies xmm or ymm register(not sure which yet)
	return registers[getRegisterArrayIndex(fullReg, xmm, size)];
}

char * operandW(unsigned char size, unsigned char * opByte, instInfo* info){

	unsigned char byte = *(opByte +1);
	unsigned char fullRm = getModRmFull(byte,info);
	unsigned char mod = getModMod(byte);
	//this operand specifies either an xmm/ymm register operand or memory operand. mod.mod field tells which one should be used.
	if(mod == modDirect){
		
		return registers[getRegisterArrayIndex(fullRm, xmm, size)];
	}
	else return registers[getRegisterArrayIndex(fullRm, genPurpose, size)];
}




