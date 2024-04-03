#include "eventlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void popEvent(EventList* eventList) {
    if (eventList == NULL || eventList->head == NULL) {
        return;
    }
    EventListNode* temp = eventList->head;
    eventList->head = eventList->head->next;
    free(temp);
}

Event front(const EventList* eventList) {
    Event emptyEvent = {IF, {"", Integer, NULL, 0, 0}};
    if (eventList == NULL || eventList->head == NULL) {
        return emptyEvent;
    }
    return eventList->head->event;
}

int size(const EventList* eventList) {
    int count = 0;
    EventListNode* current = eventList->head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

void fetch(EventList* eventList, InstructionQueue* queue) {
    if (eventList == NULL || queue == NULL) {
        return;
    }
    while (size(eventList) < queue->front->instr.num_dependents) {
        Instruction instr = getFront(queue);
        Event event = newEvent(IF, instr);
        EventListNode* newNode = malloc(sizeof(EventListNode));
        if (newNode == NULL) {
            return; // Handle memory allocation error
        }
        newNode->event = event;
        newNode->next = NULL;
        if (eventList->head == NULL) {
            eventList->head = newNode;
            eventList->tail = newNode;
        } else {
            eventList->tail->next = newNode;
            eventList->tail = newNode;
        }
        pop(queue);
    }
}



void processIF(DependencyTracker* dependencyTracker, int width, EventList* eventList) {
    if (dependencyTracker == NULL || eventList == NULL) {
        return;
    }
    int count = 0;
    while (size(eventList) > 0 && count < width) {
        Event event = front(eventList);
        if (event.stage == IF) {
            // need to process the instruction in the IF stage
            // then we will update dependency tracker, if needed
            // then we will move the instruction to the next stage in the pipeline
            popEvent(eventList);
            count++;
        } else {
            break; // No more instructions in IF stage or pipeline width limit reached
        }
    }
}

void processID(DependencyTracker* dependencyTracker, int width, EventList* eventList) {
    if (dependencyTracker == NULL || eventList == NULL) {
        return;
    }
    int count = 0;
    while (size(eventList) > 0 && count < width) {
        Event event = front(eventList);
        if (event.stage == ID) {
            // need to process the instruction in the ID stage
            // then we will update dependency tracker, if needed
            // then we will move the instruction to the next stage in the pipeline
            popEvent(eventList);
            count++;
        } else {
            break; // No more instructions in ID stage or pipeline width limit reached
        }
    }
}

void processEX(DependencyTracker* dependencyTracker, int width, EventList* eventList) {
    if (dependencyTracker == NULL || eventList == NULL) {
        return;
    }
    int count = 0;
    while (size(eventList) > 0 && count < width) {
        Event event = front(eventList);
        if (event.stage == EX) {
            // .......
            popEvent(eventList);
            count++;
        } else {
            break; // No more instructions in EX stage or pipeline width limit reached
        }
    }
}

void processMEM(DependencyTracker* dependencyTracker, int width, EventList* eventList) {
    if (dependencyTracker == NULL || eventList == NULL) {
        return;
    }
    int count = 0;
    while (size(eventList) > 0 && count < width) {
        Event event = front(eventList);
        if (event.stage == MEM) {
            // .......
            popEvent(eventList);
            count++;
        } else {
            break; // No more instructions in MEM stage or pipeline width limit reached
        }
    }
}

void processWB(DependencyTracker* dependencyTracker, int width, EventList* eventList) {
    if (dependencyTracker == NULL || eventList == NULL) {
        return;
    }
    int count = 0;
    while (size(eventList) > 0 && count < width) {
        Event event = front(eventList);
        if (event.stage == WB) {
            // .......
            popEvent(eventList);
            count++;
        } else {
            break; // No more instructions in WB stage or pipeline width limit reached
        }
    }
}


void freeEventList(EventList* eventList) {
    EventListNode* current = eventList->head;
    while (current != NULL) {
        EventListNode* temp = current;
        current = current->next;
        free(temp);
    }
    eventList->head = NULL;
    eventList->tail = NULL;
}
