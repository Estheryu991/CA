/* TODO: Task (b) Please fill in the following lines, then remove this line.
 *
 * author(s):   Tobias Brunner
 *              Olivier Cathrein
 * modified:    2010-01-07
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "memory.h"

MemoryRegion * memoryRegions[MEMORY_REGION_SIZE];
FILE * outputStream;

void emptyMemoryRegionInitializer() {}

/* ========================================================================== */
/* DEFAULT MEMORY */
byte defaultMemoryData[RAM_SIZE];

void defaultMemoryInitializer() {
	int i;
	for (i=0; i<RAM_SIZE; ++i) {
		defaultMemoryData[i] = 0;
	}
}

void defaultMemoryWriteHandler(word address, byte value) {
	defaultMemoryData[address] = value;
}

byte defaultMemoryReadHandler(word address) {
	return defaultMemoryData[address];
}

MemoryRegion defaultMemory = {
    0, 
    RAM_SIZE-1, 
    &defaultMemoryInitializer,
    &defaultMemoryWriteHandler, 
    &defaultMemoryReadHandler
};

/* NEW Macro */

/* ========================================================================== */
/* printf memory */
void fprintfMemoryRegionInitializer() {
	outputStream = stdout;
}

void fprintfMemoryWriteHandler(word address, byte value) {
        fprintf(outputStream, "%c", value);
}

byte fprintfMemoryReadHandler(word address) {
        /* ignore reads, only used to output */
	return 0;
}

MemoryRegion fprintfMemory = {
    FPRINTF_MEMORY_LOCATION, 
    FPRINTF_MEMORY_LOCATION, 
    &fprintfMemoryRegionInitializer,
    &fprintfMemoryWriteHandler, 
    &fprintfMemoryReadHandler
};

/* ========================================================================== */
/* time memory */

void timeMemorWriteHandler(word address, byte value) {}

byte timeMemoryReadHandler(word address) {
	return clock() * 1000 / CLOCKS_PER_SEC;
}

MemoryRegion timeMemory = {
    TIME_MEMORY_LOCATION, 
    TIME_MEMORY_LOCATION, 
    &emptyMemoryRegionInitializer,
    &timeMemorWriteHandler, 
    &timeMemoryReadHandler
};

/* ========================================================================== */

int checkMemoryRegionOverlap(MemoryRegion *region) {
   	int i;
	for (i=0; i<MEMORY_REGION_SIZE; ++i) {
        if (!memoryRegions[i]) {
            break;
        }
		if (((memoryRegions[i]->min >= region->min) && ( region->min <= memoryRegions[i]->max))
            || ((memoryRegions[i]->min >= region->max) && (region->max <= memoryRegions[i]->max))) {
			return FALSE;
		}
	}
    return TRUE;
}

int memoryRegionCounter = 0;

void registerMemoryRegion(MemoryRegion *region) {
    if (checkMemoryRegionOverlap(region)) {
        memoryRegions[memoryRegionCounter] = region;
        memoryRegionCounter++;
        region->initialize();
    } else {
        ERROR("Overlapping memory regions");
    }
}

void initializeMemory() {
    int i;
    memoryRegionCounter = 0;
    for (i=0; i<MEMORY_REGION_SIZE; ++i) {
        memoryRegions[i] = NULL;
    }
    registerMemoryRegion(&defaultMemory);
    registerMemoryRegion(&fprintfMemory);
    registerMemoryRegion(&timeMemory);
}

/* MEMORY FUNCTIONS ========================================================= */

/* Load a file to memory */
void loadFile(char* filename) {
    /* TODO: Task (d) implement loadFile */
    FILE* file = fopen(filename,"r");
    word wordlocation = 0;
    word word = 0;
    byte buf[4];

    while(!feof(file)){

        fread(buf, 1, 4, file);
        word = buf[3];
        word += buf[2] << 1*8;
        word += buf[1] << 2*8;
        word += buf[0] << 3*8;

        storeWord(word, wordlocation);
        wordlocation += 4;
    }
}

/* ========================================================================== */

MemoryRegion* getMemoryRegion(word location) {
    int i;
    MemoryRegion *memoryRegion = NULL;
    for (i=0; i<MEMORY_REGION_SIZE && !memoryRegion; ++i) {
        if (memoryRegions[i] && memoryRegions[i]->min <= location && memoryRegions[i]->max >= location) {
	    memoryRegion = memoryRegions[i];
	}
    }
    if (!memoryRegion) {
        ERROR("Invalid memory access at 0x%x", (int) location);
    }
    return memoryRegion;
}


/* Store a word to memory */
void storeWord(word w, word location) {
	MemoryRegion *memoryRegion = getMemoryRegion(location);
	memoryRegion->write(location,   (w >> (8*3)) & 0xFF);
	memoryRegion->write(location+1, (w >> (8*2)) & 0xFF);
	memoryRegion->write(location+2, (w >> (8*1)) & 0xFF);
	memoryRegion->write(location+3, (w >> (8*0)) & 0xFF);
}

/* Load a word from memory */
word loadWord(word location) {
	MemoryRegion *memoryRegion = getMemoryRegion(location);
	word w = 0;
	w += (memoryRegion->read(location)   << (8*3));
	w += (memoryRegion->read(location+1) << (8*2));
	w += (memoryRegion->read(location+2) << (8*1));
	w += (memoryRegion->read(location+3) << (8*0));
	return w;
}
