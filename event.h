#ifndef EVENT_H
#define EVENT_H 1

/*
 * Struct for holding one (1) event
 * 
 * Fields:
 *
 * 	Date -- A struct for representing a given date of an event
 * 	  Fields:
 * 		day -- an unsigned integer representing the day of the month (eg 14)
 * 		month -- an unsigned integer representing the month of the year (eg 04)
 * 		year -- an unsigned integer representing the current year (eg 2013)
 * 	
 * 	Time -- A struct for holding the given time of an event
 * 	  Fields:
 * 		hour -- an unsigned integer representing the hour
 * 		minutes -- an unsigned integer representing the minutes
 * 	
 * 	duration -- an unsigned integer for representing the events duration capable of a maximum size of 65535 minutes ( 1092 Hours, 15 Minutes )
 *
 *
 * 	eventName -- A string representing the name of a given event
 *
 * 	location -- A string representing the location of a given event
 *
 */
typedef struct{

	struct{
		unsigned short day;
		unsigned short month;
		unsigned int year;
	}Date;

	struct{
		unsigned short hour;
		unsigned short minutes;
	}Time;

	unsigned short duration;
	char *eventName;
	char *location;

}Event;

/*
 * Allocates memory and returns a new Event
 *
 * Expected Usage
 * 		Event *myEvent = initEvent()
 *
 * Expected Return
 *		A valid Event, or;
 *		NULL if the memory allocation failed
 */
Event* initEvent();

/*
 * Reclaims the memory used by an event
 *
 * Expected Usage
 * 		freeEvent(myEvent)
 */
void freeEvent(Event*);
#endif
