#ifndef DEPENDENCYTRACKER_H
#define DEPENDENCYTRACKER_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    unsigned stageCount[5];
    unsigned nextInstr[5];
    unsigned hazards[5];

    //need some sort of structure to keep track of whether an instruction is completed and can be used by it's dependents
} dependencyTracker;

#endif