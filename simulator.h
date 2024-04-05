#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <stdbool.h>

#include "instruction.h"

typedef struct Simulator {
    const char* fileName;
    int startLine;
    int expectedInstructionCount;
    int width;
    unsigned instructionsExecuted;
    unsigned clockCycle;
    unsigned instructionCount[5];
    double time;
} Simulator;

Simulator* newSimulator(const char* fileName, int startLine, int expectedInstructionCount, int width);
void print(Simulator *simulator);
void start(Simulator* simulator);

#endif
