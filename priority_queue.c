#include <assert.h>
#include "priority_queue.h"
#include <stdlib.h>

#define NULL_QUEUE -1

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
    int queueSize;
    ElementList iterator;
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
    queue->queueSize = 0;
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
    new_queue->queueSize = queue->queueSize;
    return new_queue;
}

int pqGetSize(PriorityQueue queue){
    return queue ? queue->queueSize : NULL_QUEUE;
}

bool pqContains(PriorityQueue queue, PQElement element){
    if (!queue || !element){
        return false;
    }
    ElementList head = queue->list->next;
    while(head) {
        if (queue->equalPqElements(element, head->element) == 0) {
            return true;
        }
        head = head->next;
    }
    return false;
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
        free(node);
        return PQ_OUT_OF_MEMORY;
    }

    node->elementPriority = queue->copyPqElementPriority(priority);
    if (!node->elementPriority){
        queue->freePqElement(node->element);
        free(node);
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
    queue->queueSize++;
    return PQ_SUCCESS;
}


PriorityQueueResult pqChangePriority(PriorityQueue queue, PQElement element,
                                     PQElementPriority old_priority, PQElementPriority new_priority){
    if (!queue || !element || !old_priority || !new_priority) {
        return PQ_NULL_ARGUMENT;
    }
    if (queue->comparePqElementPriorities(old_priority,new_priority) == 0) {
        return PQ_SUCCESS;
    }

    for(ElementList head = queue->list; head->next != NULL &&
            queue->comparePqElementPriorities(old_priority, head->next->elementPriority) <= 0;
                head = head->next) {
        if (queue->comparePqElementPriorities(old_priority, head->next->elementPriority) == 0) {
            if (queue->equalPqElements(head->next->element, element) == 0) {
                ElementList tmp = head->next;
                head->next = head->next->next;
                queue->freePqElement(tmp->element);
                queue->freePqElementPriority(tmp->elementPriority);
                free(tmp);
                return pqInsert(queue,element,new_priority);
            }
        }
    }
    return PQ_ELEMENT_DOES_NOT_EXISTS;

}

PriorityQueueResult pqRemove(PriorityQueue queue){
    if (!queue) {
        return PQ_NULL_ARGUMENT;
    }
    ElementList tmp = queue->list->next;
    if (tmp == NULL){
        return PQ_SUCCESS;
    }
    queue->list->next = tmp->next;
    queue->freePqElement(tmp->element);
    queue->freePqElementPriority(tmp->elementPriority);
    free(tmp);
    queue->queueSize--;
    return PQ_SUCCESS;
}




PriorityQueueResult pqRemoveElement(PriorityQueue queue, PQElement element){
    if (!queue || !element) {
        return PQ_NULL_ARGUMENT;
    }

    if (!pqContains(queue,element)) {
        return PQ_ELEMENT_DOES_NOT_EXISTS;
    }

    for(ElementList head = queue->list ; head->next != NULL ; head = head->next) {
        if (queue->equalPqElements(head->next->element, element) == 0) {
            ElementList tmp = head->next;
            head->next = head->next->next;
            queue->freePqElement(tmp->element);
            queue->freePqElementPriority(tmp->elementPriority);
            free(tmp);
            queue->queueSize--;
            return PQ_SUCCESS;
        }
    }
    assert(false); //would never get here
    return PQ_ERROR;
}

PQElement pqGetFirst(PriorityQueue queue){
    if (!queue || !queue->queueSize) {
        return NULL;
    }
    queue->iterator = queue->list;
    return queue->iterator->next->element;
}

PQElement pqGetNext(PriorityQueue queue){
    if (!queue) {
        return NULL;
    }
    queue->iterator = queue->iterator->next;
    return queue->iterator->next->element;
}

PriorityQueueResult pqClear(PriorityQueue queue) {
    if (!queue) {
        return PQ_NULL_ARGUMENT;
    }
    while (queue->queueSize > 0) {
        pqRemove(queue);
    }
    return PQ_SUCCESS;
}



