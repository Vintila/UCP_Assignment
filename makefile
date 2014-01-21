CC=gcc
CFLAGS=-Wall -pedantic -std=c89
DEBUGFLAGS=-g
GUIFLAGS=`pkg-config --cflags --libs gtk+-2.0`
OBJ= calendar.o window.o guiUtils.o LinkedList.o event.o calendarUtils.o
OUT=Calendar
 
$(OUT) : $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(GUIFLAGS) $(DEBUGFLAGS)

window.o : gui.h gui.c
	$(CC) $(CFLAGS) -o window.o -c gui.c $(GUIFLAGS) $(DEBUGFLAGS) 

calendar.o : guiUtils.h main.c 
	$(CC) $(CFLAGS) -o calendar.o -c main.c $(DEBUGFLAGS)

guiUtils.o : guiUtils.c guiUtils.h event.h LinkedList/LinkedList.h 
	$(CC) $(CFLAGS) -o guiUtils.o -c guiUtils.c $(DEBUGFLAGS)

event.o : event.h event.c
	$(CC) $(CFLAGS) -o event.o -c event.c $(DEBUGFLAGS) 

calendarUtils.o : calendarUtils.h calendarUtils.c LinkedList/LinkedList.h 
	$(CC) $(CFLAGS) -o calendarUtils.o -c calendarUtils.c $(DEBUGFLAGS)

LinkedList.o : LinkedList/LinkedList.h LinkedList/LinkedList.c
	$(CC) $(CFLAGS) -o LinkedList.o -c LinkedList/LinkedList.c $(DEBUGFLAGS)

clean :
	rm -f $(OBJ) $(OUT)


