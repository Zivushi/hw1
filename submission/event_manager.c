#include "event_manager.h"
#include <stdio.h>
#include "priority_queue.h"
#include "member_set.h"
#include "event.h"

#define NULL_EM -1

static Event findById(PriorityQueue queue, int id);

struct EventManager_t {
    Date date;
    PriorityQueue eventsPQ;
    MemSet members ;
};

EventManager createEventManager(Date date){
    Date new_date = dateCopy(date);
    if (!new_date) {
        return NULL;
    }
    EventManager em = malloc(sizeof(*em));
    if(!em) {
        dateDestroy(new_date);
        return NULL;
    }
    em->eventsPQ = pqCreate(void_copy_event, void_free_event, void_equal_events,
                            voidDateCopy, voidDateDestroy, voidDateCompare);

    if (!em->eventsPQ) {
        dateDestroy(new_date);
        free(em);
        return NULL;
    }
    em->members = setCreate();
    if (!em->members) {
        pqDestroy(em->eventsPQ);
        dateDestroy(new_date);
        free(em);
        return NULL;
    }
    em->date = new_date;
    return em;
}

EventManagerResult emAddEventByDate(EventManager em, char* event_name, Date date, int event_id) {
    if (!em || !event_name || !date) {
        return EM_NULL_ARGUMENT;
    }
    if (dateCompare(date , em->date) < 0) {
        return EM_INVALID_DATE;
    }
    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }

    PQ_FOREACH(Event, it, em->eventsPQ) {
        if(strcmp(get_name(it), event_name) == 0 && dateCompare(date, get_date(it)) == 0) {
            return EM_EVENT_ALREADY_EXISTS;
        }
    }
    Event event = eventCreate(event_name,event_id,date);
    if (!event) {
        return EM_OUT_OF_MEMORY;
    }
    if (pqContains(em->eventsPQ, event)) {
        free(event);
        return EM_EVENT_ID_ALREADY_EXISTS;
    }
    if (pqInsert(em->eventsPQ, event, date) == PQ_OUT_OF_MEMORY) {
        free(event);
        return EM_OUT_OF_MEMORY;
    }
    free(event);
    return EM_SUCCESS;

}

void destroyEventManager(EventManager em){
    if(!em) {
        return;
    }
    dateDestroy(em->date);
    pqDestroy(em->eventsPQ);
    setClear(em->members);
    setDestroy(em->members);
    free(em);
}

EventManagerResult emAddEventByDiff(EventManager em, char* event_name, int days, int event_id) {
    if (!em) {
        return EM_NULL_ARGUMENT;
    }
    if (days < 0) {
        return EM_INVALID_DATE;
    }
    Date date = dateCopy(em->date);
    while (days--) {
        dateTick(date);
    }
    EventManagerResult res = emAddEventByDate(em, event_name, date, event_id);
    dateDestroy(date);
    return res;
}

EventManagerResult emRemoveEvent(EventManager em, int event_id) {
    if (!em) {
        return EM_NULL_ARGUMENT;
    }
    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }
    Event event = findById(em->eventsPQ, event_id);
    if (!event) {
        return EM_EVENT_NOT_EXISTS;
    }
    negtickAllMembers(getSet(event));
    pqRemoveElement(em->eventsPQ, event);
    return EM_SUCCESS;
}

static Event findById(PriorityQueue queue, int id){
    PQ_FOREACH(Event, it, queue) {
        if (get_id(it) == id){
            return it;
        }
    }
    return NULL;
}


EventManagerResult emChangeEventDate(EventManager em, int event_id, Date new_date){
    if (!em || !new_date) {
        return EM_NULL_ARGUMENT;
    }
    if (dateCompare(new_date , em->date) < 0) {
        return EM_INVALID_DATE;
    }
    if (event_id < 0) {
        return EM_INVALID_EVENT_ID;
    }
    Event event = findById(em->eventsPQ, event_id);
    if (!event) {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if (dateCompare(new_date, get_date(event)) == 0) {
        return EM_EVENT_ALREADY_EXISTS;
    }

    PQ_FOREACH(Event, it, em->eventsPQ) {
        if(strcmp(get_name(it), get_name(event)) == 0 && dateCompare(new_date, get_date(it)) == 0) {
            return EM_EVENT_ALREADY_EXISTS;
        }
    }
    if(pqChangePriority(em->eventsPQ,event,get_date(event),new_date) == PQ_OUT_OF_MEMORY){
        return EM_OUT_OF_MEMORY;
    }
    event = findById(em->eventsPQ, event_id);
    eventChangeDate(event,new_date);
    return EM_SUCCESS;
}

EventManagerResult emAddMember(EventManager em, char* member_name, int member_id) {
    if(!em || !member_name){
        return EM_NULL_ARGUMENT;
    }
    if(member_id < 0) {
        return EM_INVALID_MEMBER_ID;
    }
    Member member = memberCreate(member_name,member_id);
    if (!member) {
        return EM_OUT_OF_MEMORY;
    }
    if(setContains(em->members,member_id)) {
        destroyMember(member);
        return EM_MEMBER_ID_ALREADY_EXISTS;
    }
    if (setAdd(em->members,member) == SET_OUT_OF_MEMORY) {
        destroyMember(member);
        return EM_OUT_OF_MEMORY;
    }
    return EM_SUCCESS;
}

EventManagerResult emAddMemberToEvent(EventManager em, int member_id, int event_id) {
    if (!em){
        return EM_NULL_ARGUMENT;
    }
    if( event_id < 0 ){
        return EM_INVALID_EVENT_ID;
    }
    if(member_id < 0) {
        return EM_INVALID_MEMBER_ID;
    }
    Event event = findById(em->eventsPQ, event_id);
    if (!event) {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if (!setContains(em->members ,member_id)) {
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    if (memberExists(event, member_id)) {
        return EM_EVENT_AND_MEMBER_ALREADY_LINKED;
    }
    Member member = setGetMember(em->members, member_id);
    eventInsertMember(event, member);
    memberEventTick(member);
    return EM_SUCCESS;
}
EventManagerResult emRemoveMemberFromEvent (EventManager em, int member_id, int event_id){
    if (!em){
        return EM_NULL_ARGUMENT;
    }
    if( event_id < 0 ){
        return EM_INVALID_EVENT_ID;
    }
    if(member_id < 0) {
        return EM_INVALID_MEMBER_ID;
    }
    Event event = findById(em->eventsPQ, event_id);
    if (!event) {
        return EM_EVENT_ID_NOT_EXISTS;
    }
    if (!setContains(em->members ,member_id)) {
        return EM_MEMBER_ID_NOT_EXISTS;
    }
    if (!memberExists(event, member_id)) {
        return EM_EVENT_AND_MEMBER_NOT_LINKED;
    }
    Member member = setGetMember(em->members, member_id);
    eventRemoveMember(event, member_id);
    memberEventNegTick(member);
    return EM_SUCCESS;
}

EventManagerResult emTick(EventManager em, int days) {
    if(!em) {
        return EM_NULL_ARGUMENT;
    }
    if(days<=0) {
        return EM_INVALID_DATE;
    }
    while (days--){
        dateTick(em->date);
    }
    int i = 0;
    PQ_FOREACH(Event, it, em->eventsPQ) {
        if(dateCompare(em->date, get_date(it)) > 0) {
            i++;
        }
    }

    while(i--){
        Event itr = pqGetFirst(em->eventsPQ);
        negtickAllMembers(getSet(itr));
        pqRemove(em->eventsPQ);
    }
    return EM_SUCCESS;
}

int emGetEventsAmount(EventManager em){
    if(!em) {
        return NULL_EM;
    }
    return pqGetSize(em->eventsPQ);
}

char* emGetNextEvent(EventManager em){
    if(!em) {
        return NULL;
    }
    Event event = (Event) pqGetFirst(em->eventsPQ);
    return get_name(event);
}

void emPrintAllEvents(EventManager em, const char* file_name){
    if(!em || ! file_name){
        return;
    }
    FILE* fp = fopen (file_name, "w");
    if(!fp){
        return;
    }
    int day,month,year;
    int* pday = &day, *pmonth = &month, *pyear = &year;
    PQ_FOREACH(Event, itr , em->eventsPQ){
        assert(itr);
        dateGet(get_date(itr),pday,pmonth,pyear);
        fprintf(fp, "%s,%d.%d.%d",get_name(itr),day,month,year);
        eventPrintAllMembers(itr,fp);
        fprintf(fp, "\n");
    }
    fclose(fp);
    return;
}

void emPrintAllResponsibleMembers(EventManager em, const char* file_name) {
    if(!em || ! file_name){
        return;
    }
    FILE* fp = fopen (file_name, "w");
    if(!fp){
        return;
    }
    setPrintAllMembersByEventNum(em->members,fp,emGetEventsAmount(em));
    fclose(fp);
    return;
}
