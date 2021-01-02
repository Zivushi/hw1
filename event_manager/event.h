#ifndef EVENT_MANAGER_EVENT_H
#define EVENT_MANAGER_EVENT_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "date.h"
#include "member_set.h"

typedef struct Event_t* Event;

/** Creating a new event */
Event eventCreate(char* name, int id, Date date);

/** Copying a given event */
Event copy_event(Event event);

/** Destroying an event */
void free_event(Event event);

/** comparing two events and returning true if equal */
bool equal_events (Event event1, Event event2);

/** comparing two dates of events and returning true if equal */
int voidDateCompare(void* date1, void* date2);

/** copying two dates of events */
void* voidDateCopy(void* date);

/** destroying a date of event */
void voidDateDestroy(void* date);

/** Getting a date from an event */
Date get_date(Event event);

/** Getting the name from an event */
char* get_name(Event event);

/** Getting the id from an event */
int get_id(Event event);

/** Returns true if a member is incharge of the given event */
bool memberExists (Event event, int member_id);

/** Adding an incharge member to an event and returning true if succeed */
bool eventInsertMember(Event event, Member member);

/** Changing a date of an event */
void eventChangeDate(Event event,Date date);

/** Removing a member incharge from an event */
void eventRemoveMember(Event event, int member_id);

/** Returns true if two events are equal */
bool void_equal_events (void* event1, void* event2);

/** Copying an event */
void* void_copy_event(void* event);

/** destroying an event */
void void_free_event(void* event);

/** Getting a member set incharge of the event */
MemSet getSet(Event event);

/** Printing members incharge of an event to a file */
void eventPrintAllMembers(Event event, FILE* fp);


#endif //EVENT_MANAGER_EVENT_H
