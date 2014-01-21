#ifndef LINKEDLIST_H
#define LINKEDLIST_H 1

/*
 * A struct representing a single node in a LinkedList
 *
 * 	Fields
 * 		nextNode -- A pointer to another LinkedListNode 
 * 		data -- A void pointer to store the data of this node
 */
typedef struct LinkedListNode{
	struct LinkedListNode *nextNode;
	void *data;
}LinkedListNode;
/*
 * A struct containing the foundations for a LinkedList
 * 
 * Fields
 * 		root -- a LinkedListNode that points to the first Node in the list
 */
typedef struct{
	LinkedListNode *root;
}LinkedList;

/*
 * Allocates required memory and returns a LinkedList
 */
LinkedList *initLinkedList();

/*
 * reclaims memory used by this list by freeing the nodes followed by the list itself
 *
 * WARNING
 * 		this does not free the memory of the data, if you haven't saved it else where
 * 		you will create a memory leak
 */
void freeList(LinkedList*);

/*
 * Inserts a new node into the LinkedList at the front
 *
 * Expected Usage
 * 		insert(myLinkedList, &myData)
 * 		insert(myLinkedList, myPointer)
 * 		insert(myLinkedList, NULL)
 */
void insert(LinkedList*, void*);

/*
 * calculates the number of elements in the list
 *
 * Expected Usage
 * 		int listSize = size(myLinkedList)
 */
int size(LinkedList*);

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
void *removeFirst(LinkedList*);

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
void *retrieve(LinkedList*, int);

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
void *removeElement(LinkedList*, int);

/*
 * Removes all nodes from an existing list and creates a new one
 */
void emptyList(LinkedList*);
#endif
