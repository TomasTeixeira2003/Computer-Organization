#include "SimpleCache.h"

uint8_t L1Cache[L1_SIZE];
uint8_t L2Cache[ASSOCIATIVITY_L2][L2_SIZE/ASSOCIATIVITY_L2];
uint8_t DRAM[DRAM_SIZE];
uint32_t time;
struct L1Cache L1;
struct L2Cache L2;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

    if (address >= DRAM_SIZE - WORD_SIZE + 1)
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
	L2.init = 1;
	for(size_t i = 0; i<(L1_SIZE / BLOCK_SIZE); i++){
		L1.lines[i].Valid = 0;
	}
	for(size_t i = 0; i<((L2_SIZE / BLOCK_SIZE) / ASSOCIATIVITY_L2); i++){
		L2.lines[i][0].Valid = 0;
		L2.lines[i][1].Valid = 0;
	}
}

void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {

	uint32_t index, Tag, MemAddress;
	uint8_t TempBlock[BLOCK_SIZE];
	int ix_block; // index of the block that is going to be used

	Tag = address >> 14; //Shift 14 bits to the right to get the tag

	MemAddress = address >> 6; // address of the block without the offset bits
	MemAddress = MemAddress << 6; // address of the block in memory

	index = (address >> 6) & 0x000000FF;	// use a mask to get the index

	L2CacheLine *block1 = &L2.lines[index][0], *block2 = &L2.lines[index][1];	// blocks from the same set

	/* access Cache*/

	if (!block1->Valid || block1->Tag != Tag) {	// if block not present - miss
		if(!block2->Valid || block2->Tag != Tag){
			if(block1->lru){
				accessDRAM(MemAddress, TempBlock, MODE_READ);	// get new block from DRAM

				if ((block1->Valid) && (block1->Dirty)) {	// line has dirty block
					MemAddress = (block1->Tag << 14) + (index<<6);	// get address of the block in memory
					accessDRAM(MemAddress, &(L2Cache[0][index*BLOCK_SIZE]), MODE_WRITE);	// then write back old block
				}

				memcpy(&(L2Cache[0][index*BLOCK_SIZE]), TempBlock, BLOCK_SIZE);	// copy new block to cache line
				block1->Valid = 1;
				block1->Tag = Tag;
				block1->Dirty = 0;
				block1->lru = 0;
				block2->lru = 1;
				ix_block = 0;
			}
			else{	
				accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

				if ((block2->Valid) && (block2->Dirty)) { // line has dirty block
					MemAddress = (block1->Tag << 14) + (index<<6);	// get address of the block in memory
					accessDRAM(MemAddress, &(L2Cache[1][index*BLOCK_SIZE]), MODE_WRITE);	// then write back old block
				}

				memcpy(&(L2Cache[1][index*BLOCK_SIZE]), TempBlock, BLOCK_SIZE);	// copy new block to cache line
				block2->Valid = 1;
				block2->Tag = Tag;
				block2->Dirty = 0;
				block2->lru = 0;
				block1->lru = 1;
				ix_block = 1;
			}
		}
		else{	// block2 is valid and has the same tag
			ix_block = 1;
			block2->lru = 0;
			block1->lru = 1;
		}
	} // if miss, then replaced with the correct block
	else{	// block1 is valid and has the same tag
		ix_block = 0;
		block2->lru = 1;
		block1->lru = 0;
	}

	if (mode == MODE_READ) {    // read data from cache line
		memcpy(data, &(L2Cache[ix_block][index*BLOCK_SIZE]), BLOCK_SIZE);
		time += L2_READ_TIME;
	}

	if (mode == MODE_WRITE) { // write data to cache line
		memcpy(&(L2Cache[ix_block][index*BLOCK_SIZE]), data, BLOCK_SIZE);
		time += L2_WRITE_TIME;
		if(ix_block)
			block2->Dirty = 1;
		else block1->Dirty = 1;
	}
}


void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {

	uint32_t index, Tag, MemAddress, offset;
	uint8_t TempBlock[BLOCK_SIZE];

	Tag = address >> 14; // Why do I do this?

	MemAddress = address >> 6; // again this....!
	MemAddress = MemAddress << 6; // address of the block in memory

	index = (address >> 6) & 0x000000FF;

	offset = 	address & 0x0000003F;

	CacheLine *Line = &L1.lines[index];

	/* access Cache*/

	// L1 MISS
	if (!Line->Valid || Line->Tag != Tag) {         // if block not present - miss
		accessL2(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

		if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
			MemAddress = (Line->Tag << 14) +(index<<6);        // get address of the block in memory
			accessL2(MemAddress, &(L1Cache[index*BLOCK_SIZE]), MODE_WRITE); // then write back old block
		}

		memcpy(&(L1Cache[index*BLOCK_SIZE]), TempBlock, BLOCK_SIZE); // copy new block to cache line
		Line->Valid = 1;
		Line->Tag = Tag;
		Line->Dirty = 0;
	} // if miss, then replaced with the correct block

	// L1 HIT

	if (mode == MODE_READ) {    // read data from cache line
		memcpy(data, &(L1Cache[index*BLOCK_SIZE + offset]), WORD_SIZE);
		time += L1_READ_TIME;
	}

	if (mode == MODE_WRITE) { // write data from cache line
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
