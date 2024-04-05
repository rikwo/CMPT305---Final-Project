#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "input.h"
#include "instruction.h"
#include "simulator.h"
#include "dependencytracker.h"
#include "eventlist.h"


Simulator* newSimulator(const char* fileName, int startLine, int expectedInstructionCount, int width) {
    Simulator* simulator = (Simulator*)malloc(sizeof(Simulator));
    if (simulator == NULL) {
        //memory allocation failed
        return NULL;
    }

    simulator->fileName = fileName;
    simulator->startLine = startLine;
    simulator->expectedInstructionCount = expectedInstructionCount;
    simulator->width = width;
    simulator->instructionsExecuted = 0;
    simulator->clockCycle = 0;
    memset(simulator->instructionCount, 0, sizeof(simulator->instructionCount));
    simulator->time = 0.0;
    return simulator;
}

void print(Simulator *simulator) {
    //to do
    double histogram[5];
    for (int i = 0; i < 5; i++) {
        if (simulator->instructionCount[i]) {
            histogram[i] = ((double) simulator->instructionCount[i] * 100) / (double) simulator->expectedInstructionCount;
        }
        else {
            histogram[i] = 0;
        }
    }

    printf("-----Simple Processor Pipeline Simulator-----\n");
    printf("%-21s%24s\n", "Trace File", simulator->fileName);
    printf("%-21s%24d\n", "Starting Instruction", simulator->startLine);
    printf("%-21s%24d\n", "Instruction Count", simulator->expectedInstructionCount);
    printf("%-21s%24d\n", "Pipeline Width", simulator->width);

    printf("------------Simulation Statistics------------\n");
    printf("%-30s%15.4lf\n", "Simulation Runtime (Seconds)", simulator->time);
    printf("%-30s%15u\n", "Total Execution Time (Cycles)", simulator->clockCycle);
    printf("%-30s%15u\n", "Total Instructions Executed", simulator->instructionsExecuted);

    printf("------------Instruction Histogram------------\n");
    printf("%-25s%20s\n", "Instruction Type", "Percentage (%%)");
    printf("%-31s%-14.4lf\n", "(1) Integer", histogram[0]);
    printf("%-31s%-14.4lf\n", "(2) Float", histogram[1]);
    printf("%-31s%-14.4lf\n", "(3) Branch", histogram[2]);
    printf("%-31s%-14.4lf\n", "(4) Load", histogram[3]);
    printf("%-31s%-14.4lf\n", "(5) Store", histogram[4]);


}

void start(Simulator* simulator) {
    clock_t startTime = clock();

    InstructionQueue *instructionQueue = initQueue();
    populateQueue(instructionQueue, simulator->fileName, simulator->startLine, simulator->expectedInstructionCount);
    // Print all instructions in the queue
    /*printf("Instructions in the queue:\n");
    QueueNode* current = instructionQueue->front;
    while (current != NULL) {
        printf("PC: %s, Type: %d, Number of Dependents: %zu, Dependencies: ", 
            current->instr.programCounter, 
            current->instr.type, 
            current->instr.num_dependents);
        for (size_t i = 0; i < current->instr.num_dependents; i++) {
            printf("%s", current->instr.dependents[i]);
            if (i < current->instr.num_dependents - 1) {
                printf(", ");
            }
        }
        printf("\n");
        current = current->next;
    }*/

    DependencyTracker *dependencyTracker = newDependencyTracker();
    EventList *eventList = initEventList(dependencyTracker, instructionQueue, simulator->width);
    
    //as long as there are events and the instructions remaining, continue
   /* while (eventList->size > 0 || (instructionQueue->front != NULL && instructionQueue->rear != NULL)) {
        unsigned instructionCount = eventList->size;
        for (unsigned int i = 0; i < instructionCount; i++) {
            Event current = front(eventList);
            switch (current.stage) {
                case IF:
                    processIF(dependencyTracker, simulator->width, eventList);
                    break;
                case ID:
                    processID(dependencyTracker, simulator->width, eventList);
                    break;
                case EX:
                    processEX(dependencyTracker, simulator->width, eventList);
                    break;
                case MEM:
                    processMEM(dependencyTracker, simulator->width, eventList);
                    break;
                case WB:
                    processWB(dependencyTracker, simulator->width, eventList);

                    simulator->instructionCount[current.instr.type - 1]++;
                    simulator->instructionsExecuted++;
                    break;

            }
            popEvent(eventList);
        }
        fetch(eventList, instructionQueue);
        simulator->clockCycle++;
    }*/
    clock_t endTime = clock();
    simulator->time = (double)(endTime - startTime)/ CLOCKS_PER_SEC;
    //print(simulator);
}