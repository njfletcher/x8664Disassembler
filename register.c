#include "register.h"

//models the register table found at https://wiki.osdev.org/X86-64_Instruction_Encoding
//the four special register spots which depend on if a rex prefix is used or not change dynamically.

char * registers[176] = {

	"AL", "AX", "EAX", "RAX", "ST0", "MMX0", "XMM0", "YMM0", "ES", "CR0", "DR0", 
	"CL", "CX", "ECX", "RCX", "ST1", "MMX1", "XMM1", "YMM1", "CS", "CR1", "DR1", 
	"DL", "DX", "EDX", "RDX", "ST2", "MMX2", "XMM2", "YMM2", "SS", "CR2", "DR2", 
	"BL", "BX", "EBX", "RBX", "ST3", "MMX3", "XMM3", "YMM3", "DS", "CR3", "DR3", 
	"AH", "SP", "ESP", "RSP", "ST4", "MMX4", "XMM4", "YMM4", "FS", "CR4", "DR4", 
	"CH", "BP", "EBP", "RBP", "ST5", "MMX5", "XMM5", "YMM5", "GS", "CR5", "DR5", 
	"DH", "SI", "ESI", "RSI", "ST6", "MMX6", "XMM6", "YMM6", "-", "CR6", "DR6", 
	"BH", "DI", "EDI", "RDI", "ST7", "MMX7", "XMM7", "YMM7", "-", "CR7", "DR7", 
	"R8L", "R8W", "R8D", "R8", "-", "MMX0", "XMM8", "YMM8", "ES", "CR8", "DR8", 
	"R9L", "R9W", "R9D", "R9", "-", "MMX1", "XMM9", "YMM9", "CS", "CR9", "DR9", 
	"R10L", "R10W", "R10D", "R10", "-", "MMX2", "XMM10", "YMM10", "SS", "CR10", "DR10", 
	"R11L", "R11W", "R11D", "R11", "-", "MMX3", "XMM11", "YMM11", "DS", "CR11", "DR11", 
	"R12L", "R12W", "R12D", "R12", "-", "MMX4", "XMM12", "YMM12", "FS", "CR12", "DR12", 
	"R13L", "R13W", "R13D", "R13", "-", "MMX5", "XMM13", "YMM13", "GS", "CR13", "DR13", 
	"R14L", "R14W", "R14D", "R14", "-", "MMX6", "XMM14", "YMM14", "-", "CR14", "DR14", 
	"R15L", "R15W", "R15D", "R15", "-", "MMX7", "XMM15", "YMM15", "-", "CR15", "DR15"

};

//return value of -1 denotes illegal val/type/size
int getRegisterArrayIndex(unsigned char regVal, unsigned char regType, unsigned char regSize){

	if(regVal >= 16 || regType >= 8 || regSize >= 5){
		return -1;
	}
	unsigned char baseIndex = regVal * 15;
	unsigned char rowStartIndex = 0;
	switch(regType){
	
		case genPurpose:
			rowStartIndex = 0;
			break;
		case x87:
			rowStartIndex = 4;
			break;
		case mmx:
			rowStartIndex = 5;
			break;
		case xmm:
			rowStartIndex = 6;
			break;
		case ymm:
			rowStartIndex = 7;
			break;
		case seg:
			rowStartIndex = 8;
			break;
		case control:
			rowStartIndex = 9;
			break;
		case debug:
			rowStartIndex = 10;
			break;
		case default:
			return -1;

	}
	
	//no bounds checking on size, so could bleed over into another register type if you specify a wrong size.
	//ie x87 register has only one size(80 bit). if you wanted size index of 1 for this, this would bleed over into the mmx register section.
	return baseIndex + rowStartIndex + regSize;

}



void loadRexRegisters(char ** registers){
	
	int index = 0;
	
	index = getRegisterArrayIndex(4,genPurpose,0);
	registers[index] = "SPL";
	index = getRegisterArrayIndex(5,genPurpose,0);
	registers[index] = "BPL";
	index = getRegisterArrayIndex(6,genPurpose,0);
	registers[index] = "SIL";
	index = getRegisterArrayIndex(7,genPurpose,0);
	registers[index] = "DIL";

}

void loadNonRexRegisters(char ** registers){

	int index = 0;
	
	index = getRegisterArrayIndex(4,genPurpose,0);
	registers[index] = "AH";
	index = getRegisterArrayIndex(5,genPurpose,0);
	registers[index] = "CH";
	index = getRegisterArrayIndex(6,genPurpose,0);
	registers[index] = "DH";
	index = getRegisterArrayIndex(7,genPurpose,0);
	registers[index] = "BH";

}



