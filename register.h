#define genPurpose 0
#define x87 1
#define mmx 2
#define xmm 3
#define ymm 4
#define seg 5
#define control 6
#define debug 7

int getRegisterArrayIndex(unsigned char regVal, unsigned char regType, unsigned char regSize);
