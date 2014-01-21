#include <stdlib.h>
#include <ctype.h>
#include "calendarUtils.h"
#include <stdio.h>
#include <string.h>

const char* months[] = { "January", "Febuary", "March", "April", "May", "June", "Jul", "August", "September", "October", "November", "December" }; 

/*
 * Misc utils for the Calendar Application
 */

/*
 * Creates a single string from an event passed in as a paramater
 */
static char *createEventString(Event *event)
{
	char *finalString = (char*) calloc(65356, sizeof(char));
	sprintf(finalString + strlen(finalString), "%s", event->eventName);
	
	if(strlen(event->location) > 0)
	{
		sprintf(finalString + strlen(finalString), " @ %s", event->location);
	}

	sprintf(finalString + strlen(finalString), " (");	

	if(event->duration > 60)	
	{
		sprintf(finalString + strlen(finalString), "%d", event->duration / 60);
		
		if(event->duration / 60 > 1)
		{
			sprintf(finalString + strlen(finalString), " hours");
		}
		else
		{
			sprintf(finalString + strlen(finalString), " hour");
		}
	}

	/* Check if there are any minutes */	
	if(event->duration % 60 != 0)
	{
		if(event->duration > 60)
		{ 
			sprintf(finalString + strlen(finalString), ", "); 
		}
		
		sprintf(finalString + strlen(finalString), "%d", event->duration % 60); 
		
		if(event->duration % 60 == 1)
		{
			sprintf(finalString + strlen(finalString), " minute");
		}
		else
		{
			sprintf(finalString + strlen(finalString), " minutes");
		}
	}
	
	sprintf(finalString + strlen(finalString), ")");
	sprintf(finalString + strlen(finalString), "\n");
	
	sprintf(finalString + strlen(finalString), "%d %s %d", event->Date.day, months[event->Date.month - 1], event->Date.year);
	
	sprintf(finalString + strlen(finalString), ", %d:%02d%s", (event->Time.hour+11) % 12 + 1, event->Time.minutes, event->Time.hour / 12  == 0 ? "am" : "pm");
	
	sprintf(finalString + strlen(finalString), "\n--\n\n");

	/* Reallocate the correct amount of memory for this string */	
	finalString = (char*) realloc(finalString, sizeof(char) * (strlen(finalString)+1));	
	return finalString;
}

/*
 * Creates a single string from a LinkedList of Events
 *
 * Expected Usage
 * 		char* myString = createStringFromEvents(myLinkedList)	
 
* 	Warning
 * 		Do not use the return value of this function as a paramater to another function unless you are sure they will free it		
 */
char* createStringFromEvents(LinkedList *list)
{
	int ii;
	Event *event;
	int stringSize = 128;
	char *finalString = (char*) calloc((stringSize + 1), sizeof(char));	
	char *newString;

	for(ii = 1; ii < size(list) + 1; ii++)
	{
		event = (Event*)retrieve(list, ii);	
		newString = createEventString(event);

		while(strlen(finalString) + strlen(newString) > stringSize)
		{
			stringSize *= 2;
			finalString = (char*) realloc(finalString, sizeof(char) * (stringSize + 1));
		}
		
		sprintf(finalString + strlen(finalString), "%s", newString);
		
		free(newString);	
	}
	
	return finalString;
}
/*
 * Reads from the file provided as a paramater, returns a new valid event or NULL if a part of the file could not be read 
 *
 * Expected Usage
 *		Event* myEvnet = readEvent(myFile)
 *
 * Return Value
 *		A valid Event, or;
 *		NULL if the Event could not be read 
 */
static Event *readEvent(FILE *file)
{
	int numRead;
	Event *newEvent = NULL;
	unsigned short day, month, hour, minutes, duration;
	unsigned int year;
	char nextChar;
	numRead = fscanf(file, "%u-%hu-%hu %hu:%hu %hu", 
		&year,
		&month,
		&day,
		&hour,
		&minutes,
		&duration			
	);

	if(
			numRead == 6 &&
			day <= 31 &&
			month <= 12 &&
			hour < 24 &&
			minutes <= 60
		)
	{
		newEvent = initEvent();		
	
		newEvent->Date.year = year;
		newEvent->Date.month = month;
		newEvent->Date.day = day; 
		newEvent->Time.hour = hour;
		newEvent->Time.minutes = minutes;
		newEvent->duration = duration;
		/*Read and discard all spaces until the event name is reached*/	
		do
		{
			nextChar = fgetc(file);
		}while(isspace(nextChar));

		newEvent->eventName[0] = nextChar;	
		fgets(newEvent->eventName + 1, 129, file);
		
		/*Remove the \n at the end...*/
		newEvent->eventName[strlen(newEvent->eventName) - 1] = '\0';
		
		fgets(newEvent->location, 129, file);

		/*Remove the \n at the end...*/
		newEvent->location[strlen(newEvent->location) - 1] = '\0';

	}

	return newEvent;
}

/* Opens a file (name provided as paramater), reads calendar entries up until the end
 * of the file or it incounters an errorous record
 *
 * 	Expected Usage
 * 		readCalendar("MyFileName", myLinkedList)
 */
void readCalendar(const char* fileName, LinkedList *list)
{
	FILE *calendar = fopen(fileName, "r");
	Event *newEvent = NULL;

	if(calendar != NULL)
	{
		while((newEvent = readEvent(calendar)) != NULL)
		{
			insert(list, newEvent);
		}		
			
		fclose(calendar);
	}
	else
	{
		perror("The file could not be opened");
	}
}

/*
 * Opens a file (name provided as paramater) and writes out the contents of the Linked
 * List provided as a paramater 
 *
 * Expected Usage
 * 		writeCalendar("MyFileName", myLinkedList)
 */
void writeCalendar(const char* fileName, LinkedList *list)
{
	FILE *calendar = fopen(fileName, "w");
	int ii;
	Event *event;
	if(calendar != NULL)
	{
		for(ii = 1; ii <= size(list); ii++)
		{
			event = retrieve(list, ii);
			fprintf(calendar, 
				"%u-%hu-%hu %hu:%hu %hu %s\n",
				event->Date.year,
				event->Date.month,
				event->Date.day,
				event->Time.hour,
				event->Time.minutes,
				event->duration,
				event->eventName
			);
			if(strlen(event->location) > 0)
			{
				fprintf(calendar,
					"%s\n",
					event->location
				);
			}
			fprintf(calendar,
				"\n"
			);
		}	
		
		fclose(calendar);
	}
	else
	{
		perror("the file could not be written to");
	}
}

/*
 * Frees events contained in a LinkedList
 *
 * Expected Usage
 * 		freeEventsFromList(myList)
 */
void freeEventsFromList(LinkedList *list)
{
	int ii;
	for(ii = 1; ii <= size(list); ii++)
	{
		freeEvent(retrieve(list, ii));
	}
}
