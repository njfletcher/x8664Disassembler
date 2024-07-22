
#define maxInstructionLength 15 //bytes
#define illegalInstructionNum -1

#define opSizePre = 0x66;
#define addrSizePre = 0x67;
#define segCSPre = 0x2E;
#define segDSPre = 0x3E;
#define segESPre = 0x26;
#define segFSPre =  0x64;
#define segGSPre = 0x65;
#define segSSPre = 0x36;
#define lockPre = 0xF0;
#define repPre = 0xF3;
#define repezPre = 0xF3; //repe or repz
#define repnezPre = 0xF2; // repne or repnz

typedef struct instructionInfo{

	//lsb to msb: opSizePre(1 bit), addrSizePre(1 bit), segmentPre(7 values: 3 bits), lockPre( 1 bit), repPre(4 values: 2 bits) 
	unsigned char legPreInfo;
	//lsb to msb: rex b(1 bit), rex x(1 bit), rex r(1 bit), rex w(1 bit), unused(4bits)
	unsigned char rexInfo;
	
	//lsb to msb: isXop(1 bit), isThreeByteSequence(1 bit), B( 1 bit unused if two byte seq), X ( 1 bit unused if two byte seq), R ( 1 bit), 
	//pp ( 2 bits), L ( 1 bit), vvvv( 4 bits), W ( 1 bit unused if two byte seq).
	unsigned short int xopVexInfo;







}instInfo;

char getOpSizeInfo(instInfo info);
char getAddrSizeInfo(instInfo info);
char getSegmentInfo(instInfo  info);
char getLockInfo(instInfo info);
char getRepInfo(instInfo info);

void setOpSizeInfo(instInfo* info, int value);
void setAddrSizeInfo(instInfo* info, int value);
void setSegmentInfo(instInfo* info, int value);
void setLockInfo(instInfo* info, int value);
void setRepInfo(instInfo* info, int value);

char legacyPrefixFSM(char* instructionCandidate, instInfo* info);

char isRexPrefix(char byte);

char getRexB(instInfo info);
char getRexX(instInfo info);
char getRexR(instInfo  info);
char getRexW(instInfo info);

void setRexByte(instInfo info, char rexByte);

char normalEscapeSequenceFSM(char* currentByte);

char getMapSelect(char byte);
void setXOPVexInfoThreeByte(instInfo * info, char firstByte, char secondByte);
void setVexInfoTwoByte(instInfo * info, char firstByte);

char queryXOPTables(char opByte, char mapSelect);
char queryVEXTables(char opByte, char mapSelect);

char parseXOPVEXSequence(char isXOP, char isThreeByte, char* currentPos, instInfo* info)

char secondaryPrefixFSM(char * currentPos, instInfo* info)




