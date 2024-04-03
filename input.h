#ifndef INPUTQUEUE_H
#define INPUTQUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "instruction.c"

typedef struct queueNode {
    Instruction instr;
    struct queueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} InstructionQueue;

//function to initialize a queue
InstructionQueue* initQueue();

//function to populate a queue with instruction from input file
void populateQueue(InstructionQueue *queue, const char* fileName, int startLine, int instrCount);

//function to remove the front instruction
void pop(InstructionQueue* queue);

//function to get the front instruction
Instruction getFront(const InstructionQueue* queue);

//function to check if queue is empty
bool isEmpty(const InstructionQueue queue);

//function to free memory allocated to queue
void freeQueue(InstructionQueue* queue);

#endif