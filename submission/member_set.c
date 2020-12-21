#include "member_set.h"

/** The factor by which the set array is expanded */
#define EXPAND_FACTOR 2

/** The initial size of the set array */
#define INITIAL_SIZE 10

/** Used by 'find' for reporting a member that does not exist in the set */
#define MEMBER_NOT_FOUND -1

/** Used to expand the size of the set */
static SetResult expand(MemSet set);

/** Used to find a member in a set */
static int find(MemSet set, int id);

/** Struct representing a member */

struct Member_t {
    char* name;
    int id;
    int events_number;
};

/** Struct representing a member implemented as an array */
struct Member_set_t {
    Member* member;
    int size;
    int maxSize;
    int iterator;
};

Member memberCreate(char* name, int id){
    if(!name) {
        return NULL;
    }
    Member member = malloc(sizeof(*member));
    if(!member) {
        return  NULL;
    }
    member->name = malloc(strlen(name) + 1);
    if (member->name == NULL) {
        free(member);
        return NULL;
    }
    strcpy(member->name,name);
    member->id=id;
    member->events_number = 0;
    return member;
}

MemSet setCreate(){
    MemSet memSet = malloc(sizeof(*memSet));
    if(!memSet){
        return  NULL;
    }
    memSet->member = malloc(sizeof(Member)*INITIAL_SIZE);
    if(!memSet->member){
        free(memSet);
        return NULL;
    }
    memSet->size = 0;
    memSet->maxSize = INITIAL_SIZE;
    return  memSet;
}

SetResult setAdd(MemSet set, Member member){
    if(!set || !member){
        return SET_NULL_ARG;
    }

    if (set->size == set->maxSize) {
        if (expand(set) == SET_OUT_OF_MEMORY) {
            return SET_OUT_OF_MEMORY;
        }
    }
    if(set->size == 0 || (set->member[set->size-1]->id <= member->id)){
        set->member[set->size] = member;
    }
    else{
        for(int i = 0 ; i < set->size ; i++){
            if(set->member[i]->id > member->id ){
                for (int j = set->size ; j > i ; j--){
                    set->member[j] = set->member[j-1];
                }
                set->member[i] = member;
                break;
            }
        }
    }
    set->size++;
    return SET_SUCCESS;
}


static SetResult expand(MemSet set) {
    assert(set != NULL);
    int newSize = EXPAND_FACTOR * set->maxSize;
    Member* newMembers = realloc(set->member, newSize * sizeof(Member));
    if (newMembers == NULL) {
        return SET_OUT_OF_MEMORY;
    }
    set->member = newMembers;
    set->maxSize = newSize;
    return SET_SUCCESS;
}

SetResult setRemove(MemSet set, int id){
    if(!set){
        return  SET_NULL_ARG;
    }
    int index = find(set, id);
    if (index == MEMBER_NOT_FOUND) {
        return SET_SUCCESS;
    }
    set->member[index] = set->member[set->size - 1];
    set->size--;
    return SET_SUCCESS;
}

static int find(MemSet set, int id) {
    assert(set != NULL);
    for (int i = 0; i < set->size; i++) {
        if (set->member[i]->id == id) {
            return i;
        }
    }
    return MEMBER_NOT_FOUND;
}

bool setContains(MemSet set, int id){
    assert(set);
    return  find(set,id)!=MEMBER_NOT_FOUND;
}

int setGetSize(MemSet set){
    assert(set);
    return set->size;
}

void setDestroy(MemSet set){
    assert(set);
    free(set->member);
    free(set);
    return;
}

void destroyMember(Member member){
    if (!member) {
        return;
    }
    free(member->name);
    free(member);
}

void setClear(MemSet set){
    if (!set) {
        return;
    }
    for(int i = 0 ; i < set->size ; i++ ){
        destroyMember(set->member[i]);
    }
}

Member setGetMember(MemSet set, int id){
    if(!set){
        return NULL;
    }
    int index = find(set, id);
    return set->member[index] ;
}

void memberEventTick(Member member) {
    member->events_number++;
}

void memberEventNegTick(Member member) {
    member->events_number--;
}

void setPrintAllMembers(MemSet set, FILE* fp){
    for (int i = 0; i < set->size; i++) {
        fprintf(fp, ",%s",set->member[i]->name);
    }
}

void setPrintAllMembersByEventNum(MemSet set, FILE* fp,int events_num){
    assert(set && fp);
    for (int i = events_num; i > 0; i--) {
        for( int j = 0 ; j < set->size ; j++){
            if(set->member[j]->events_number == i ){
                fprintf(fp, "%s,%d\n",set->member[j]->name,i);
            }
        }
    }
    return;
}

Member setGetFirstMember(MemSet set){
    if (!set || !set->size) {
        return NULL;
    }
    set->iterator = 0;
    return set->member[0] ;
}

Member setGetNextMember(MemSet set) {
    assert(!setIsDone(set));
    return set->member[++(set->iterator)];
}

bool setIsDone(MemSet set) {
return set->iterator >= set->size;
} 

int setGetMemberId(Member member){
    if(!member){
        return MEMBER_NOT_FOUND;
    }
    return member->id;
}

void setCopyMembers(MemSet set , MemSet new_set){
    if (!set || !new_set || set->size <=0){
        return;
    }
    while(new_set->maxSize <  set->maxSize ){
        expand(new_set);
    }

    for ( int i = 0 ; i < set->size ; i++){
        new_set->member[i] = set->member[i];
    }
    new_set->size = set->size;
}

int memnum (MemSet set , int id){
    Member member = setGetMember(set , id);
    return member->events_number;
}

void negtickAllMembers(MemSet set){
    for(int i = 0 ; i < set->size ; i++ ){
        set->member[i]->events_number--;
    }
    return;
}
