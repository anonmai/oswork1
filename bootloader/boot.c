#include "boot.h"
#define SECTSIZE 512

void bootMain(void) {
	unsigned int sceno = 1;			//设置要读写的磁盘扇区号为1，即第二个磁盘扇区
	unsigned long va = 0x8c00;		//设置要写入的内存地址为0x8c00
	readSect((void*)va, sceno);		//
	
	asm volatile("jmp 0x8c00");		//跳转到刚刚写入代码的位置执行
	while(1);						//无限循环
}

void waitDisk(void) { // waiting for disk
	while((inByte(0x1F7) & 0xC0) != 0x40);
}

//其中*dst为要写入的内存地址，offset为磁盘扇区号
void readSect(void *dst, int offset) { // reading one sector of disk
	int i;
	waitDisk();
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();

	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}
