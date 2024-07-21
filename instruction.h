
#define maxInstructionLength 15 //bytes
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
