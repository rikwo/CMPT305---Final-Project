#include <stdlib.h>
#include <string.h>

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

//type of program instruction
typedef enum {
    Integer = 1,
    Float = 2,
    Branch = 3,
    Load = 4,
    Store = 5
}instrType;

//structure representing instructions
typedef struct Instruction {
    char* programCounter; //hexadecimal value representing the isntruction address
    instrType type; //type of instruction
    char** dependents; //list of program counter values that the current instruction is dependent on
    size_t num_dependents; //number of dependents
    unsigned lineNum; //line number of the associated instruction
} Instruction;

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
#endif