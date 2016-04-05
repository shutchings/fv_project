/* Author: Steve Hutchings
 * Date: 1/21/205
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h> 
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/*********************************************************************
 *
 * These C functions use patterns and functionality often found in
 * operating system code. Your job is to implement them. Of course you
 * should write test cases. However, do not hand in your test cases
 * and (especially) do not hand in a main() function since it will
 * interfere with our tester.
 *
 * Additional requirements on all functions you write:
 *
 * - you may not refer to any global variables
 *
 * - you may not call any functions except those specifically
 *   permitted in the specification
 *
 * - your code must compile successfully on CADE lab Linux
 *   machines when using:
 *
 * /usr/bin/gcc -O2 -fmessage-length=0 -pedantic-errors -std=c99 -Werror -Wall -Wextra -Wwrite-strings -Winit-self -Wcast-align -Wcast-qual -Wpointer-arith -Wstrict-aliasing -Wformat=2 -Wmissing-include-dirs -Wno-unused-parameter -Wshadow -Wuninitialized -Wold-style-definition -c assign1.c 
 *
 * NOTE 1: Some of the specifications below are specific to 64-bit
 * machines, such as those found in the CADE lab.  If you choose to
 * develop on 32-bit machines, some quantities (the size of an
 * unsigned long and the size of a pointer) will change. Since we will
 * be grading on 64-bit machines, you must make sure your code works
 * there.
 *
 * NOTE 2: You should not need to include any additional header files,
 * but you may do so if you feel that it helps.
 *
 * HANDIN: submit your finished file, still called assign.c, in Canvas
 *
 *
 *********************************************************************/

#define BYTE_0_MASK	0x00000000000000FF
#define BYTE_1_MASK     0x000000000000FF00
#define BYTE_2_MASK	0x0000000000FF0000
#define BYTE_3_MASK	0x00000000FF000000
#define BYTE_4_MASK	0x000000FF00000000
#define BYTE_5_MASK	0x0000FF0000000000
#define BYTE_6_MASK	0x00FF000000000000
#define BYTE_7_MASK	0xFF00000000000000

#define NIBBLE_0_MASK	0x000000000000000F
#define NIBBLE_1_MASK   0x00000000000000F0
#define NIBBLE_2_MASK	0x0000000000000F00
#define NIBBLE_3_MASK	0x000000000000F000
#define NIBBLE_4_MASK	0x00000000000F0000
#define NIBBLE_5_MASK	0x0000000000F00000
#define NIBBLE_6_MASK	0x000000000F000000
#define NIBBLE_7_MASK	0x00000000F0000000
#define NIBBLE_8_MASK	0x0000000F00000000
#define NIBBLE_9_MASK   0x000000F000000000
#define NIBBLE_10_MASK	0x00000F0000000000
#define NIBBLE_11_MASK	0x0000F00000000000
#define NIBBLE_12_MASK	0x000F000000000000
#define NIBBLE_13_MASK	0x00F0000000000000
#define NIBBLE_14_MASK	0x0F00000000000000
#define NIBBLE_15_MASK	0xF000000000000000

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 * 
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 *
 *********************************************************************/

unsigned long byte_sort (unsigned long arg)
{
	unsigned long result = 0;
	unsigned char bytes[8];
	int i = 0;
	int j = 0;

	int min = 0;
	int temp = 0;

	// Byte shift input into individual bytes
	bytes[0] = (arg & BYTE_0_MASK) >> (8 * 0);
	bytes[1] = (arg & BYTE_1_MASK) >> (8 * 1);
	bytes[2] = (arg & BYTE_2_MASK) >> (8 * 2);
	bytes[3] = (arg & BYTE_3_MASK) >> (8 * 3);
	bytes[4] = (arg & BYTE_4_MASK) >> (8 * 4);
	bytes[5] = (arg & BYTE_5_MASK) >> (8 * 5);
	bytes[6] = (arg & BYTE_6_MASK) >> (8 * 6);
	bytes[7] = (arg & BYTE_7_MASK) >> (8 * 7);

	// Sort individual bytes (albeit slowly...)
	for(i = 0; i < 8; i++) {

		min = i;

		for(j = i + 1; j < 8; j++) {
			if(bytes[j] < bytes[min]) {
				min = j;
			}
		}

		// Switch the current value with the minimum found
		temp = bytes[i];
		bytes[i] = bytes[min];
		bytes[min] = temp;
	}

	// Put the bytes back into a long
	result = (((unsigned long) bytes[7]) << (8 * 7)) |
			 (((unsigned long) bytes[6]) << (8 * 6)) |
			 (((unsigned long) bytes[5]) << (8 * 5)) |
			 (((unsigned long) bytes[4]) << (8 * 4)) | 
			 (((unsigned long) bytes[3]) << (8 * 3)) |
			 (((unsigned long) bytes[2]) << (8 * 2)) |
			 (((unsigned long) bytes[1]) << (8 * 1)) |
			 (((unsigned long) bytes[0]) << (8 * 0));

  	return result;
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 * 
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

unsigned long nibble_sort (unsigned long arg)
{
  	unsigned long result = 0;
	unsigned char nibbles[16];
	int i = 0;
	int j = 0;

	int min = 0;
	int temp = 0;

	// Shift input into individual nibbles
	nibbles[0]  = (arg & NIBBLE_0_MASK) >> (4 * 0);
	nibbles[1]  = (arg & NIBBLE_1_MASK) >> (4 * 1);
	nibbles[2]  = (arg & NIBBLE_2_MASK) >> (4 * 2);
	nibbles[3]  = (arg & NIBBLE_3_MASK) >> (4 * 3);
	nibbles[4]  = (arg & NIBBLE_4_MASK) >> (4 * 4);
	nibbles[5]  = (arg & NIBBLE_5_MASK) >> (4 * 5);
	nibbles[6]  = (arg & NIBBLE_6_MASK) >> (4 * 6);
	nibbles[7]  = (arg & NIBBLE_7_MASK) >> (4 * 7);
	nibbles[8]  = (arg & NIBBLE_8_MASK) >> (4 * 8);
	nibbles[9]  = (arg & NIBBLE_9_MASK) >> (4 * 9);
	nibbles[10] = (arg & NIBBLE_10_MASK) >> (4 * 10);
	nibbles[11] = (arg & NIBBLE_11_MASK) >> (4 * 11);
	nibbles[12] = (arg & NIBBLE_12_MASK) >> (4 * 12);
	nibbles[13] = (arg & NIBBLE_13_MASK) >> (4 * 13);
	nibbles[14] = (arg & NIBBLE_14_MASK) >> (4 * 14);
	nibbles[15] = (arg & NIBBLE_15_MASK) >> (4 * 15);

	// Sort individual nibbles (albeit slowly...)
	for(i = 0; i < 16; i++) {

		min = i;

		for(j = i + 1; j < 16; j++) {
			if(nibbles[j] < nibbles[min]) {
				min = j;
			}
		}

		// Switch the current value with the minimum found
		temp = nibbles[i];
		nibbles[i] = nibbles[min];
		nibbles[min] = temp;
	}

	// Put the bytes back into a long
	result = (((unsigned long) nibbles[15]) << (4 * 15)) |
			 (((unsigned long) nibbles[14]) << (4 * 14)) |
			 (((unsigned long) nibbles[13]) << (4 * 13)) |
			 (((unsigned long) nibbles[12]) << (4 * 12)) | 
			 (((unsigned long) nibbles[11]) << (4 * 11)) |
			 (((unsigned long) nibbles[10]) << (4 * 10)) |
			 (((unsigned long) nibbles[9]) << (4 * 9)) |
			 (((unsigned long) nibbles[8]) << (4 * 8)) |
			 (((unsigned long) nibbles[7]) << (4 * 7)) |
			 (((unsigned long) nibbles[6]) << (4 * 6)) |
			 (((unsigned long) nibbles[5]) << (4 * 5)) |
			 (((unsigned long) nibbles[4]) << (4 * 4)) | 
			 (((unsigned long) nibbles[3]) << (4 * 3)) |
			 (((unsigned long) nibbles[2]) << (4 * 2)) |
			 (((unsigned long) nibbles[1]) << (4 * 1)) |
			 (((unsigned long) nibbles[0]) << (4 * 0));

  	return result;
}

/*********************************************************************
 *
 * little_linked_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *  
 *********************************************************************/

struct elt {
  char val;
  struct elt *link;
};

struct elt *name_list (void)
{
	struct elt * s;
	struct elt * t;
	struct elt * e1;
	struct elt * v;
	struct elt * e2;
	struct elt * n;

	// Set up values...
	s = (struct elt *) malloc(sizeof(struct elt));
	s->val = 's';
	if(s == NULL) {
		return NULL;
	}

	t = (struct elt *) malloc(sizeof(struct elt));
	t->val = 't';
	if(t == NULL) {
		free(s);
		return NULL;
	}

	e1 = (struct elt *) malloc(sizeof(struct elt));
	e1->val = 'e';
	if(e1 == NULL) {
		free(s);
		free(t);
		return NULL;
	}

	v = (struct elt *) malloc(sizeof(struct elt));
	v->val = 'v';
	if(v == NULL) {
		free(s);
		free(t);
		free(e1);
		return NULL;
	}

	e2 = (struct elt *) malloc(sizeof(struct elt));
	e2->val = 'e';
	if(e2 == NULL) {
		free(s);
		free(t);
		free(e1);
		free(v);
		return NULL;
	}

	n = (struct elt *) malloc(sizeof(struct elt));
	n->val = 'n';
		if(n == NULL) {
		free(s);
		free(t);
		free(e1);
		free(v);
		free(e2);
		return NULL;
	}

	// Set up pointers...
	s->link = t;
	t->link = e1;
	e1->link = v;
	v->link = e2;
	e2->link = n;
	n->link = NULL;

  	return s;
}

/*********************************************************************
 *
 * convert()
 *
 * specification: depending on the value of "mode", print "value" as
 * octal, binary, or hexidecimal to stdout, followed by a newline
 * character
 *
 * extra requirement 1: output must be via putc()
 *
 * extra requirement 2: print nothing if "mode" is not one of OCT,
 * BIN, or HEX
 *
 * extra requirement 3: all leading/trailing zeros should be printed
 *
 * EXAMPLE: convert (HEX, 0xdeadbeef) should print
 * "00000000deadbeef\n" (including the newline character but not
 * including the quotes)
 *
 *********************************************************************/

enum format_t {
  OCT = 66, BIN, HEX
};

void convert (enum format_t mode, unsigned long value)
{
	int i = 0;
	char nibbles[16];
	char binary[66];

	switch(mode) {
	case OCT:

		// Separate out into nibbles
		nibbles[0]  = (value & NIBBLE_0_MASK) >> (4 * 0);
		nibbles[1]  = (value & NIBBLE_1_MASK) >> (4 * 1);
		nibbles[2]  = (value & NIBBLE_2_MASK) >> (4 * 2);
		nibbles[3]  = (value & NIBBLE_3_MASK) >> (4 * 3);
		nibbles[4]  = (value & NIBBLE_4_MASK) >> (4 * 4);
		nibbles[5]  = (value & NIBBLE_5_MASK) >> (4 * 5);
		nibbles[6]  = (value & NIBBLE_6_MASK) >> (4 * 6);
		nibbles[7]  = (value & NIBBLE_7_MASK) >> (4 * 7);
		nibbles[8]  = (value & NIBBLE_8_MASK) >> (4 * 8);
		nibbles[9]  = (value & NIBBLE_9_MASK) >> (4 * 9);
		nibbles[10] = (value & NIBBLE_10_MASK) >> (4 * 10);
		nibbles[11] = (value & NIBBLE_11_MASK) >> (4 * 11);
		nibbles[12] = (value & NIBBLE_12_MASK) >> (4 * 12);
		nibbles[13] = (value & NIBBLE_13_MASK) >> (4 * 13);
		nibbles[14] = (value & NIBBLE_14_MASK) >> (4 * 14);
		nibbles[15] = (value & NIBBLE_15_MASK) >> (4 * 15);

		// Convert each nibble into a binary string and add to our string
		for(i = 15; i >= 0; i--) {
			switch(nibbles[i]) {
			case 0:
				binary[(i*4)]   = '0';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '0';
				break;
			case 1:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '0';
				break;	
			case 2:
				binary[(i*4)] = '0';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '0';
				break;	
			case 3:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '0';
				break;	
			case 4:
				binary[(i*4)]   = '0';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '0';
				break;	
			case 5:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '0';
				break;	
			case 6:
				binary[(i*4)]   = '0';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '0';
				break;	
			case 7:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '0';
				break;	
			case 8:
				binary[(i*4)]   = '0';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '1';
				break;		
			case 9:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '1';
				break;	
			case 0xA:
				binary[(i*4)]   = '0';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '1';
				break;	
			case 0xB:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '0';
				binary[(i*4)+3] = '1';
				break;	
			case 0xC:
				binary[(i*4)]   = '0';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '1';
				break;	
			case 0xD:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '0';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '1';
				break;	
			case 0xE:
				binary[(i*4)]   = '0';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '1';
				break;	
			case 0xF:
				binary[(i*4)]   = '1';
				binary[(i*4)+1] = '1';
				binary[(i*4)+2] = '1';
				binary[(i*4)+3] = '1';
				break;	
			default:
				return;
				break;
			}
		}

		// Octal uses binary in sets of three
		binary[64] = '0';
		binary[65] = '0';

		// Parse string for octal output (in reverse because putc)
		for(i = 65; i >= 0; i -= 3) {
			//printf("%d: %c\n%d: %c\n%d: %c\n", i, binary[i], i-1, binary[i-1], i-2, binary[i-2]);

			if(binary[i] == '0' && binary[i-1] == '0' && binary[i-2] == '0') {
				putc('0', stdout);
			}
			else if(binary[i] == '0' && binary[i-1] == '0' && binary[i-2] == '1') {
				putc('1', stdout);
			}
			else if(binary[i] == '0' && binary[i-1] == '1' && binary[i-2] == '0') {
				putc('2', stdout);
			}
			else if(binary[i] == '0' && binary[i-1] == '1' && binary[i-2] == '1') {
				putc('3', stdout);
			}
			else if(binary[i] == '1' && binary[i-1] == '0' && binary[i-2] == '0') {
				putc('4', stdout);
			}
			else if(binary[i] == '1' && binary[i-1] == '0' && binary[i-2] == '1') {
				putc('5', stdout);
			}
			else if(binary[i] == '1' && binary[i-1] == '1' && binary[i-2] == '0') {
				putc('6', stdout);
			}
			else if(binary[i] == '1' && binary[i-1] == '1' && binary[i-2] == '1') {
				putc('7', stdout);
			}
			else {
				return;
			}
		}

		putc('\n', stdout);
		break;
	case BIN:

		// Separate out into nibbles
		nibbles[0]  = (value & NIBBLE_0_MASK) >> (4 * 0);
		nibbles[1]  = (value & NIBBLE_1_MASK) >> (4 * 1);
		nibbles[2]  = (value & NIBBLE_2_MASK) >> (4 * 2);
		nibbles[3]  = (value & NIBBLE_3_MASK) >> (4 * 3);
		nibbles[4]  = (value & NIBBLE_4_MASK) >> (4 * 4);
		nibbles[5]  = (value & NIBBLE_5_MASK) >> (4 * 5);
		nibbles[6]  = (value & NIBBLE_6_MASK) >> (4 * 6);
		nibbles[7]  = (value & NIBBLE_7_MASK) >> (4 * 7);
		nibbles[8]  = (value & NIBBLE_8_MASK) >> (4 * 8);
		nibbles[9]  = (value & NIBBLE_9_MASK) >> (4 * 9);
		nibbles[10] = (value & NIBBLE_10_MASK) >> (4 * 10);
		nibbles[11] = (value & NIBBLE_11_MASK) >> (4 * 11);
		nibbles[12] = (value & NIBBLE_12_MASK) >> (4 * 12);
		nibbles[13] = (value & NIBBLE_13_MASK) >> (4 * 13);
		nibbles[14] = (value & NIBBLE_14_MASK) >> (4 * 14);
		nibbles[15] = (value & NIBBLE_15_MASK) >> (4 * 15);

		// Convert each nibble to a binary string and print
		for(i = 15; i >= 0; i--) {
			switch(nibbles[i]) {
			case 0:
				putc('0', stdout);
				putc('0', stdout);
				putc('0', stdout);
				putc('0', stdout);
				break;
			case 1:
				putc('0', stdout);
				putc('0', stdout);
				putc('0', stdout);
				putc('1', stdout);
				break;	
			case 2:
				putc('0', stdout);
				putc('0', stdout);
				putc('1', stdout);
				putc('0', stdout);
				break;	
			case 3:
				putc('0', stdout);
				putc('0', stdout);
				putc('1', stdout);
				putc('1', stdout);
				break;	
			case 4:
				putc('0', stdout);
				putc('1', stdout);
				putc('0', stdout);
				putc('0', stdout);
				break;	
			case 5:
				putc('0', stdout);
				putc('1', stdout);
				putc('0', stdout);
				putc('1', stdout);
				break;	
			case 6:
				putc('0', stdout);
				putc('1', stdout);
				putc('1', stdout);
				putc('0', stdout);
				break;	
			case 7:
				putc('0', stdout);
				putc('1', stdout);
				putc('1', stdout);
				putc('1', stdout);
				break;	
			case 8:
				putc('1', stdout);
				putc('0', stdout);
				putc('0', stdout);
				putc('0', stdout);
				break;		
			case 9:
				putc('1', stdout);
				putc('0', stdout);
				putc('0', stdout);
				putc('1', stdout);
				break;	
			case 0xA:
				putc('1', stdout);
				putc('0', stdout);
				putc('1', stdout);
				putc('0', stdout);
				break;	
			case 0xB:
				putc('1', stdout);
				putc('0', stdout);
				putc('1', stdout);
				putc('1', stdout);
				break;	
			case 0xC:
				putc('1', stdout);
				putc('1', stdout);
				putc('0', stdout);
				putc('0', stdout);
				break;	
			case 0xD:
				putc('1', stdout);
				putc('1', stdout);
				putc('0', stdout);
				putc('1', stdout);
				break;	
			case 0xE:
				putc('1', stdout);
				putc('1', stdout);
				putc('1', stdout);
				putc('0', stdout);
				break;	
			case 0xF:
				putc('1', stdout);
				putc('1', stdout);
				putc('1', stdout);
				putc('1', stdout);
				break;	
			default:
				return;
				break;
			}
		}

		putc('\n', stdout);
		break;

	case HEX: 

		// Separate out into nibbles
		nibbles[0]  = (value & NIBBLE_0_MASK) >> (4 * 0);
		nibbles[1]  = (value & NIBBLE_1_MASK) >> (4 * 1);
		nibbles[2]  = (value & NIBBLE_2_MASK) >> (4 * 2);
		nibbles[3]  = (value & NIBBLE_3_MASK) >> (4 * 3);
		nibbles[4]  = (value & NIBBLE_4_MASK) >> (4 * 4);
		nibbles[5]  = (value & NIBBLE_5_MASK) >> (4 * 5);
		nibbles[6]  = (value & NIBBLE_6_MASK) >> (4 * 6);
		nibbles[7]  = (value & NIBBLE_7_MASK) >> (4 * 7);
		nibbles[8]  = (value & NIBBLE_8_MASK) >> (4 * 8);
		nibbles[9]  = (value & NIBBLE_9_MASK) >> (4 * 9);
		nibbles[10] = (value & NIBBLE_10_MASK) >> (4 * 10);
		nibbles[11] = (value & NIBBLE_11_MASK) >> (4 * 11);
		nibbles[12] = (value & NIBBLE_12_MASK) >> (4 * 12);
		nibbles[13] = (value & NIBBLE_13_MASK) >> (4 * 13);
		nibbles[14] = (value & NIBBLE_14_MASK) >> (4 * 14);
		nibbles[15] = (value & NIBBLE_15_MASK) >> (4 * 15);

		// Convert each nibble to a character and print
		for(i = 15; i >= 0; i--) {
			if(nibbles[i] >= 0 && nibbles[i] <= 9) {
				putc(nibbles[i] + 48, stdout);
			}
			else {
				putc(nibbles[i] + 87, stdout);
			}
		}

		putc('\n', stdout);
		break;
	default:
		// Invalid mode
		break;
	}

}

/*********************************************************************
 *
 * draw_me()
 *
 * this function creates a file called me.txt which contains an ASCII-art
 * picture of you (it does not need to be very big). the file must (pointlessly,
 * since it does not contain executable content) be marked as executable.
 * 
 * extra requirement 1: you may only call the function syscall() (type "man
 * syscall" to see what this does)
 *
 * extra requirement 2: you must ensure that every system call succeeds; if any
 * fails, you must clean up the system state (closing any open files, deleting
 * any files created in the file system, etc.) such that no trash is left
 * sitting around
 *
 * you might be wondering how to learn what system calls to use and what
 * arguments they expect. one way is to look at a web page like this one:
 * http://blog.rchapman.org/post/36801038863/linux-system-call-table-for-x86-64
 * another thing you can do is write some C code that uses standard I/O
 * functions to draw the picture and mark it as executable, compile the program
 * statically (e.g. "gcc foo.c -O -static -o foo"), and then disassemble it
 * ("objdump -d foo") and look at how the system calls are invoked, then do
 * the same thing manually using syscall()
 *
 *********************************************************************/

void draw_me (void)
{
  	int statusCode = 0;
	int fileDescriptor = 0;

	// 40 * 19 picture of me
	char me[] = "++++++================~================\n++++==========$O$7$OZ$I================\n+++=========+OZMNN88O88$+==============\n++=+=======8DZMZ7II???$D7++============\n==========88DN7I?II???+=ZI++=======+===\n+========ODNZ77II??II?++=Z7++++++=+++++\n=========DNO$I7$$III777I+Z77+++++++++++\n======~==$DZ$ZZZZZI?ZOZ$7O$I++++++==+=+\n==========D$ZO8O$ZI+$Z8IIO$+++==++=++++\n===~======Z$$777ZZI++?I+=I?++=========~\n===~~=====Z$7777Z$II=????7+==~~~~~=~~~~\n~=======+?7$$$77$O$I7I++=+===~~~~~~::::\n~~~====+???$$777Z$$?+++~~+~~~~:~:::::,,\n~~====++????$$777$7I7+====~~~~:::::,,,,\n~~====++????$$7$Z$III+I=+~:::::::,,,,,,\n~====+++?+??I$$77I77I7+==::,,,,,,,,,,..\n====++++???7I$Z7I???++?++~::,,,,,,,,,..\n===++?II7$ZD8$$O77?II?+~~7=+~:::,,,,,..\n==+?I7$$7$ZZDO7?I7$7==~~~+I~:====~~:,,,\n";

	// Create and open the file
	fileDescriptor = syscall(SYS_open, "me.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IXUSR); 

	// Check to make sure it worked, if not, clean up
	if(fileDescriptor == -1) {
	  // Nothing to do here since there was no file created
	  return;
	}
	
	// Write the picture to the file
        statusCode = syscall(SYS_write, fileDescriptor, me, 40*19);

	// Check for errors and clean up if needed
	if(statusCode == -1) {
	  statusCode = syscall(SYS_unlink, "me.txt");
	  return;
	}

	statusCode = syscall(SYS_close, fileDescriptor); 

	// Check for errors and clean up if needed
	if(statusCode == -1) {
       	  statusCode = syscall(SYS_unlink, "me.txt");
	  return;
	}
}