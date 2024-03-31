#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    const char* fileName; //name of the trace file
    int startLine; //first line that will be read 
    int instructionCount; //number of instructions from the file to read
    int width; //number of pipelines in the processor
    unsigned instructionsExecuted; //total number of instructions executed
    unsigned clockCycle; //current clock cycle
    unsigned instructionCount[5]; //count of instructions of each type 
    double time; //total time elapsed
} Simulator;

Simulator* newSimulator(const char* fileName, int startLine, int instructionCount, int width) {
    Simulator* simulator = (Simulator*)malloc(sizeof(Simulator));
    if (simulator == NULL) {
        //memory allocation failed
        return NULL;
    }

    simulator->fileName = fileName;
    simulator->startLine = startLine;
    simulator->instructionCount = instructionCount;
    simulator->width = width;
    simulator->instructionsExecuted = 0;
    simulator->clockCycle = 0;
    memset(simulator->instructionCount, 0, sizeof(simulator->instructionCount));
    simulator->time = 0.0;
    return simulator;
}

void start(Simulator* simulator) {
    //to do
}