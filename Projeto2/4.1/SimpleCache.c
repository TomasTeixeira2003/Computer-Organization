#include "SimpleCache.h"

uint8_t L1Cache[L1_SIZE];
uint8_t DRAM[DRAM_SIZE];
uint32_t time;
struct L1Cache L1;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

	if (address >= DRAM_SIZE - WORD_SIZE + 1)	// check that the address is within the memory bounds
		exit(-1);

	if (mode == MODE_READ) {
		memcpy(data, &(DRAM[address]), BLOCK_SIZE);
		time += DRAM_READ_TIME;
	}

	if (mode == MODE_WRITE) {
		memcpy(&(DRAM[address]), data, BLOCK_SIZE);
		time += DRAM_WRITE_TIME;
	}
}

/*********************** L1 cache *************************/

void initCache() { 
	L1.init = 1;
	for(size_t i = 0; i<(L1_SIZE / BLOCK_SIZE); i++){
		L1.lines[i].Valid = 0;
	}
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {

	uint32_t index, Tag, MemAddress, offset;
	uint8_t TempBlock[BLOCK_SIZE];

	Tag = address >> 14;	// Shift 14 bits to the right to get the tag

	MemAddress = address >> 6;	// address of the block without the offset bits
	MemAddress = MemAddress << 6;	// address of the block in memory

	index = (address >> 6) & 0x000000FF; // use a mask to get the index

	offset = address & 0x0000003F;	// use a mask to get the offset

	CacheLine *Line = &L1.lines[index];	// get the line from the cache

	/* access Cache*/

	if (!Line->Valid || Line->Tag != Tag) {         // if block not present || miss
		accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

		if ((Line->Valid) && (Line->Dirty)) {	// line has dirty block
			MemAddress = (Line->Tag << 14) + (index<<6);	// get address of the block in memory
			accessDRAM(MemAddress, &(L1Cache[index*BLOCK_SIZE]), MODE_WRITE);	// then write back old block
		}

		memcpy(&(L1Cache[index*BLOCK_SIZE]), TempBlock, BLOCK_SIZE);	// copy new block to cache line
		Line->Valid = 1;
		Line->Tag = Tag;
		Line->Dirty = 0;
	}	// if miss, then replaced with the correct block

	if (mode == MODE_READ) {	// read data from cache line
		memcpy(data, &(L1Cache[index*BLOCK_SIZE + offset]), WORD_SIZE);
		time += L1_READ_TIME;
	}

	if (mode == MODE_WRITE) {	// write data to cache line
		memcpy(&(L1Cache[index*BLOCK_SIZE + offset]), data, WORD_SIZE);
		time += L1_WRITE_TIME;
		Line->Dirty = 1;
	}
}

void read(uint32_t address, uint8_t *data) {
	accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
	accessL1(address, data, MODE_WRITE);
}
