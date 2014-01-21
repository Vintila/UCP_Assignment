#include<stdio.h>
#include<stdlib.h>
#include"event.h"
/*
 * Allocates memory and returns a new Event*
 *
 * Expected Usage
 * 		Event *myEvent = initEvent()
 *
 * Expected Return
 *		A valid Event, or;
 *		NULL if the memory allocation failed
 */
Event *initEvent()
{
	Event *newEvent = (Event*)malloc(sizeof(Event));
	newEvent->eventName = (char*)calloc(129, sizeof(char));
	newEvent->location = (char*)calloc(129, sizeof(char));	
	return newEvent;
}

/*
 * Reclaims the memory used by an event
 *
 * Expected Usage
 * 		freeEvent(myEvent)
 */

void freeEvent(Event *event)
{
	free(event->eventName);
	free(event->location);
	free(event);
}
