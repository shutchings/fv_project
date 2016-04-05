#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h> 
#include <unistd.h>

struct elt {
  char val;
  struct elt *link;
};

enum format_t {
  OCT = 66, BIN, HEX
};

unsigned long byte_sort (unsigned long arg);
unsigned long nibble_sort (unsigned long arg);
struct elt *name_list (void);
void convert (enum format_t mode, unsigned long value);
void draw_me (void);
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

	// Test nibble_sort
	printf("Testing nibble_sort()...\n");
	assert(nibble_sort(0xfedcba9876543210), 0xfedcba9876543210, 9);
	assert(nibble_sort(0x0403deadbeef0201), 0xfeeeddba43210000, 10);
	assert(nibble_sort(0x0000000000000000), 0x0000000000000000, 11);
	assert(nibble_sort(0xffffffffffffffff), 0xffffffffffffffff, 12);
	assert(nibble_sort(0x1000000000000000), 0x1000000000000000, 13);
	assert(nibble_sort(0x0101010101010101), 0x1111111100000000, 14);
	assert(nibble_sort(0x0000000000000010), 0x1000000000000000, 15);
	assert(nibble_sort(0x1), 0x1000000000000000, 16);

	// Test name_list
	printf("Testing name_list()...\n");
	struct elt * name = name_list();
	printf("%c%c%c%c%c%c\n", name->val, name->link->val, name->link->link->val,
		   name->link->link->link->val, name->link->link->link->link->val,
		   name->link->link->link->link->link->val);

	// Test Convert
	printf("Testing convert()...\n");
	convert(HEX, 0xdeadbeef);
	convert(BIN, 0x0123456789abcdef);	
	convert(BIN, 0xabcdef);
	convert(BIN, 0xfedcba987);
	convert(OCT, 0xdeadbeef);
	convert(OCT, 0xFFFFFFFFFFFFFFFF);
	convert(OCT, 0xFFFFFFFFFFFFFFF);	
	convert(1, 0xFFFFFFFFFFFFFFFF);
	convert(2, 0xFFFFFFFFFFFFFFFF);
	convert(3, 0xFFFFFFFFFFFFFFFF);

	// Test draw_me
	printf("Testing draw_me()...\n");
	draw_me();

	return 0;
}

void assert(unsigned long testCase, unsigned long answer, int num) {
  if(testCase != answer) {
    printf("Test case %d failed!\n", num);
  }
  else {
    printf("Test case %d passed!\n", num);
  }
}