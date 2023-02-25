/*

  Name: Jignash Reddy Yaduguri & Abhyudai Singh
  ID: 1002027941

*/
// The MIT License (MIT)
// 
// Copyright (c) 2022 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include "mavalloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

enum TYPE
{
	FREE = 0,
	USED
};

struct Node
{
	size_t size;
	enum TYPE type;
	void *arena;
	struct Node * next;
	struct Node * prev;
};

struct Node * alloc_list;
struct Node * previous_node;

void *arena;

enum ALGORITHM allocation_algorithm = FIRST_FIT;

int mavalloc_init(size_t size, enum ALGORITHM algorithm)
{
	arena = malloc(ALIGN4(size)); //allocate memory for arena

	allocation_algorithm = algorithm; //set allocation algorithm

	alloc_list = (struct Node *) malloc(sizeof(struct Node)); //allocate memory for alloc_list

	alloc_list->arena = arena; //set arena
	alloc_list->size = ALIGN4(size); //set size
	alloc_list->type = FREE; //set type
	alloc_list->next = NULL; //set next
	alloc_list->prev = NULL; //set prev

	previous_node = alloc_list; //set previous_node to alloc_list

	return 0;
}

void mavalloc_destroy()
{
	free(arena);	//free the arena
	struct Node *head = alloc_list;	//set head to the first node

	// iterate over the linked list and free the nodes
	while (head)	//while head is not null
	{
		struct Node *ptr = head;	//set ptr to head
		head = head->next;	//set head to the next node
		free(ptr);	//free the ptr
	}

	alloc_list = NULL;	//set alloc_list to null
	return;
}

void *mavalloc_alloc(size_t size)
{
	struct Node * node;

	if (allocation_algorithm != NEXT_FIT)	//if the allocation algorithm is not nextfit
	{
		node = alloc_list;	// assign node to head node
	}
	else if (allocation_algorithm == NEXT_FIT)	// if the allocation algorithm is nextfit
	{
		node = previous_node;	//assign node to the last occupied node.
	}
	else
	{
		printf("ERROR: Unknown allocation algorithm!\n");
		exit(0);
	}

	size_t aligned_size = ALIGN4(size);
	//Implement the First Fit
	if (allocation_algorithm == FIRST_FIT)
	{
		while (node)
		{
			if (node->size >= aligned_size && node->type == FREE)	//if the node is free and the available memory size is greater than or equal to the required size.
			{
				int leftover_size = 0;

				node->type = USED;	//set the node type as used.
				leftover_size = node->size - aligned_size;	//check the free size
				node->size = aligned_size;	//change the node size to required memory size

				if (leftover_size > 0)
				{
					struct Node *previous_next = node->next;	//assign the new block
					struct Node *leftover_node = (struct Node *) malloc(sizeof(struct Node));	//create a free node

					leftover_node->arena = node->arena + size;	//change the arena size
					leftover_node->type = FREE;	//set node type to free
					leftover_node->size = leftover_size;	//free block size
					leftover_node->next = previous_next;

					node->next = leftover_node;	//link the new free node to the current node.
				}

				previous_node = node;
				return (void*) node->arena;	//return the address of the node.
			}

			node = node->next;	//move to the next node.
		}
	}

	// Implement Next Fit
	else if (allocation_algorithm == NEXT_FIT)

	{
		while (node)
		{
			if (node->size >= aligned_size && node->type == FREE)	//if the node is free and the available memory size is greater than or equal to the required size.
			{
				int leftover_size = 0;

				node->type = USED;	//set the node type as used.
				leftover_size = node->size - aligned_size;	//check the free size
				node->size = aligned_size;	//change the node size to required memory size

				if (leftover_size > 0)
				{
					struct Node *previous_next = node->next;	//assign the new block
					struct Node *leftover_node = (struct Node *) malloc(sizeof(struct Node));	//create a free node

					leftover_node->arena = node->arena + size;	//change the arena size
					leftover_node->type = FREE;	//set node type to free
					leftover_node->size = leftover_size;	//free block size
					leftover_node->next = previous_next;	//link the new free node to the previous node.

					node->next = leftover_node;	//link the new free node to the current node.
				}

				previous_node = node;	//set the previous node to the current node.
				return (void*) node->arena;	//return the address of the node.
			}

			node = node->next;	//move to the next node.
		}
	}

	// Implement Best Fit
	else if (allocation_algorithm == BEST_FIT)
	{
		struct Node *best_fit_node = NULL;	//set best fit node to null
		size_t best_fit_size = INT_MAX;	//set best fit size to maximum integer value.

		while (node)	//while node is not null.
		{
			//find the node with the smallest avaiable memory size and free.
			if (node->size >= aligned_size && node->type == FREE)	//if the node is free and the available memory size is greater than or equal to the required size.
			{
				if (node->size < best_fit_size)	//if the node  has smaller size than the best fit size.
				{
					best_fit_node = node;	//set best fit node to the current node.
					best_fit_size = node->size;	//set best fit size to the current node size.
				}
			}

			node = node->next;	//move to the next node.
		}

		if (best_fit_node)	//once we find the smallest available free size node.
		{
			int leftover_size = 0;

			best_fit_node->type = USED;	//set the node type as used.
			leftover_size = best_fit_node->size - aligned_size;	//check the free size
			best_fit_node->size = aligned_size;	//change the node size to required memory size

			if (leftover_size > 0)	//if the leftover size is greater than 0.
			{
				struct Node *previous_next = best_fit_node->next;	//assign the new block
				struct Node *leftover_node = (struct Node *) malloc(sizeof(struct Node));	//create a free node

				leftover_node->arena = best_fit_node->arena + size;	//change the arena size
				leftover_node->type = FREE;	//set node type to free
				leftover_node->size = leftover_size;	//free block size
				leftover_node->next = previous_next;	//link the new free node to the previous node.

				best_fit_node->next = leftover_node;	//link the new free node to the current node.
			}

			// previous_node = best_fit_node;
			return (void*) best_fit_node->arena;	//return the address of the best fit node allocated memory.
		}
	}

	// Implement Worst Fit
	else if (allocation_algorithm == WORST_FIT)
	{
		struct Node *worst_fit_node = NULL;	//set worst fit node to null
		size_t worst_fit_size = 0;	//set worst fit size to 0.

		while (node)
		{
			//find the node with the largest avaiable memory size and free.
			if (node->size >= aligned_size && node->type == FREE)	//if the node is free and the available memory size is greater than or equal to the required size.
			{
				if (node->size > worst_fit_size)	//if the node has larger size than the worst fit size.
				{
					worst_fit_node = node;	//set worst fit node to the current node.
					worst_fit_size = node->size;	//set worst fit size to the current node size.
				}
			}

			node = node->next;	//move to the next node.
		}

		if (worst_fit_node)	//once we find the largest available free size node.
		{
			int leftover_size = 0;	//set the leftover size to 0.

			worst_fit_node->type = USED;	//set the node type as used.
			leftover_size = worst_fit_node->size - aligned_size;	//check the free size
			worst_fit_node->size = aligned_size;	//change the node size to required memory size

			if (leftover_size > 0)	//if the leftover size is greater than 0.
			{
				struct Node *previous_next = worst_fit_node->next;	//assign the new block
				struct Node *leftover_node = (struct Node *) malloc(sizeof(struct Node));	//create a free node

				leftover_node->arena = worst_fit_node->arena + size;	//change the arena size
				leftover_node->type = FREE;	//set node type to free
				leftover_node->size = leftover_size;	//free block size
				leftover_node->next = previous_next;	//link the new free node to the previous node. 

				worst_fit_node->next = leftover_node;	//link the new free node to the current node.
			}

			previous_node = worst_fit_node;	//set the previous node to the current node.
			return (void*) worst_fit_node->arena;	//return the address of the worst fit node allocated memory.
		}
	}

	return NULL;
}

void mavalloc_free(void *ptr)
{
	struct Node *node = alloc_list;	//set node to the head of the list.

	while (node != NULL)	//while node is not null.
	{
		if (node->arena == ptr)	//if the node arena is equal to the pointer.
		{
			node->type = FREE;	//set the node type as free.
			previous_node = alloc_list;	//set the previous node to the head of the list.
			return;
		}

		node = node->next;	//move to the next node.
	}

	return;
}

int mavalloc_size()
{
	int number_of_nodes = 0;	//set number of nodes to 0.
	int free_nodes = 0;	//set free nodes to 0.
	struct Node *ptr = alloc_list;	//set ptr to the head of the list.

	while (ptr)	//while ptr is not null.
	{
		//count the number of free nodes.
		if (ptr->type == FREE)	//if the node is free.
		{
			free_nodes++;	//increment the free nodes.
		}

		number_of_nodes++;	//increment the number of nodes.

		ptr = ptr->next;	//move to the next node.
	}

	if (number_of_nodes == 0)	//if the linked list is empty
	{
		return 0;	//return 0.
	}
	else if (number_of_nodes == free_nodes)
	{
		//if the number of nodes is equal to the number of free nodes.
		return 1;	//return 1 since we combine all the free nodes to one single node.
	}

	return number_of_nodes;	//return the number of nodes.
}