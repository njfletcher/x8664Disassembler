#include "register.h"

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
