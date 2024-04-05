#include <stdlib.h>
#include <string.h>
#include "instruction.h"


Instruction* newInstruction(const char* programCounter, instrType type, const char** dependents, size_t num_dependents, unsigned lineNum) {
    //create new instruction
    Instruction* instr = (Instruction*)malloc(sizeof(Instruction));
    if (instr == NULL) {
        //memory allocation failed
        return NULL;
    }

    //set program counter
    instr->programCounter = strdup(programCounter);
    if (instr->programCounter == NULL) {
        //program counter allocation failed
        free(instr);
        return NULL;
    }

    //set type
    instr->type = type;
    
    //set dependents
    instr->dependents = (char**)malloc(num_dependents * sizeof(char*));
    if  (instr->dependents == NULL) {
        //dependent memory allocation failed
        free(instr->programCounter);
        free(instr);
        return NULL;
    }
    for (size_t index = 0; index < num_dependents; index++) {
        //set each individual dependent
        instr->dependents[index] = strdup(dependents[index]);
        if (instr->dependents[index] == NULL) {
            //current dependent was not set correctly or does not exist
            for (size_t j = 0; j < index; j++) {
                free(instr->dependents[j]);
            }
            free(instr->dependents);
            free(instr->programCounter);
            free(instr);
            return NULL;
        }
    }

    //set number of dependents
    instr->num_dependents = num_dependents;

    //set line number
    instr->lineNum = lineNum;

    return instr;
}

void freeInstruction(Instruction* instr) {
    if (instr == NULL) {
        return;
    }
    free(instr->programCounter);
    for (size_t index = 0; index < instr->num_dependents; index++) {
        free(instr->dependents[index]);
    }
    free(instr->dependents);
    free(instr);
}
