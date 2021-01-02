#include "event.h"


struct Event_t {
    Date date;
    char* name;
    int id;
    MemSet members;
};


Event eventCreate(char* name, int id, Date date) {
    assert (name && date);
    Event event = malloc (sizeof(*event));
    if (!event) {
        return NULL;
    }
    event->date = date;
    event->name = name;
    event->id = id;
    event->members = NULL;
    return event;
}


Event copy_event(Event event) {
    Event new_event = malloc (sizeof(*event));
    if (!new_event) {
        return NULL;
    }
    if((new_event->date = dateCopy(event->date)) == NULL) {
        free(new_event);
        return NULL;
    }
    new_event->name = malloc(strlen(event->name) + 1);
    if (new_event->name == NULL) {
        free(new_event->date);
        free(new_event);
        return NULL;
    }
    strcpy(new_event->name,event->name);
    if ((new_event->members = setCreate()) == NULL) {
        free(new_event->name);
        free(new_event->date);
        free(new_event);
        return NULL;
    }
    new_event->id = event->id;
    setCopyMembers(event->members ,new_event->members);
    return new_event;
}

void* void_copy_event(void* event) {
    return (Event)copy_event((Event)event);
}

void free_event(Event event) {
    free(event->name);
    dateDestroy(event->date);
    setDestroy(event->members);
    free(event);
    return;
}
void void_free_event(void* event) {
    free_event((Event)event);
}


bool equal_events(Event event1, Event event2){
    return event1->id == event2->id;
}

bool void_equal_events (void* event1, void* event2){
    return equal_events((Event)event1, (Event)event2);
}

int voidDateCompare(void* date1, void* date2) {
    return -dateCompare((Date)date1, (Date)date2);
}

void* voidDateCopy(void* date){
    return (Date)dateCopy((Date)date);
}

void voidDateDestroy(void* date) {
     dateDestroy((Date)date);
}

Date get_date(Event event) {
    assert(event);
    return event->date;
}

char* get_name(Event event){
    if(!event){
        return NULL;
    }
    return event->name;
}

int get_id(Event event){
    assert(event);
    return event->id;
}

bool memberExists (Event event, int member_id){
    assert(event);
    return setContains(event->members, member_id);
}

bool eventInsertMember(Event event, Member member){
    assert(event && member);
    if (setAdd(event->members, member) == SET_OUT_OF_MEMORY) {
        return false;
    }
    return true;
}

void eventChangeDate(Event event,Date date){
    assert(event && date);
    dateDestroy(event->date);
    event->date = dateCopy(date);
    return;
}

void eventRemoveMember(Event event, int member_id){
    setRemove(event->members,member_id);
}

void eventPrintAllMembers(Event event, FILE* fp) {
    assert(event && fp);
    setPrintAllMembers(event->members,fp);
}

MemSet getSet(Event event){
    if(!event){
        return NULL;
    }
    return event->members;
}
