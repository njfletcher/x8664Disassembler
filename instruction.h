#ifndef instruction
#define instruction


#define maxInstructionLength 15 //bytes
#define illegalInstructionNum -1

#define opSizePre 0x66
#define addrSizePre 0x67
#define segCSPre 0x2E
#define segDSPre 0x3E
#define segESPre 0x26
#define segFSPre 0x64
#define segGSPre 0x65
#define segSSPre 0x36
#define lockPre 0xF0
#define repPre 0xF3
#define repezPre 0xF3//repe or repz
#define repnezPre 0xF2 // repne or repnz

#define modDirect 0x3


typedef struct instructionInfo{

	//lsb to msb: opSizePre(1 bit), addrSizePre(1 bit), segmentPre(7 values: 3 bits), lockPre( 1 bit), repPre(3 values: 2 bits) 
	unsigned char legPreInfo;
	//lsb to msb: rex b(1 bit), rex x(1 bit), rex r(1 bit), rex w(1 bit), unused(4bits)
	unsigned char rexInfo;
	
	//lsb to msb: isXop(1 bit), isThreeByteSequence(1 bit), B( 1 bit unused if two byte seq), X ( 1 bit unused if two byte seq), R ( 1 bit), 
	//pp ( 2 bits), L ( 1 bit), vvvv( 4 bits), W ( 1 bit unused if two byte seq).
	unsigned short int xopVexInfo;

	
}instInfo;



char getOpSizeInfo(instInfo* info);
char getAddrSizeInfo(instInfo* info);
char getSegmentInfo(instInfo*  info);
char getLockInfo(instInfo* info);
char getRepInfo(instInfo* info);

void setOpSizeInfo(instInfo* info, int value);
void setAddrSizeInfo(instInfo* info, int value);
void setSegmentInfo(instInfo* info, int value);
void setLockInfo(instInfo* info, int value);
void setRepInfo(instInfo* info, int value);

char legacyPrefixFSM(unsigned char* instructionCandidate, instInfo* info);

char isRexPrefix(unsigned char byte);

unsigned char getRexB(instInfo* info);
unsigned char getRexX(instInfo* info);
unsigned char getRexR(instInfo*  info);
unsigned char getRexW(instInfo* info);

void setRexByte(instInfo* info, unsigned char rexByte);

char normalEscapeSequenceFSM(unsigned char* currentByte);

char getMapSelect(unsigned char byte);
void setXOPVexInfoThreeByte(instInfo * info, unsigned char isXOP, unsigned char firstByte, unsigned char secondByte);
void setVexInfoTwoByte(instInfo * info, unsigned char firstByte);

char queryXOPTables(unsigned char opByte, unsigned char mapSelect);
char queryVEXTables(unsigned char opByte, unsigned char mapSelect);

char parseXOPVEXSequence(unsigned char isXOP, unsigned char isThreeByte, unsigned char* currentPos, instInfo* info);


unsigned char getXOPVEXR(instInfo* info);
unsigned char getXOPVEXB(instInfo* info);

unsigned char getXOPVEXQuadV(instInfo* info);
//xop/vex vvvv field is encoded in inverted one's complement form.
unsigned char getXOPVEXQuadVReal(instInfo* info);

	
char secondaryPrefixFSM(unsigned char * currentPos, instInfo* info);


unsigned char getModMod(unsigned char byte);
unsigned char getModReg(unsigned char byte);
unsigned char getModRegFull(unsigned char byte, instInfo* info);
unsigned char getModRM(unsigned char byte);
unsigned char getModRMFull(unsigned char byte, instInfo* info);

unsigned char getSIBScale(unsigned char byte);
char getSIBScaleChar(unsigned char scale);
unsigned char getSIBIndex(unsigned char byte);
unsigned char getSIBIndexFull(unsigned char byte, instInfo* info);
unsigned char getSIBBase(unsigned char byte);
unsigned char getSIBBaseFull(unsigned char byte, instInfo* info);

void getModRMOperandForced(unsigned char isDirectOperand, unsigned char regType, unsigned char regSize, unsigned char isFromRM, unsigned char * modBytePtr, instInfo* info, char * buffer);
void getModMemoryOperand16Bit(unsigned char modVal, unsigned char * modBytePtr, instInfo* info, char * buffer);
void getModMemoryOperandLong(unsigned char modVal, unsigned char * modBytePtr, instInfo * info, unsigned char regSize, char * buffer);
void getModRMOperandUnforced(unsigned char * modBytePtr, unsigned char regSize, unsigned char regType, instInfo* info, char * buffer);
void getSIBOperand(unsigned char * sibBytePtr, unsigned char modValue, unsigned char regSize, instInfo* info, char * buffer);

char * getNextBytesAsString(void * currBytePtr, unsigned char scale);







#endif

