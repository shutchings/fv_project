/*********************************************************************
 *
 * nibble_sort()
 *
 * Specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * The fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 * 
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

#include <smack.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

unsigned long nibble_sort (unsigned long arg);
void assert(unsigned long testCase, unsigned long answer, int num);

int main(int argc, char* argv[]) {

	// Test nibble_sort

	/*
	printf("Testing nibble_sort()...\n");
	assert(nibble_sort(0xfedcba9876543210), 0xfedcba9876543210, 0);
	assert(nibble_sort(0x0403deadbeef0201), 0xfeeeddba43210000, 1);
	assert(nibble_sort(0x0000000000000000), 0x0000000000000000, 2);
	assert(nibble_sort(0xffffffffffffffff), 0xffffffffffffffff, 3);
	assert(nibble_sort(0x1000000000000000), 0x1000000000000000, 4);
	assert(nibble_sort(0x0101010101010101), 0x1111111100000000, 5);
	assert(nibble_sort(0x0000000000000010), 0x1000000000000000, 6);
	assert(nibble_sort(0x1), 0x1000000000000000, 7);
	*/

	unsigned long val = __VERIFIER_nondet_ulong();

	return 0;
}

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

void assert(unsigned long testCase, unsigned long answer, int num) {
  if(testCase != answer) {
    printf("Test case %d failed!\n", num);
  }
  else {
    printf("Test case %d passed!\n", num);
  }
}

