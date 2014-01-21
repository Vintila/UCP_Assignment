#include <stdio.h>
#include "guiUtils.h"

int main(int argC, char *argV[])
{
	char *fileName = NULL;	
	WindowList *windowList;
	/*Check for Command Line arguments"*/
	if(argC > 1)
	{
		fileName = argV[1];
	}	

 	windowList = setupGUI("Calendar", fileName);
	
	startGUI(windowList->window);

	tearDownGUI(windowList);		

	return 0;
}
