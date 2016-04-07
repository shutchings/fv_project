/*********************************************************************
 *
 * Sorted Linked List
 *
 * specification: allocate and return a pointer to a linked list of
 * of integers
 *
 * - the list should always be sorted
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 *********************************************************************/

#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef int bool;
#define true 1
#define false 0

struct Node {
  int val;
  struct Node* next;
};

struct Node* add_link(struct Node* head, int num);
bool is_sorted(struct Node* head);
void assert(bool testCase, int num);

int main(int argc, char* argv[]) {
	struct Node* head = NULL;

	head = add_link(head, 10);
	assert(is_sorted(head), 0);
	head = add_link(head, 7);
	assert(is_sorted(head), 1);
	head = add_link(head, 8);
	assert(is_sorted(head), 2);
	head = add_link(head, 11);
	assert(is_sorted(head), 3);

	// Print list
	struct Node* temp = head;

	while (temp != NULL)
	{
		printf("%d ", temp->val);
		temp = temp->next;
	}

	printf("\n");

	return 0;
}

struct Node* add_link(struct Node* head, int num)
{
	// Nothing in the list yet
	if (head == NULL)
	{
		head = (struct Node*) malloc(sizeof(struct Node));
		head->val = num;
		head->next = NULL;
	}
	else
	{
		// Put it on the front
		if (num < head->val)
		{
			struct Node* old_head = head;
			head = (struct Node*) malloc(sizeof(struct Node));
			head->val = num;
			head->next = old_head;
		}
		else // Insert it into the list
		{
			struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
			new_node->next = head;
			new_node->val = num;

			struct Node* marker = head->next;

			while(marker != NULL && marker->val < num)
			{
				new_node->next = marker;
				marker = marker->next;
			}

			new_node->next->next = new_node;
			new_node->next = marker;
		}
	}

	return head;
}

bool is_sorted(struct Node* head)
{
	int max = INT_MIN;
	struct Node* temp = head;

	while (temp != NULL)
	{
		if (temp->val < max)
		{
			return false;
		}
		else
		{
			max = temp->val;
		}

		temp = temp->next;
	}

	return true;
}

void assert(bool testCase, int num) {
  if(!testCase) {
    printf("Test case %d failed!\n", num);
  }
  else {
    printf("Test case %d passed!\n", num);
  }
}

