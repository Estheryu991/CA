#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "memory.h"
#include "mips.h"
#include "compiler.h"

#define TESTFILE "mipsTestFile"

void test_execute(word instr) {
	word w;
	Instruction *instruction;
	
	/* Store the executable word  */
	storeWord(instr, pc);
	
	/* Fetch the next Instruction */
	w  = loadWord(pc);
	instruction = (Instruction *) &w;
	pc += 4;
	
	/* Execute the fetched instruction*/
	operations[instruction->i.opcode].operation(instruction);
	assert(ZERO == 0);
}

/* ADD */
void test_add() {
	T1=1;
	T2=1;
	test_execute(create_rtype_hex(FC_ADD, 0x0000, I_T0, I_T1, I_T2, OC_ADD));
	assert(T0==2);
	
	T1=1;
	T2=-1;
	test_execute(create_rtype_hex(FC_ADD, 0x0000, I_T0, I_T1, I_T2, OC_ADD));
	assert(T0==0);
	
	T1=-1;
	T2=-1;
	test_execute(create_rtype_hex(FC_ADD, 0x0000, I_T0, I_T1, I_T2, OC_ADD));
	assert(T0==-2);
}


/* ADDI */
void test_addi() {
	test_execute(create_itype_hex(0xFFFF, I_T0, I_ZERO, OC_ADDI));
	assert(T0 == -1); 
	test_execute(create_itype_hex(1, I_T0, I_T0, OC_ADDI));
	assert(T0 ==  0);
	
	test_execute(create_itype_hex(0xFFFF, I_T0, I_ZERO, OC_ADDI));
	assert(T0 == -1); 
	test_execute(create_itype_hex(0xFFFF, I_T0, I_T0, OC_ADDI));
	assert(T0 == -2); 
	
	test_execute(create_itype_hex(3, I_T0, I_ZERO, OC_ADDI));
	assert(T0 ==  3);
	test_execute(create_itype_hex(1, I_T1, I_T0, OC_ADDI));
	assert(T0 ==  3);
	assert(T1 ==  4);
}


/* DIV */
void test_div() {
    T1=4;
    T2=8;
    test_execute(create_rtype_hex(FC_DIV, 0x0000, I_T0, I_T1, I_T2, OC_DIV));
    assert(T0==2);
	
    T1=4;
    T2=5;
    test_execute(create_rtype_hex(FC_DIV, 0x0000, I_T0, I_T1, I_T2, OC_DIV));
    assert(T0==1);
	
    T1=5;
    T2=4;
    test_execute(create_rtype_hex(FC_DIV, 0x0000, I_T0, I_T1, I_T2, OC_DIV));
    assert(T0==0);
	
    T1=4;
    T2=-8;
    test_execute(create_rtype_hex(FC_DIV, 0x0000, I_T0, I_T1, I_T2, OC_DIV));
    assert(T0==-2);
	
    T1=-2;
    T2=8;
    test_execute(create_rtype_hex(FC_DIV, 0x0000, I_T0, I_T1, I_T2, OC_DIV));
    assert(T0==-4);
	
    T1=-8;
    T2=-8;
    test_execute(create_rtype_hex(FC_DIV, 0x0000, I_T0, I_T1, I_T2, OC_DIV));
    assert(T0==1);
	
    T1=5;
    T2=-8;
    test_execute(create_rtype_hex(FC_DIV, 0x0000, I_T0, I_T1, I_T2, OC_DIV));
    assert(T0==-1);
}

/* LUI */
void test_lui() {
    test_execute(create_itype_hex(0xFFFF, I_T0, I_ZERO, OC_LUI));
    assert(T0 == 0xFFFF0000);
    
    test_execute(create_itype_hex(0x0001, I_T0, I_ZERO, OC_LUI));
    assert(T0 == 0x00010000);
}

/* LW */
void test_lw() {
    word location1 =  0x00001230;
    word location2 =  0x00001234;
	
    word w = 0xFFFFFFFF;
    storeWord(w, location1);
    T1 = location1;
    test_execute(create_itype_hex(0x0000, I_T0, I_T1, OC_LW));
    assert(T0 == w);
	
    w = 0x87654321;
    storeWord(w, location2);
    T1 = location1;
    test_execute(create_itype_hex(0x0004, I_T0, I_T1, OC_LW));
    assert(T0 == w);
} 

/* MULT */
void test_mult() {
    T1=2;
    T2=3;
    test_execute(create_rtype_hex(FC_MULT, 0x0000, I_T0, I_T1, I_T2, OC_MULT));
    assert(T0==6);
	
    T1=-2;
    T2=3;
    test_execute(create_rtype_hex(FC_MULT, 0x0000, I_T0, I_T1, I_T2, OC_MULT));
    assert(T0==-6);

    T1=2;
    T2=-3;
    test_execute(create_rtype_hex(FC_MULT, 0x0000, I_T0, I_T1, I_T2, OC_MULT));
    assert(T0==-6);
	
    T1=-2;
    T2=-3;
    test_execute(create_rtype_hex(FC_MULT, 0x0000, I_T0, I_T1, I_T2, OC_MULT));
    assert(T0==6);
}

/* ORI */
void test_ori() {
    T0 = 0xFFFF0000;
    test_execute(create_itype_hex(0x0000FFFF, I_T0, I_T0, OC_ORI));
    assert(T0 == 0xFFFFFFFF);
	
    T0 = 0xF0F0F0F0;
    test_execute(create_itype_hex(0x0000FFF0, I_T0, I_T0, OC_ORI));
    assert(T0 == 0xF0F0FFF0);
	
}

/* SUB */
void test_sub() {
    T1=2;
    T2=2;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T0, I_T1, I_T2, OC_SUB));
    assert(T0==0);
	
    T1=1;
    T2=2;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T0, I_T1, I_T2, OC_SUB));
    assert(T0==1);
	
    T1=2;
    T2=1;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T0, I_T1, I_T2, OC_SUB));
    assert(T0==-1);
	
    T1=1;
    T2=-2;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T0, I_T1, I_T2, OC_SUB));
    assert(T0==-3);
	
	
    T1=-1;
    T2=-2;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T0, I_T1, I_T2, OC_SUB));
    assert(T0==-1);
}

/* SW */
void test_sw() {
    word location1 = 0x00001000;
    word location2 = 0x00001004;
	
    word w = 0xFFFFFFFF;
    T0 = w;
    T1 = location1;
    test_execute(create_itype_hex(0x0000, I_T0, I_T1, OC_SW));
    assert(loadWord(location1) == w);
	
    w =0x12345678;
    T0 = w;
    T1 = location2;
    test_execute(create_itype_hex(0xFFFC, I_T0, I_T1, OC_SW));
    assert(loadWord(location1) == w);
}

/* printf memory */
void test_printf_memory_write() {
        byte buf[4];
        word w = 'a' << 24 | 'b'<<16 | 'c'<<8 | '\n';

	/* set up a testfile in write mode */
	FILE *file = fopen(TESTFILE,"w");
        assert(file);

        /* redirect output of fprintf memory to this file */
        outputStream = file;
	
        /* write the string "abc" to fprintf */
	S2 = FPRINTF_MEMORY_LOCATION;
	S1 = w;
	test_execute(create_itype_hex(0x000, I_S1, I_S2, OC_SW));
        fclose(file);

        /* check if the output was "abc" */
	file = fopen(TESTFILE,"r");
        assert(file);
	fread(buf, 1, 4, file);
	fclose(file);
        assert(buf[0] == 'a');
        assert(buf[1] == 'b');
        assert(buf[2] == 'c');
        assert(buf[3] == '\n');
}

/* Compiler */
void test_compiler(char *expr, long eval) {
    initialize();
    compiler(expr,TESTFILE);
    loadFile(TESTFILE);
    run();
    assert(loadWord(SP) == eval);
}

/* ============================================================================ */

void execute_test(void (*test)(void)) {
    initialize();
    test();
}


int main (int argc, const char * argv[]) {
    execute_test(&test_add);
    execute_test(&test_addi);
    execute_test(&test_div);
    execute_test(&test_lui);
    execute_test(&test_lw);
    execute_test(&test_mult);
    execute_test(&test_ori);
    execute_test(&test_sub);
    execute_test(&initialize);
    execute_test(&test_sw);
    execute_test(&test_printf_memory_write);
    test_compiler("1+1", (1+1));
    test_compiler("(3*(45+6))+12", ((3*(45+6))+12));
    test_compiler("5*( 4-3)  /(7- 5* (8+3/2))", (5*(4-3)/(7-5*(8+3/2))));        
    return 0;
}
