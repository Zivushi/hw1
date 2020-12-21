#ifndef EVENT_MANAGER_MEMBER_SET_H
#define EVENT_MANAGER_MEMBER_SET_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>


typedef struct Member_t* Member;

/** A set of members */
typedef struct Member_set_t* MemSet;

/** Type used for reporting errors in Set */
typedef enum {
    SET_SUCCESS, SET_OUT_OF_MEMORY, SET_NULL_ARG
} SetResult;


/** Creating a new set of members */
MemSet setCreate();

/** Destroying a set */
void setDestroy(MemSet set);

/** Adding a new member to the set */
SetResult setAdd(MemSet set, Member member);

/** Removing a member from a set */
SetResult setRemove(MemSet set, int id);

/** Checking if a member exists in a set */
bool setContains(MemSet set, int id);

/** Getting the number of members in a set */
int setGetSize(MemSet set);

/** Creating a new  members */
Member memberCreate(char* name, int id);

/** Destroying a member */
void destroyMember(Member member);

/** Getting the pointer to a member in a set */
Member setGetMember(MemSet set, int id);

/** Adding to the size of events of a member */
void memberEventTick(Member member);

/** substracting to the size of events of a member */
void memberEventNegTick(Member member);

/** Printing all members to a file */
void setPrintAllMembers(MemSet set, FILE* fp);

/** Printing all members to a file by the numbers of event given */
void setPrintAllMembersByEventNum(MemSet set, FILE* fp,int events_num);

/** Getting the pointer to the first member in a set */
Member setGetFirstMember(MemSet set);

/** Getting the pointer to the next member in a set */
Member setGetNextMember(MemSet set);

/** Returning true if the iteration over the set is done  */
bool setIsDone(MemSet set);

/** Getting a member id  */
int setGetMemberId(Member member);

/** Removes all members from the set */
void setClear(MemSet set);

/** Copying members from a set to a new set */
void setCopyMembers(MemSet set , MemSet new_set);


/** Substracting to the size of events of all member */
void negtickAllMembers(MemSet set);


#endif //EVENT_MANAGER_MEMBER_SET_H
