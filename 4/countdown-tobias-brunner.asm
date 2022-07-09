	# SOLUTIONS
	#
	# -------------------------------------------------------------------------------------
	# Series 4 - MIPS Programming Part 1 - Countdown Clock
	# 
	# Group members:
	# tobias brunner; olivier cathrein
	# 
	# Individualised code by:
	# tobias brunner
	# 
	# Exercise Version:
	# ** denote the exercise version here (1, 2) **
	# 1
	#
	# Notes:
	# We provide hints and guidance in the comments below and
	# strongly encourage you to follow the skeleton.
	# However, you are free to change the code as you like.
	# 
	# -------------------------------------------------------------------------------------

	# Declare main as a global function
	.globl main 
		
	# All memory structures are placed after the .data assembler directive
	.data

	value: .word 60  # this is the initial value of the countdown (seconds)

	start: .asciiz "COUNTDOWN STARTED\n"

	# *************************************************************************************
	# TASK (c): Add the correct hex code for each digit. One byte (8 bits) fully determines
	# the state of one digit in the display. See the exercise sheet for a legend. 
	# *************************************************************************************
	#
	# The .byte assembler directive reserves space
	# in memory for a single byte (or multiple bytes when separated by comma)
	# and assigns that memory location an initial value
	# (or a comma separated list of initial values)
	# 
	# 	      0     1     2     3     4     5     6     7     8     9			
	digits: .byte 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F

	# All program code is placed after the .text assembler directive
	.text 		

	# The label 'main' represents the starting point
	main:
		li      $v0, 4       	# code 4 == print string
		la      $a0, start  	# $a0 == address of the string
		syscall             	# Ask the operating system to print to stdout.
		
		
	# *************************************************************************************
	# TASK (e): Implement the main countdown loop. It should stop when the countdown 
	# reaches zero. From here you should call the subroutines defined below, some of which
	# are already complete and some you have to implement in other tasks.
	# *************************************************************************************
		
		lw $t0, value  		# register t0 holds the countdown value
		
		loop:
			move $a0, $t0		
			jal get_digits          
			
			move $a0, $v0			
			li $a1, 0xFFFF0010	
			jal write_digit

			move $a0, $v1			
			li $a1, 0xFFFF0011	
			jal write_digit

			li $v0, 32
			li $a0, 1000		
			syscall		

			addi, $t0, ,$t0, -1	
			bne $t0, -1, loop	
			
			j blink			# jump to loop loop
		
		# 1. Split the countdown value into its two digits (see get_digits subroutine)
		# 2. Write the digits to the display (see write_digit subroutine)
		# 3. Stall for 1 second
		# 4. Check if countdown runs out and continue looping
		
		
	# *************************************************************************************
	# TASK (f): Alarm signal - create a blinking animation. 
	# *************************************************************************************

		blink:
			add $t0, $zero, $zero

			li $a0, 0x0
			li $a1, 0xFFFF0010
			jal write_digit

			li $a0, 0x0
			li $a1, 0xFFFF0011
			jal write_digit

			li $v0, 32
			li $a0, 300		
			syscall	

			li $a0, 0x3F
			li $a1, 0xFFFF0010
			jal write_digit

			li $a0, 0x3F
			li $a1, 0xFFFF0011
			jal write_digit

			li $v0, 32
			li $a0, 300		
			syscall	

			j blink

		

		
		# 1. Turn display off
		# 2. Wait
		# 3. Turn display on
		# 4. Loop
		
		

		# Exit the program by means of a syscall.
		# The code for exit is "10". 
		li $v0, 10 		# sets $v0 to "10" to select exit syscall
		syscall




	# This is a helper subroutine. It splits an integer into its two individual digits and
	# saves the results in registers v0 and v1.
	# Arguments: 
	#     a0: an integer value 0 <= x < 100
	# Outputs:
	#     v0: right digit (10^0)
	#     v1: left digit (10^1)
	get_digits:
		li $a1, 10 			# Loads immediate value into register
		divu $a0, $a1 		# divide unsigned: divides $a0 by $a1 and stores the quotient in $LO 
							# and the remainder in $HI
		mfhi $v0			# move from HI: The contents of register HI are moved to the specified 
							# register
		mflo $v1 			# move from LO: The contents of register LO are moved to the specified
							# register
		jr $ra 				# jump register: jumps to the return address stored in $ra
		
	# *************************************************************************************
	# TASK (d): Write a digit to the segment display. This subroutine expects two 
	# arguments as input, saved in registers a0 and a1. 
	# 
	# a0 (word): The digit, a number between 0 and 9
	#
	# a1 (byte): This is the address of the segment status buffer to which we want to 
	#	write the segment state. 
	# 	This will either be 0xFFFF0010 (right segment) or 0xFFFF0011 (left segment).
	#
	#
	# Hint 1: Make sure to save any temporary registers that you use here to the stack
	# and restore them again before returning.
	#
	# Hint 2: The register "ra" contains the return address
	#
	# *************************************************************************************

		# 1. Fetch the correct byte pattern depending on digit passed in (see the 
		#    array "digits" defined at the top). 
		#    Hint: You can load an address with the "la" instruction.
		# 2. Save the byte to the memory address in a1. 
		#    Hint: You can store a byte with the "sb" instruction. 
		#          Compare it to the "sw" instruction seen in class.
		# 3. Return.

	write_digit:
		la $t1, digits		
		add $t2, $a0, $t1	
		lb $t3, 0($t2)		
		sb $t3, 0($a1)		
		add  $t1, $zero, $zero	
		add  $t2, $zero, $zero	
		add  $t3, $zero, $zero	
		jr $ra
		
