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

<<<<<<< HEAD
    if(!copy_element || !free_element || !equal_elements ||
       !copy_priority|| !free_priority || !compare_priorities) {
=======
void pqDestroy(PriorityQueue queue){
    if (!queue) {
        return;
    }
    pqClear(queue);
    free(queue);
}


PriorityQueue pqCopy(PriorityQueue queue){
    PriorityQueue new_queue = pqCreate(queue->copyPqElement,
                                       queue->freePqElement,
                                       queue->equalPqElements,
                                       queue->copyPqElementPriority,
                                       queue->freePqElementPriority,
                                       queue->comparePqElementPriorities);
    if(!new_queue){
        return NULL;
    }

    for(ElementList head = queue->list->next, head_new = new_queue->list ;head != NULL
            ;head = head->next, head_new = head_new->next) {
        ElementList node = malloc(sizeof((*node)));
        if(!node) {
            pqDestroy(new_queue);
            return NULL;
        }
        node->next = NULL;
        head_new->next = node;
        node->element = queue->copyPqElement(head->element);
        if(!node->element) {
            pqDestroy(new_queue);
            return NULL;
        }
        node->elementPriority = queue->copyPqElementPriority(head->elementPriority);
        if(!node->elementPriority) {
            pqDestroy(new_queue);
>>>>>>> ffd459b741d431f8f8ed77d53d029d3042e47d38
            return NULL;
    }
<<<<<<< HEAD

    PriorityQueue queue = malloc(sizeof(PriorityQueue));




=======
    return queue;
>>>>>>> ffd459b741d431f8f8ed77d53d029d3042e47d38
}
