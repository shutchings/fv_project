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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

struct Node {
  int val;
  struct Node* next;
};

struct Node* add_link (struct Node* head, int num);

int main(int argc, char* argv[]) {
	struct Node* head = NULL;

	head = add_link(head, 10);
	head = add_link(head, 9);

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

struct Node* add_link (struct Node* head, int num)
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
			struct new_node = (struct Node*) malloc(sizeof(struct Node));

			while()
			{
				
			}
		}
	}

	return head;
}