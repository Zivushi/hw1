//
// Created by Ziv on 17/12/2020.
//

#include <stdlib.h>
#include <assert.h>
#include "priority_queue.h"

typedef struct element_list_t {
    PQElement element;
    PQElementPriority elementPriority;
    struct element_list_t* next;
} *ElementList;


struct PriorityQueue_t {
    ElementList list;
    CopyPQElement copyPqElement;
    CopyPQElementPriority copyPqElementPriority;
    FreePQElement freePqElement;
    FreePQElementPriority freePqElementPriority;
    EqualPQElements equalPqElements;
    ComparePQElementPriorities comparePqElementPriorities;
};

PriorityQueue pqCreate(CopyPQElement copy_element,
                       FreePQElement free_element,
                       EqualPQElements equal_elements,
                       CopyPQElementPriority copy_priority,
                       FreePQElementPriority free_priority,
                       ComparePQElementPriorities compare_priorities){

    if(!copy_element || !free_element || !equal_elements ||
       !copy_priority|| !free_priority || !compare_priorities) {
            return NULL;
    }

    PriorityQueue queue = malloc(sizeof(PriorityQueue));




}
