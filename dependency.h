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

typedef struct {
    Pair pair;
    struct DequeNode* next;
} DequeNode;

typedef struct {
    DequeNode* front;
    DequeNode* rear;
} Deque;

Deque* initDeque() {
    Deque* deque = (Deque*)malloc(sizeof(Deque));
    if (deque == NULL) {
        //memory allocation for initializing deque failed
        return NULL;
    }
    deque->front = NULL;
    deque->rear = NULL;
    return deque;
}

void pushFront(Deque* deque, Pair pair) {
    DequeNode* newNode = (DequeNode*)malloc(sizeof(DequeNode));
    if (newNode == NULL) {
        //allocation of memmory for new DequeNode failed
        return;
    }
    newNode->pair = pair;
    newNode->next = deque->front;
    deque->front = newNode;
    if (deque->rear == NULL) {
        deque->rear = newNode;
    }
}

void popFront(Deque* deque) {
    if (deque->front == NULL) {
        //deque is empty
        return;
    }
    DequeNode* temp = deque->front;
    deque->front = deque->front->next;
    if (deque->front == NULL) {
        deque->rear = NULL;
    }
    free(temp);
}

typedef struct HashNode {
    char* key;
    Deque* value;
    struct HashNode* next;
} HashNode;

typedef struct {
    size_t size;
    HashNode** array;
} HashMap;

HashMap* initHashMap(size_t size) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (map == NULL) {
        // memory allocation for hash map failed
        return NULL;
    }
    map->size = size;
    map->array = (HashNode**)calloc(size, sizeof(HashNode*));
    if (map->array == NULL) {
        //memory allocation for array of map failed
        free(map);
        return NULL;
    }
    return map;
}

size_t hash(const char* key, size_t size) {
    size_t hashValue = 0;
    for (size_t i = 0; key[i] != '\0'; i++) {
        hashValue = 31 * hashValue + key[i];
    }
    return hashValue % size;
}

void insert(HashMap* map, const char* key, Deque* value) {
    size_t index = hash(key, map->size);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    if (newNode == NULL) {
        //memory allocation for new node failed
        return;
    }
    newNode-> key = strdup(key);
    if (newNode->key == NULL) {
        //memory allocation failed
        free(newNode);
        return;
    }
    newNode->value = value;
    newNode->next = map->array[index];
    map->array[index] = newNode;
}

Deque* get(HashMap* map, const char* key) {
    size_t index = hash(key, map->size);
    HashNode* current = map->array[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

typedef struct {
    unsigned stageCount[5];
    unsigned nextInstr[5];
    unsigned hazards[5];

    //need some sort of structure to keep track of whether an instruction is completed and can be used by it's dependents
    HashMap* instructions;
} DependencyTracker;

DependencyTracker* newDependencyTracker() {
    DependencyTracker* dependencyTracker = (DependencyTracker*)malloc(sizeof(DependencyTracker));
    if (dependencyTracker == NULL) {
        //memory allocation for dependency tracker failed
        return NULL;
    }
    for (int i = 0; i < 5; i++) {
        dependencyTracker->stageCount[i] = 0;
        dependencyTracker->nextInstr[i] = 0;
        dependencyTracker->hazards[i] = 0;
    }
    dependencyTracker->instructions = initHashMap(10000000);
    if (dependencyTracker->instructions == NULL) {
        //memory allocation for instructions in dependency tracker failed
        free(dependencyTracker);
        return NULL;
    }
    return dependencyTracker;
}

void freeDependecyTracker(DependencyTracker* dependencyTracker) {
    if (dependencyTracker == NULL) {
        return;
    }
    for (size_t i = 0; i < dependencyTracker->instructions->size; i++) {
        HashNode* current = dependencyTracker->instructions->array[i];
        while (current != NULL) {
            HashNode* temp = current;
            current = current ->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(dependencyTracker->instructions->array);
    free(dependencyTracker->instructions);
    free(dependencyTracker);
}
#endif