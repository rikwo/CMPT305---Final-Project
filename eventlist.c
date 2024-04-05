#include "eventlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Event newEvent(Stage stage, Instruction instr) {
    Event event;
    event.stage = stage;
    event.instr = instr;
    return event;
}
EventList* initEventList(DependencyTracker* dependencyTracker, InstructionQueue* instructionQueue, int width) {
    if (dependencyTracker == NULL || instructionQueue == NULL || width <= 0) {
        return NULL;
    }
    
    EventList* eventList = malloc(sizeof(EventList));
    if (eventList == NULL) {
        // Memory allocation failed
        return NULL;
    }
    
    eventList->head = NULL;
    eventList->tail = NULL;
    eventList->size = 0;
    
    // Fetch initial events from the instruction queue depending upon the width
    int i = 1;
    while(i <= width){
        fetch(eventList, instructionQueue);
        i++;
    }
    
    return eventList;
}

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
    while (size(eventList) <= (int)(queue->front->instr.num_dependents)) {
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



void processIF(DependencyTracker* dependencyTracker, unsigned width, EventList* eventList) {
    /*if (dependencyTracker == NULL || eventList == NULL) {
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
    }*/

    Instruction instruction = eventList->head->event.instr;

    if (dependencyTracker->stageCount[ID] == width || dependencyTracker->nextInstr[IF] != instruction.lineNum) {
        EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
        newNode->event = eventList->head->event;
        newNode->next = NULL;
        if (eventList->head == eventList->tail) {
            eventList->head = eventList->tail = NULL;
        }
        else {
            eventList->head = eventList->head->next;
        }
        if (eventList->size > 0) {
            eventList->size--;
        }
        if (newNode != NULL) {
            if (eventList->tail == NULL) {
                eventList->head = eventList->tail = newNode;
            }
            else {
                eventList->tail->next = newNode;
                eventList->tail = newNode;
            }
            eventList->size++;
        }
        return;
    }

    dependencyTracker->stageCount[IF]--;
    dependencyTracker->stageCount[ID]++;
    dependencyTracker->nextInstr[IF]++;

    EventListNode* temp = eventList->head;
    eventList->head = eventList->head->next;
    if (eventList->size > 0) {
        eventList->size--;
    }
    free(temp);
}

void processID(DependencyTracker* dependencyTracker, unsigned int width, EventList* eventList) {
    /*if (dependencyTracker == NULL || eventList == NULL) {
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
    }*/
    if (eventList->head == NULL) {
        //eventList is empty and therefore no instriction to process
        return;
    }

    Instruction instruction = eventList->head->event.instr;

    for (size_t i = 0; i < instruction.num_dependents; i++) {
        char key[20];
        sprintf(key, "%s", instruction.dependents[i]);
        Deque* dependencyDeque = get(dependencyTracker->instructions, key);
        if (dependencyDeque != NULL) {
            DequeNode* search = dependencyDeque->front;
            while (search != NULL) {
                if (search->pair.first < instruction.lineNum) {
                    if (!search->pair.second) {
                    EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
                    newNode->event.stage = ID;
                    newNode->event.instr = instruction;
                    newNode->next = NULL;
                    if (eventList->head == eventList->tail) {
                        eventList->head = eventList->tail = newNode;
                    }
                    else {
                        eventList->tail->next = newNode;
                        eventList->tail = newNode;
                    }
                    eventList->size++;
                    return;
                    }
                    else {
                        break;
                    }
                }
                search = search->next;
            }
        }
    }

    if (dependencyTracker->stageCount[EX] == width || dependencyTracker->nextInstr[ID] != instruction.lineNum) {
        //reschedule current instruction if EX stage is full or not next program order instruction
        EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
        newNode->event.stage = ID;
        newNode->event.instr = instruction;
        newNode->next = NULL;
        if (eventList->head == eventList->tail) {
            eventList->head = eventList->tail = newNode;
        }
        else {
            eventList->tail->next = newNode;
            eventList->tail = newNode;
        }
        eventList->size++;
        return;
    }

    if (instruction.type == Integer || instruction.type == Float) {
        //reschedule current instruction if structural dependency not available
        if (dependencyTracker->hazards[instruction.type - 1] != 0) {
            EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
            newNode->event.stage = ID;
            newNode->event.instr = instruction;
            newNode->next = NULL;
            if (eventList->head == eventList->tail) {
                eventList->head = eventList->tail = newNode;
            }
            else {
                eventList->tail->next = newNode;
                eventList->tail = newNode;
            }
            eventList->size++;
            return;
        }
        dependencyTracker->hazards[instruction.type - 1] = instruction.lineNum;
    }

    EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
    newNode->event.stage = EX;
    newNode->event.instr = instruction;
    newNode->next = NULL;
    if (eventList->head == eventList->tail) {
        eventList->head = eventList->tail = newNode;
    }
    else {
        eventList->tail->next = newNode;
        eventList->tail = newNode;
    }
    eventList->size++;
    dependencyTracker->stageCount[ID]--;
    dependencyTracker->stageCount[EX]++;
    dependencyTracker->nextInstr[ID]++;
}

void processEX(DependencyTracker* dependencyTracker, unsigned int width, EventList* eventList) {
    /*if (dependencyTracker == NULL || eventList == NULL) {
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
    }*/
    if (eventList->head == NULL) {
        return;
    }
    Instruction instruction = eventList->head->event.instr;

    if (instruction.type == Integer || instruction.type == Float || instruction.type == Branch) {
        //update structural/control hazards
        if (dependencyTracker->hazards[instruction.type - 1] == instruction.lineNum) {
            dependencyTracker->hazards[instruction.type - 1] = 0;
        }

        char key[20];
        sprintf(key, "%s", instruction.programCounter);
        Deque* dependencyDeque = get(dependencyTracker->instructions, key);
        if (dependencyDeque != NULL) {
            DequeNode* search = dependencyDeque->front;
            while (search != NULL) {
                if (search->pair.first == instruction.lineNum) {
                    search->pair.second = true;
                    break;
                }
                search = search->next;
            }
        }
    }

    if (dependencyTracker->stageCount[MEM] == width || dependencyTracker->nextInstr[EX] != instruction.lineNum) {
        EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
        newNode->event.stage = EX;
        newNode->event.instr = instruction;
        newNode->next = NULL;
        if (eventList->head == eventList->tail) {
            eventList->head = eventList->tail = newNode;
        }
        else {
            eventList->tail->next = newNode;
            eventList->tail = newNode;
        }
        eventList->size++;
        return;
    }

    if (instruction.type == Load || instruction.type == Store) {
        if (dependencyTracker->hazards[instruction.type - 1] != 0) {
            EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
            newNode->event.stage = EX;
            newNode->event.instr = instruction;
            newNode->next = NULL;
            if (eventList->head == eventList->tail) {
                eventList->head = eventList->tail = newNode;
            }
            else {
                eventList->tail->next = newNode;
                eventList->tail = newNode;
            }
            eventList->size++;
            return;
        }
        dependencyTracker->hazards[instruction.type - 1] = instruction.lineNum;
    }
    EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
    newNode->event.stage = MEM;
    newNode->event.instr = instruction;
    newNode->next = NULL;
    if (eventList->head == eventList->tail) {
        eventList->head = eventList->tail = newNode;
    }
    else {
        eventList->tail->next = newNode;
        eventList->tail = newNode;
    }
    eventList->size++;

    dependencyTracker->stageCount[EX]--;
    dependencyTracker->stageCount[MEM]++;
    dependencyTracker->nextInstr[EX]++;
}

void processMEM(DependencyTracker* dependencyTracker, unsigned int width, EventList* eventList) {
    /*if (dependencyTracker == NULL || eventList == NULL) {
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
    }*/
    if (eventList->head == NULL) {
        return;
    }
    Instruction instruction = eventList->head->event.instr;

    if (instruction.type == Load || instruction.type == Store) {
        if (dependencyTracker->hazards[instruction.type - 1] == instruction.lineNum) {
            dependencyTracker->hazards[instruction.type - 1] = 0;
        }

        char key[20];
        sprintf(key, "%s", instruction.programCounter);
        Deque* depedencyDeque = get(dependencyTracker->instructions, key);
        if (depedencyDeque != NULL) {
            DequeNode* search = depedencyDeque->front;
            while (search != NULL) {
                if (search->pair.first == instruction.lineNum) {
                    search->pair.second = true;
                    break;
                }
                search = search->next;
            }
        }
    }

    if (dependencyTracker->stageCount[WB] == width || dependencyTracker->nextInstr[MEM] != instruction.lineNum) {
        EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
        newNode->event.stage = MEM;
        newNode->event.instr = instruction;
        newNode->next = NULL;
        if (eventList->head == eventList->tail) {
            eventList->head = eventList->tail = newNode;
        }
        else {
            eventList->tail->next = newNode;
            eventList->tail = newNode;
        }
        eventList->size++;
        return;
    }

    EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
    newNode->event.stage = WB;
    newNode->event.instr = instruction;
    newNode->next = NULL;
    if (eventList->head == eventList->tail) {
        eventList->head = eventList->tail = newNode;
    }
    else {
        eventList->tail->next = newNode;
        eventList->tail = newNode;
    }
    eventList->size++;

    dependencyTracker->stageCount[MEM]--;
    dependencyTracker->stageCount[WB]++;
    dependencyTracker->stageCount[MEM]++;
}

void processWB(DependencyTracker* dependencyTracker, EventList* eventList) {
    /*if (dependencyTracker == NULL || eventList == NULL) {
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
    }*/
    if (eventList->head == NULL) {
        return;
    }

    Instruction instruction = eventList->head->event.instr;
    if (dependencyTracker->nextInstr[WB] != instruction.lineNum) {
        EventListNode* newNode = (EventListNode*)malloc(sizeof(EventListNode));
        newNode->event.stage = WB;
        newNode->event.instr = instruction;
        newNode->next = NULL;
        if (eventList->head == eventList->tail) {
            eventList->head = eventList->tail = newNode;
        }
        else {
            eventList->tail->next = newNode;
            eventList->tail = newNode;
        }
        eventList->size++;
        return;
    }
    dependencyTracker->stageCount[WB]--;
    dependencyTracker->nextInstr[WB]++;
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