/*
 *  A wrapper around David Cooper's gui wrapper 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "guiUtils.h"
#include "calendarUtils.h"
#define NUM_INPUTS 5
#define FALSE 0
#define TRUE !FALSE
static void spawnCreateCalendarDialogBox(void*);
static void createAndLinkButtons(WindowList*);
static void spawnSaveCalendarDialogBox(void*);
static void spawnLoadCalendarDialogBox(void*);
static void freePropertiesAndInputs(InputProperties*, char**, int);
static void spawnEditCalendarDialogBox(void*);
static void spawnRemoveCalendarDialogBox(void*);
static void loadEventDetailsIntoForm(char**, Event*);
static int getUserSelectedEvent(WindowList*);
static WindowList *initWindowList();
static Event *getEventFromForm(char**);
static void setCalendarText(WindowList*);
static void initInputsAndPropertiesForEventForm(InputProperties*, char**);

/*
 * Creates a valid WindowList ready for use with other functions in this file
 * optionally takes the name of a file to preload the calendar with.
 *
 * Expected usage:
 * 		setupGUI("MyApplication", "MyFile")
 * 		setupGUI("MyApplication", NULL)
 */
WindowList* setupGUI(char* windowTitle, char* fileName)
{
	
	WindowList* windowList = initWindowList();
	windowList->list = initLinkedList();
	windowList->window = createWindow(windowTitle);

	if(fileName != NULL)
	{
		readCalendar(fileName, windowList->list);	
		setCalendarText(windowList);
	}

	createAndLinkButtons(windowList);
	
	return windowList;
}

/*
 * Cleans up the WindowList passed as a paramater
 */
static void freeWindowList(WindowList* windowList)
{
	free(windowList);
}

/* 
 * Shows a dialog box with one text box asking for an integer
 *
 * Expected Usage
 *		 eventIndex = getUserSelectedEvent(myWindowList)
 *
 * Expected Return
 *		The number the user seclected, or;
 * 		-1 if cancel was pressed or input was not valid
 */
static int getUserSelectedEvent(WindowList* windowList)
{
	int dialogBoxResult = FALSE;
	int eventIndex = -1;
	char **inputs = (char**) malloc(sizeof(char*) * 1);
		
	InputProperties *properties = (InputProperties*)  malloc(sizeof(InputProperties) * 1);
	inputs[0] = (char*) calloc(9, sizeof(char)); /* space for the index */

	properties[0].label = "Enter Event Index positon";	
	properties[0].maxLength=8;
	properties[0].isMultiLine=FALSE;

	dialogBoxResult = dialogBox(
		windowList->window,
		"Find Event",
		1,
		properties,
		inputs
	);
	if(dialogBoxResult != FALSE && strlen(inputs[0]) > 0)
	{
		sscanf(inputs[0], "%d", &eventIndex);
	}	

	return eventIndex;
}

/* Creates and allocates memory for a new WindowList
 *
 * Expected Usage
 *		WindowList *wl = initWindowList()
 *
 * Expected Return value
 * 		This function should return a valid WindowList, or;
 * 		NULL if the memory allocation failed
 */
static WindowList *initWindowList()
{
	WindowList *newWindowList = (WindowList*)malloc(sizeof(WindowList));
	return newWindowList;
}

/* Helper function for setting the gui text built from the linkedlist
 *
 *
 * Expected Usage
 * 		setCalendarText(myWindowList)
 */
static void setCalendarText(WindowList *windowList)
{
	char* calendarString = createStringFromEvents(windowList->list);
	setText(windowList->window, calendarString);
	free(calendarString);
}

/* Kicks the whole gui off, this will take control away from your program until the 
 * window is closed
 *
 * Expeced Usage
 * 		startGUI(myWindow)
 */
void startGUI(Window* window)
{
	runGUI(window);
}

/* Cleans up the gui by reclaiming memory used
 *
 * Expected Usage
 * 		tearDownGUI(myWindowList)
 */
void tearDownGUI(WindowList* windowList)
{
	freeEventsFromList(windowList->list);

	freeWindow(windowList->window);
	
	freeList(windowList->list);

	freeWindowList(windowList);
}

/* Adds required buttons to the window provided as a paramater and assigns
 * the correct callback funcitons
 *
 */
void createAndLinkButtons(WindowList *windowList)
{
	addButton(windowList->window, "Load Calendar", &spawnLoadCalendarDialogBox, windowList);	
	addButton(windowList->window, "Save Calendar", &spawnSaveCalendarDialogBox, windowList);
	addButton(windowList->window, "Create new Event", &spawnCreateCalendarDialogBox, windowList);
	addButton(windowList->window, "Edit an Event", &spawnEditCalendarDialogBox, windowList);
	addButton(windowList->window, "Delete an Event", &spawnRemoveCalendarDialogBox, windowList);
}

/*****************
 *
 * callback functions that are used for the button presses
 *
 *****************
*/

/* 
 * Callback function that walks a user through creating a new event
 * 	
 * 	Firstly asking the user for the details of the new event
 * 	Then until the user cancels or enters a valid evnet step 1 will repeat
 */
static void spawnCreateCalendarDialogBox(void* inWindowList)
{	
	WindowList *windowList = (WindowList*)inWindowList;
	InputProperties *properties = (InputProperties*) malloc(sizeof(InputProperties) * NUM_INPUTS);
	char **inputs = (char**) malloc(sizeof(char*) * NUM_INPUTS);
	Event *newEvent = NULL;
	int dialogBoxResult;
	initInputsAndPropertiesForEventForm(properties, inputs);
	/*loops until the cancel button is pressed or a valid set of input is entered*/
	do	
	{
		if(dialogBoxResult == TRUE)
		{
			messageBox(windowList->window, "The values you entered are either incorrect or missing...");
		}	
		dialogBoxResult = dialogBox(
			windowList->window,
			"Creating new Event",
			NUM_INPUTS,
			properties,
			inputs
		);
		
	}while(
		(strlen(inputs[0]) == 0 ||
		strlen(inputs[1]) == 0 ||
		strlen(inputs[2]) == 0 ||
		strlen(inputs[3]) == 0 || 
		getEventFromForm(inputs) == NULL) &&
		dialogBoxResult != FALSE
	);

	if(dialogBoxResult != FALSE)
	{
			newEvent = getEventFromForm(inputs);
			insert(windowList->list, newEvent); 
			setCalendarText(windowList);
					
	}

	freePropertiesAndInputs(properties, inputs, 5);	

}
/* A callback function that takes a user through the process of editing an event
 * 
 * 		first by asking for the index of an event
 * 		then providing a form to fill in with the new details
 */
static void spawnEditCalendarDialogBox(void *inWindowList)
{
	WindowList *windowList = (WindowList*)inWindowList;
	
	InputProperties *properties = (InputProperties*) malloc(sizeof(InputProperties) * NUM_INPUTS);
	
	char **inputs = (char**) malloc(sizeof(char*) * NUM_INPUTS);

	int eventIndex;	
	Event *event;
		
	int dialogBoxResult;
	
	if(size(windowList->list) == 0)
	{
		messageBox(windowList->window, "There is no loaded Events");
		return;
	}
			
	eventIndex = getUserSelectedEvent(windowList);

	/*Checks that the selected index matches to an index within the linkedlist*/
	if(eventIndex > 0 && eventIndex <= size(windowList->list))
	{
		event = (Event*) retrieve(windowList->list, eventIndex);
		initInputsAndPropertiesForEventForm(properties, inputs);				
		loadEventDetailsIntoForm(inputs, event);
		do	
		{
			if(dialogBoxResult == TRUE)
			{
				messageBox(windowList->window, "The values you entered are either incorrect or missing...");
			}	
			dialogBoxResult = dialogBox(
				windowList->window,
				"Editing Event",
				NUM_INPUTS,
				properties,
				inputs
			);
			
		}while(
			(strlen(inputs[0]) == 0 ||
			strlen(inputs[1]) == 0 ||
			strlen(inputs[2]) == 0 ||
			strlen(inputs[3]) == 0 || 
			getEventFromForm(inputs) == NULL) &&
			dialogBoxResult != FALSE
		);	
		
		if(dialogBoxResult != FALSE)
		{
			/*could also be done by creating a new Event and overwriting the old one...*/
			sscanf(inputs[0], "%hu/%hu/%u", &event->Date.day, &event->Date.month, &event->Date.year);
			
			sscanf(inputs[1], "%hu:%hu", &event->Time.hour, &event->Time.minutes);			
			sscanf(inputs[2], "%hu", &event->duration);
			
			strcpy(event->eventName, inputs[3]);
			strcpy(event->location, inputs[4]);
			setCalendarText(windowList);
			
		}	
		freePropertiesAndInputs(properties, inputs, 5);
	}
}
/* Callback function that takes a user through removing an event
 * 		
 * 		first by getting them to enter the index of an element they wish to remove into a dialogbox
 * 		then by removing it from the linkedlist
 */
void spawnRemoveCalendarDialogBox(void *inWindowList)
{
	WindowList *windowList = (WindowList*)inWindowList;
	
	int eventIndex;
	if(size(windowList->list) == 0)
	{
		messageBox(windowList->window, "There is no loaded Events");
		return;
	}
	eventIndex = getUserSelectedEvent(windowList);
	/*Checks that the user selected index is within the linked list*/
	if(eventIndex > 0 && eventIndex <= size(windowList->list))
	{
		removeElement(windowList->list, eventIndex);
		setCalendarText(windowList);
	}
	
}

/* 	a callback function that Displays a dialogbox to the user allowing them to
 *  enter the name of a file to save the conents of the linkedlist
 *
 * 	Expected Usage
 * 		spawnSaveCalenddarDialogBox(myWindowList)		
 */
void spawnSaveCalendarDialogBox(void *inWindowList)
{
	WindowList *windowList = (WindowList*)inWindowList;
	
	InputProperties *properties = (InputProperties*) malloc(sizeof(InputProperties) * 1);
	
	char **input = (char**) malloc(sizeof(char*) * 1); 
	int dialogBoxResult = FALSE;
	properties[0].label = "File Name";
	properties[0].maxLength=64;
	properties[0].isMultiLine=FALSE;
	
	input[0] = (char*) calloc(65, sizeof(char)); /* space for the file name */

	dialogBoxResult = dialogBox(
		windowList->window,
		"Save Calendar to File",
		1,
		properties,
		input
	);

	if(dialogBoxResult != FALSE)
	{
		writeCalendar(input[0], windowList->list);	
	}

	freePropertiesAndInputs(properties, input, 1);
}


/* callback function which prompts the user for a file name to read a calendar from
 * into the linkedlist
 */
void spawnLoadCalendarDialogBox(void* inWindowList)
{
	WindowList *windowList = (WindowList*)inWindowList;

	InputProperties *properties = (InputProperties*) malloc(sizeof(InputProperties) * 1);

	char **inputs = (char**) malloc(sizeof(char*) * 1);
	
	int dialogBoxResult = FALSE;
	properties[0].label = "File Name";
	properties[0].maxLength=64;
	properties[0].isMultiLine=FALSE;

	inputs[0] = (char*) calloc(65, sizeof(char)); /* space for the file name */
	
	dialogBoxResult = dialogBox(
		windowList->window,
		"Load Calendar from File",
		1,
		properties,
		inputs
	);

	if(dialogBoxResult != FALSE)
	{
		emptyList(windowList->list);
		
		readCalendar(inputs[0], windowList->list);
		setCalendarText(windowList);
	}
	freePropertiesAndInputs(properties, inputs, 1);	

}
/* Misc functions for the gui
 *
 *
 */

/* Helper function to allocated correct memory and assign correct properties to a set of inputs that will be used in event forms
 *
 * Expected Usage
 * 		initInputsAndPropertiesForEventForm(myProperties, myInputs);
 */
static void initInputsAndPropertiesForEventForm(InputProperties *properties, char** inputs)
{
		properties[0].label = "Date";
		properties[0].maxLength=10;
		properties[0].isMultiLine=FALSE;

		properties[1].label = "Time";
		properties[1].maxLength=5;
		properties[1].isMultiLine=FALSE;
		
		properties[2].label = "Duration";
		properties[2].maxLength=5;
		properties[2].isMultiLine=FALSE;

		properties[3].label = "Event Name";
		properties[3].maxLength=128;
		properties[3].isMultiLine=FALSE;

		properties[4].label = "Location";
		properties[4].maxLength=128;
		properties[4].isMultiLine=FALSE;

		inputs[0] = (char*) calloc(11, sizeof(char)); /* space for the Date */
		inputs[1] = (char*) calloc(6, sizeof(char)); /* space for the Time in xx:yy format */
		inputs[2] = (char*) calloc(6, sizeof(char)); /* space for up to 65535 duration  */
		inputs[3] = (char*) calloc(129, sizeof(char)); /* space to hold 128 character event name*/
		inputs[4] = (char*) calloc(129, sizeof(char)); /*space for 128 character location */

}
/* Helper function to free up memory used for the form inputs
 *
 * Expected Usage
 * 		freePropertiesAndInputs(myProperties, myInputs, numberOfInputs)
 */
static void freePropertiesAndInputs(InputProperties *properties, char **inputs, int nElements)
{
	int ii;

	for(ii = 0; ii < nElements; ii++)
	{
		free(inputs[ii]);
	}

	free(properties);
	free(inputs);
	
}

/* Uses a set of inputs create a new event
 *
 * Expected Usage
 * 		getEventFromForm(myStrings)
 *
 * 	Expected Return
 * 		A valid event if possilbe, or;
 * 		NULL if there is not the right data to create an event		
 */
Event *getEventFromForm(char** inputs)
{
	Event *newEvent = initEvent();

	int totalRead = 0;
	
	totalRead += sscanf(
					inputs[0],
					"%hu/%hu/%u",
					&newEvent->Date.day,
					&newEvent->Date.month, 
					&newEvent->Date.year
				);
	
	totalRead += sscanf(
					inputs[1], 
					"%hu:%hu", 
					&newEvent->Time.hour, 
					&newEvent->Time.minutes
				);			
	totalRead += sscanf(
					inputs[2], 
					"%hu", 
					&newEvent->duration
				);
	
	if(strlen(inputs[3]) > 0)
		totalRead += 1;
	
	strcpy(newEvent->eventName, inputs[3]);
	strcpy(newEvent->location, inputs[4]);	
	/*Check that we read the right amount and they contain valid values*/	
	if(
		totalRead < 7 ||
		newEvent->Date.day > 31 || 
		newEvent->Date.month > 12 ||
		newEvent->Time.hour > 24 ||
		newEvent->Time.minutes > 60 ||
		strlen(newEvent->eventName) == 0
	  )
	{
		freeEvent(newEvent);
		newEvent = NULL;
	}
	
	return newEvent;
}

/* Uses the provided Event to load into the the provided inputs to 
 * display in a form
 *
 */
void loadEventDetailsIntoForm(char **inputs, Event* event)
{
	sprintf(inputs[0], "%hu/%hu/%u", 
		event->Date.day,
		event->Date.month,
		event->Date.year
	);
	
	sprintf(inputs[1], "%hu:%hu",
		event->Time.hour,
		event->Time.minutes
	);	
	
	sprintf(inputs[2], "%hu",
		event->duration
	);
	
	sprintf(inputs[3], "%s",
		event->eventName
	);
	
	sprintf(inputs[4], "%s",
		event->location
	);		
}
