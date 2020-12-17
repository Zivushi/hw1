
#include <assert.h>
#include "priority_queue.h"
#include <stdlib.h>


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
                       ComparePQElementPriorities compare_priorities) {

    if (!copy_element || !free_element || !equal_elements ||
        !copy_priority || !free_priority || !compare_priorities) {
        return NULL;
    }

    PriorityQueue queue = malloc(sizeof(*queue));
    if (!queue) {
        return NULL;
    }
    queue->comparePqElementPriorities = compare_priorities;
    queue->equalPqElements = equal_elements;
    queue->freePqElementPriority = free_priority;
    queue->freePqElement = free_element;
    queue->copyPqElementPriority = copy_priority;
    queue->copyPqElement = copy_element;
    queue->list->next = NULL;
    return queue;
}

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
            return NULL;
        }
    }
    return queue;
}



PriorityQueueResult pqInsert(PriorityQueue queue, PQElement element, PQElementPriority priority){

    if (!queue || !element || !priority) {
        return PQ_NULL_ARGUMENT;
    }

    ElementList node = malloc(sizeof(*node));
    if (!node) {
        return PQ_OUT_OF_MEMORY;
    }

    node->element = queue->copyPqElement(element);
    if (!node->element){
        return PQ_OUT_OF_MEMORY;
    }

    node->elementPriority = queue->copyPqElementPriority(priority);
    if (!node->elementPriority){
        return PQ_OUT_OF_MEMORY;
    }

    for(ElementList head = queue->list ; head->next != node ; head = head->next) {
        if (!head->next) {
            head->next = node;
            node->next = NULL;
        }
        else if (queue->comparePqElementPriorities(node->elementPriority, head->next->elementPriority) > 0) {
            node->next = head->next;
            head->next = node;
        }
    }
    return PQ_SUCCESS;
}

