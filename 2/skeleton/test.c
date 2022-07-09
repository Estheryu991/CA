/* TODO: Task (b) Please fill in the following lines, then remove this line.
 *
 * author(s):   Tobias Brunner
 *              Oliver Cathrein
 * modified:    2010-01-07
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mips.h"

/* executes exactly the given instrution */
void test_execute(word instr) {
	word w;
	Instruction *instruction;
	
	/* Store the executable word  */
	storeWord(instr, pc);
	
	/* Fetch the next Instruction */
	w  = loadWordFrom(pc);
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

/* JAL */
void test_jal() {
      	int pcSaved;
        word w;
        Instruction* instruction;

        pc = 0x00000000;
        pcSaved = pc;
	    test_execute(create_jtype_hex(0x0001, OC_JAL));
	    assert(RA == pcSaved + 4);
        assert(pc == 4);

        /* The following test is executed manually as the desired pc is outside the memory,
         * i.e. the test needs to bypass actually storing the instruction in the memory.
         */
	    initialize();
        pc = 0xAF000000;
	    pcSaved = pc;
        w = create_jtype_hex(0x0001, OC_JAL);
        instruction = (Instruction *) &w;
        pc += 4;
       	operations[instruction->i.opcode].operation(instruction);
	    assert(RA == pcSaved + 4);
        assert(pc == 0xA0000004);
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
    /* TODO: Task (d) add test for LW here */
    word w1 = 0xAAAAAAAA;
    word w2 = 0xBBBBBBBB;

    word location1 = 0x00000102;
    word location2 = 0x00000096;

    T0 = location1;
    storeWord(w1, T0);
    test_execute(create_itype_hex(0, I_T1, I_T0, OC_LW));
    assert(T1 == w1);

    T1 = location2;
    storeWord(w2, T1);
    test_execute(create_itype_hex(0, I_T2, I_T1, OC_LW));
    assert(T2 == w2);
} 

/* ORI */
void test_ori() {
    /* TODO: Task (d) add test for ORI here */
    word w1 = 0x11110000;
    word w2 = 0x00110101;

    unsigned immediate1 = 0x00001111;
    unsigned immediate2 = 0x10001100;

    T0 = w1;
    test_execute(create_itype_hex(immediate1, I_T1, I_T0, OC_ORI));
    assert(T1 == 0x11111111);

    T1 = w2;
    test_execute(create_itype_hex(immediate2, I_T2, I_T1, OC_ORI));
    assert(T2 == 0x10111101);
}

/* SUB */
void test_sub() {
    /* TODO: Task (d) add test for SUB here */
    T0 = 1;
    T1 = -1;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T2, I_T1, I_T0, OC_SUB));
    assert(T2 == 2);

    T3 = -4;
    T4 = 6;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T5, I_T3, I_T4, OC_SUB));
    assert(T5 == 10);

    T6 = 4;
    T7 = -6;
    test_execute(create_rtype_hex(FC_SUB, 0x0000, I_T8, I_T6, I_T7, OC_SUB));
    assert(T8 == -10);
}

/* SW */
void test_sw() {
    word location1 = 0x00001000;
    word location2 = 0x00001004;
	
    word w = 0xFFFFFFFF;
    T0 = w;
    T1 = location1;
    test_execute(create_itype_hex(0x0000, I_T0, I_T1, OC_SW));
    assert(loadWordFrom(location1) == w);
	
    w =0x12345678;
    T0 = w;
    T1 = location2;
    test_execute(create_itype_hex(0xFFFC, I_T0, I_T1, OC_SW));
    assert(loadWordFrom(location1) == w);
}

/* ============================================================================ */
/* make sure you've got a "fresh" environment for every test */
void execute_test(void (*test)(void)) {
    initialize();
    test();
}

/* executes all tests */
int main (int argc, const char * argv[]) {
	execute_test(&test_add);
	execute_test(&test_addi);
	execute_test(&test_jal);
	execute_test(&test_lui);
	execute_test(&test_lw);
	execute_test(&test_ori);
	execute_test(&test_sub);
	execute_test(&test_sw);
	return 0;
}
