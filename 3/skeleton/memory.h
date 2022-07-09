#ifndef MEMORY_H
#define MEMORY_H

#include "mips.h"

/* ========================================================================== */

/* in honour of C64 */
#define RAM_SIZE 1024*64
#define MEMORY_REGION_SIZE 256
#define FPRINTF_MEMORY_LOCATION 0x10000
#define TIME_MEMORY_LOCATION 0x10001

/* ========================================================================== */

typedef struct {
	int min;
	int max;
    void (*initialize)();
	void (*write)(word address, byte value);
	byte (*read)(word address);
} MemoryRegion;


/* ========================================================================== */

extern byte defaultMemoryData[RAM_SIZE];
extern FILE * outputStream;

void initializeMemory(void);

/* MEMORY FUNCTIONS ========================================================== */

void loadFile(char*);

void storeWord(word, word);
word loadWord(word);

/* ========================================================================== */

#endif /* MEMORY_H */

