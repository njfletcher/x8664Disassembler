#include "instruction.h"

char getOpSizeInfo(instInfo info) return (info.legPreInfo & 0x01);
char getAddrSizeInfo(instInfo info) return ((info.legPreInfo & 0x02) >> 1);
char getSegmentInfo(instInfo info) return ((info.legPreInfo & 0x1C) >> 2);
char getLockInfo(instInfo info) return ((info.legPreInfo & 0x20) >> 5);
char getSeqInfo(instInfo info) return ((info.legPreInfo & 0xC0) >> 6);

void setOpSizeInfo(instInfo* info, int value) info->legPreInfo &= (value & 0x01);
void setAddrSizeInfo(instInfo* info, int value) info->legPreInfo &= ((value << 1) & 0x02);
void setSegmentInfo(instInfo* info, int value) info->legPreInfo &= ((value << 2) & 0x1C);
void setLockInfo(instInfo* info, int value) info->legPreInfo &= (value << 5) & 0x20);
void setSeqInfo(instInfo* info, int value) info->legPreInfo &= (value << 6) & 0xC0);


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
