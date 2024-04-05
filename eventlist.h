#ifndef EVENT_AND_EVENTLIST_H
#define EVENT_AND_EVENTLIST_H

#include "instruction.h"
#include "dependencytracker.h"
#include "input.h"

#ifndef STAGE_ENUM
#define STAGE_ENUM
typedef enum { //stages of the 5 stage cycle represented numerically
    IF = 0,
    ID = 1,
    EX = 2,
    MEM = 3,
    WB = 4
} Stage;
#endif

typedef struct {
    Stage stage; //current stage of the instruction
    Instruction instr; //instruction associated with event
} Event;


typedef struct EventListNode {
    Event event;
    struct EventListNode* next;
} EventListNode;

typedef struct {
    EventListNode* head;
    EventListNode* tail;
    int size;
} EventList;


EventList* initEventList(DependencyTracker* dependencyTracker, InstructionQueue* instructionQueue, int width);

void popEvent(EventList* eventList);

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
