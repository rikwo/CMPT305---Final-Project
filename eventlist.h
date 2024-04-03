#ifndef EVENTLIST_H
#define EVENTLIST_H

#include "dependency.h"
#include "events.h"
#include "input.h"

typedef struct {
    Event event;
    struct EventListNode* next;
} EventListNode;

typedef struct {
    EventListNode* head;
    EventListNode* tail;
    int size;
} EventList;

EventList* initEventList(DependencyTracker* dependencyTracker, InstructionQueue* instructionQueue, int width);

void pop(EventList* eventList);

Event front(const EventList* eventList);

int size(const EventList* eventList);

void fetch();

void processIF();

void processID();

void processEX();

void processMEM();

void processWB();

void freeEventList(EventList* eventList);

#endif