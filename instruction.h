#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdlib.h>

typedef enum {
    Integer = 1,
    Float = 2,
    Branch = 3,
    Load = 4,
    Store = 5
} instrType;

typedef struct Instruction {
    char* programCounter;
    instrType type;
    char** dependents;
    size_t num_dependents;
    unsigned lineNum;
} Instruction;

Instruction* newInstruction(const char* programCounter, instrType type, const char** dependents, size_t num_dependents, unsigned lineNum);
void freeInstruction(Instruction* instr);

#endif
