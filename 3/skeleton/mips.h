#ifndef MIPS_H
#define MIPS_H

#include <stdint.h>

/* Always useful */
#define TRUE 1
#define FALSE 0

/* Define abbreviations for the indices of the registers */
#define I_ZERO 0
#define I_AT 1
#define I_V0 2
#define I_V1 3
#define I_A0 4
#define I_A1 5
#define I_A2 6
#define I_A3 7
#define I_T0 8
#define I_T1 9
#define I_T2 10
#define I_T3 11
#define I_T4 12
#define I_T5 13
#define I_T6 14
#define I_T7 15
#define I_S0 16
#define I_S1 17
#define I_S2 18
#define I_S3 19
#define I_S4 20
#define I_S5 21
#define I_S6 22
#define I_S7 23
#define I_T8 24
#define I_T9 25
#define I_K0 26
#define I_K1 27
#define I_GP 28
#define I_SP 29
#define I_FP 30
#define I_RA 31

/* Define abbreviations for the registers */
#define ZERO registers[I_ZERO]
#define AT registers[I_AT]
#define V0 registers[I_V0]
#define V1 registers[I_V1]
#define A0 registers[I_A0]
#define A1 registers[I_A1]
#define A2 registers[I_A2]
#define A3 registers[I_A3]
#define T0 registers[I_T0]
#define T1 registers[I_T1]
#define T2 registers[I_T2]
#define T3 registers[I_T3]
#define T4 registers[I_T4]
#define T5 registers[I_T5]
#define T6 registers[I_T6]
#define T7 registers[I_T7]
#define S0 registers[I_S0]
#define S1 registers[I_S1]
#define S2 registers[I_S2]
#define S3 registers[I_S3]
#define S4 registers[I_S4]
#define S5 registers[I_S5]
#define S6 registers[I_S6]
#define S7 registers[I_S7]
#define T8 registers[I_T8]
#define T9 registers[I_T9]
#define K0 registers[I_K0]
#define K1 registers[I_K1]
#define GP registers[I_GP]
#define SP registers[I_SP]
#define FP registers[I_FP]
#define RA registers[I_RA]

/* Define abbreviations for the operation codes (OC) and function codes (FC) */
#define OC_ADD		0x00
#define FC_ADD		0x20
#define OC_ADDI		0x08
#define OC_ADDIU	0x09
#define OC_ADDU		0x00
#define FC_ADDU		0x21
#define OC_AND		0x00
#define FC_AND		0x24
#define OC_ANDI		0x0C
#define OC_BEQ		0x04
#define OC_BNE		0x05
#define OC_DIV		0x00
#define FC_DIV		0x1A
#define OC_J		0x02
#define OC_JAL		0x03
#define OC_JR		0x00
#define FC_JR		0x08
#define OC_LBU		0x00
#define FC_LBU		0x24
#define OC_LHU		0x00
#define FC_LHU		0x25
#define OC_LUI		0x0F
#define OC_LW		0x23
#define OC_MULT		0x00
#define FC_MULT		0x18
#define OC_NOR		0x00
#define FC_NOR		0x27
#define OC_OR		0x00
#define FC_OR		0x25
#define OC_ORI		0x0D
#define OC_SLT		0x00
#define FC_SLT		0x2A
#define OC_SLTI		0x0A
#define OC_SLTIU	0x0B
#define OC_SLTU		0x00
#define FC_SLTU		0x2B
#define OC_SLL		0x00
#define FC_SLL		0x00
#define OC_SRL		0x00
#define FC_SRL		0x02
#define OC_SB		0x28
#define OC_SH		0x29
#define OC_SW		0x2B
#define OC_SUB		0x00
#define FC_SUB		0x22
#define OC_SUBU		0x00
#define FC_SUBU		0x23
/* Just a handy abbreviation */
#define OC_ZERO 0x00
/* Stop Operation --- not actual MIPS */
#define OC_STOP 0x3F

#define OPERATION_COUNT 64
#define FUNCTION_COUNT 64
#define REGISTER_COUNT 32

/* Maximal length of operation and function names */
#define OP_NAME_LENGTH 4
#define FUNC_NAME_LENGTH 4


#define ERROR(...) error(__func__, __FILE__, __LINE__, __VA_ARGS__);

/* Define some types */
typedef uint32_t word;
typedef uint16_t halfword;
typedef uint8_t byte;

/* Create the InstructionType structs as bitfields with the given sizes from
 * the Script p. 5 / p. 9 .
 * Bit 0 is at the first position the struct. */
typedef struct {
	unsigned immediate : 16;
	unsigned rt : 5;
	unsigned rs : 5;
	unsigned opcode : 6;
} InstructionTypeI;

typedef struct {
	unsigned address : 26;
	unsigned opcode : 6;
} InstructionTypeJ;

typedef struct {
	unsigned funct : 6;
	unsigned shamt : 5;
	unsigned rd : 5;
	unsigned rt : 5;
	unsigned rs : 5;
	unsigned opcode : 6;
} InstructionTypeR;

typedef union {
	InstructionTypeI i;
	InstructionTypeJ j;
	InstructionTypeR r;
} Instruction;

typedef enum {iType, jType, rType, specialType} InstructionType;

typedef struct {
    char name[5];
    InstructionType type;
    void (*operation)(Instruction*);
} Operation;

typedef struct {
    char name[5];
    void (*function)(Instruction*);
} Function;

/* Functions to initialize and run the MIPS machine */

void initialize();
void run();


/* Boolean to stop the MIPS machine */
extern int doRun;

/* In case you want to watch the machine working */
extern int verbose;

/* The "Hardware" */
extern word registers[REGISTER_COUNT];
extern word pc;

/* Operation and function dispatcher */
extern Operation operations[OPERATION_COUNT];
extern Function functions[FUNCTION_COUNT];

/* Function declarations for helper functions */
void error(const char*, const char*, int , char*, ...);

word create_itype_hex(unsigned short, unsigned short, unsigned short, unsigned short);
word create_jtype_hex(unsigned long, unsigned short);
word create_rtype_hex(unsigned short, unsigned short, unsigned short, unsigned short, unsigned short, unsigned short);

word signExtend(halfword);
word zeroExtend(halfword);

void undefinedOperation(Instruction*);
void undefinedFunction(Instruction*);
void opCodeZeroOperation(Instruction*);
void stopOperation(Instruction*);

void mips_add(Instruction*);
void mips_addi(Instruction*);
void mips_div(Instruction*);
void mips_lui(Instruction*);
void mips_lw(Instruction*);
void mips_mult(Instruction*);
void mips_ori(Instruction*);
void mips_sub(Instruction*);
void mips_sw(Instruction*);
#endif /* MIPS_H */
