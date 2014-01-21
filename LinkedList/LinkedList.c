#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>


/*
 * Allocates required memory and returns a LinkedList
 */
LinkedList *initLinkedList()
{
	LinkedList *list = malloc(sizeof(LinkedList));
	list->root = NULL;
	return list;	
}

/*
 * reclaims memory used by this list by freeing the nodes followed by the list itself
 *
 * WARNING
 * 		this does not free the memory of the data, if you haven't saved it else where
 * 		you will create a memory leak
 */
void freeList(LinkedList* list)
{
	LinkedListNode *curr = list->root;
	LinkedListNode *temp;
	while(curr != NULL)
	{
		temp = curr;
		curr = curr->nextNode;
		free(temp);	
	}

	free(list);
}

/*
 * Inserts a new node into the LinkedList at the front
 *
 * Expected Usage
 * 		insert(myLinkedList, &myData)
 * 		insert(myLinkedList, myPointer)
 * 		insert(myLinkedList, NULL)
 */
void insert(LinkedList *linkedList, void *data)
{
	LinkedListNode *newNode = malloc(sizeof(LinkedListNode));
	newNode->data = data;
	newNode->nextNode = linkedList->root;
	linkedList->root = newNode;
}

/*
 * real calulation behind the size funciton...
 * recursivly goes through the list until a null node is hit(last one) and counts how many elements there are
 */
static int sizeRecursve(LinkedListNode *node)
{
	if (node != NULL)
		return 1 + sizeRecursve(node->nextNode);
	return 0;
}

/*
 * calculates the number of elements in the list
 *
 * Expected Usage
 * 		int listSize = size(myLinkedList)
 */
int size(LinkedList *linkedList)
{
	return sizeRecursve(linkedList->root);	
}

/*
 * returns the data from the first position in the list and also removes it
 *
 * Expected Usage
 * 		myDataType data = removeFirst(myList)
 *
 * Expected Return
 * 		The value of the first element, or;
 * 		NULL if the list is empty
 */
void *removeFirst(LinkedList *list)
{
	void* temp = NULL;

	if(size(list) != 0)
	{
		LinkedListNode *curr = list->root;

		list->root = list->root->nextNode;

		temp = curr->data;

		free(curr);	
	}

	return temp;
}

/* 
 * returns data from a specific position in the list
 *
 * Expected Usage
 * 		myDataType data = retrieve(myList, 42)
 *
 * Expected Return
 * 		The value of the selected element, or;
 * 		NULL if the list is empty
 */
void *retrieve(LinkedList *list, int position)
{
	void* value = NULL;
	int listSize = size(list);
	int ii;
	LinkedListNode *curr = list->root;
	if(listSize != 0 && position <= listSize)
	{
		for(ii=1; ii != position; ii++)
		{
			curr = curr->nextNode;	
		}	

		value = curr->data;
	}
	
	return value;

}

/* 
 * returns data from a specific position in the list and also removes it
 *
 * Expected Usage
 * 		myDataType data = removeElement(myList, 42)
 *
 * Expected Return
 * 		The value of the selected element, or;
 * 		NULL if the list is empty
 */
void *removeElement(LinkedList *list, int position)
{
	void* value = NULL;
	int listSize = size(list);
	int ii;
	LinkedListNode *curr = list->root;
	LinkedListNode *prev = list->root;
	if(listSize != 0 && position <= listSize)
	{
		for(ii=1; ii != position; ii++)
		{
			prev = curr;
			curr = curr->nextNode;	
		}
		value = curr->data;
		if(ii == 1)
		{
			list->root = curr->nextNode;
		}
		else
		{
			prev->nextNode = curr->nextNode;	
		}	
		free(curr);
	}
	return value;	
}

/*
 * Removes all nodes from an existing list and creates a new one
 */
void emptyList(LinkedList* list)
{
	freeList(list);
	list = initLinkedList();	
}
