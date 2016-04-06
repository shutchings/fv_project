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

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BYTE_0_MASK	0x00000000000000FF
#define BYTE_1_MASK 0x000000000000FF00
#define BYTE_2_MASK	0x0000000000FF0000
#define BYTE_3_MASK	0x00000000FF000000
#define BYTE_4_MASK	0x000000FF00000000
#define BYTE_5_MASK	0x0000FF0000000000
#define BYTE_6_MASK	0x00FF000000000000
#define BYTE_7_MASK	0xFF00000000000000

unsigned long byte_sort (unsigned long arg);
void assert(unsigned long testCase, unsigned long answer, int num);

int main(int argc, char* argv[]) {

	// Test byte_sort
	printf("Testing byte_sort()...\n");
	assert(byte_sort(0x0706050403020100), 0x0706050403020100, 1);
	assert(byte_sort(0x0403deadbeef0201), 0xefdebead04030201, 2);
	assert(byte_sort(0x0001020304050607), 0x0706050403020100, 3);
	assert(byte_sort(0x0000000000000000), 0x0000000000000000, 4);
	assert(byte_sort(0xFFFFFFFFFFFFFFFF), 0xFFFFFFFFFFFFFFFF, 5);
	assert(byte_sort(0x0100000000000000), 0x0100000000000000, 6);
	assert(byte_sort(0x0000000000000001), 0x0100000000000000, 7);
	assert(byte_sort(0x1), 0x0100000000000000, 8);

	return 0;
}

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

void assert(unsigned long testCase, unsigned long answer, int num) {
  if(testCase != answer) {
    printf("Test case %d failed!\n", num);
  }
  else {
    printf("Test case %d passed!\n", num);
  }
}