/*
 *  A wrapper around David Cooper's gui wrapper 
 */
#include "event.h"
#include "LinkedList/LinkedList.h"
#include "gui.h"
#ifndef GUIUTILS_H
#define GUIUTILS_H 1

/*
 * A struct for passing multiple values through a callback function
 *
 * Fields
 * 		list -- A LinkedList holding all of the events
 * 		window -- A window which is used for the gui system
 */
typedef struct{
	LinkedList *list;
	Window *window;
}WindowList;

/*
 * Creates a valid WindowList ready for use with other functions in this file
 * optionally takes the name of a file to preload the calendar with.
 *
 * Expected usage:
 * 		setupGUI("MyApplication", "MyFile")
 * 		setupGUI("MyApplication", NULL)
 */
WindowList *setupGUI(char*, char*);


/*
 * Kicks the whole gui off, this will take control away from your program until the 
 * window is closed
 *
 * Expeced Usage
 * 		startGUI(myWindow)
 */
void startGUI(Window*);

/*
 * Cleans up the gui by reclaiming memory used
 *
 * Expected Usage
 * 		tearDownGUI(myWindowList)
 */
void tearDownGUI(WindowList*);

#endif
