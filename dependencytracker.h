#ifndef DEPENDENCYTRACKER_H
#define DEPENDENCYTRACKER_H

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct {
    unsigned first;
    bool second;
} Pair;

typedef struct DequeNode{
    Pair pair;
    struct DequeNode* next;
} DequeNode;

typedef struct {
    DequeNode* front;
    DequeNode* rear;
} Deque;

Deque* initDeque();
void pushFront(Deque* deque, Pair pair);
void popFront(Deque* deque);

typedef struct HashNode {
    char* key;
    Deque* value;
    struct HashNode* next;
} HashNode;

typedef struct {
    size_t size;
    HashNode** array;
} HashMap;

HashMap* initHashMap(size_t size);
size_t hash(const char* key, size_t size);
void insert(HashMap* map, const char* key, Deque* value);
Deque* get(HashMap* map, const char* key);

typedef struct {
    // Used to track the # of instructions in each stage of execution. It typically represents the number of instructions currently in each stage of the pipeline (e.g., fetch, decode, execute, memory access, write back).
    unsigned stageCount[5];
    // This array stores the index of the next instruction to be executed in each stage of the pipeline.
    unsigned nextInstr[5];
    // This array is used to identify hazards in the pipeline. A hazard indicates a condition that prevents an instruction from proceeding to the next stage due to resource conflicts or data dependencies.
    unsigned hazards[5];
    // This is a pointer to a hash map that stores instructions along with their dependencies. Each instruction is identified by a unique key (e.g., program counter value), and its dependencies are stored in a deque. 
    HashMap* instructions;
} DependencyTracker;

DependencyTracker* newDependencyTracker();
void freeDependecyTracker(DependencyTracker* dependencyTracker);

#endif
