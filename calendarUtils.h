#include "LinkedList/LinkedList.h"
#include "event.h"
#ifndef CALENDARUTILS_H
#define CALENDARUTILS_H 1

/*
 * Creates a single string from a LinkedList of Events
 *
 * Expected Usage
 * 		char* myString = createStringFromEvents(myLinkedList)	
 *
 * 	Warning
 * 		Do not use the return value of this function as a paramater to another function unless you are sure they will free it		
 */
char *createStringFromEvents(LinkedList*);

/* 
 * Opens a file (name provided as paramater), reads calendar entries and loads them 
 * into the linkedlist provided as a parmater.
 *
 * Expected Usage 
 * 		writeCalendar("MyFile", myList);
 */
void readCalendar(const char*, LinkedList*);

/*
 * Creates a file (name provided as paramater) and writes out the contents of the Linked
 * List provided as a paramater 
 *
 * Expected Usage
 * 		writeCalendar("MyFile", myList);
 */
void writeCalendar(const char*, LinkedList*);

/*
 * Frees events contained in a LinkedList
 *
 * Expected Usage
 * 		freeEventsFromList(myList)
 */
void freeEventsFromList(LinkedList*);

#endif
