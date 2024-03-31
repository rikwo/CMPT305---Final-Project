#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "simulator.c"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Insufficient number of parameters.\n");
        return 1;
    }

    const char* fileName = argv[1];
    int startLine = atoi(argv[2]);
    int instructionCount = atoi(argv[3]);
    int width = atoi(argv[4]);

    if (startLine <= 0 || instructionCount <= 0 || width <= 0) {
        fprintf(stderr, "Invalid parameters\n");
        return 1;
    }

    Simulator* simulator = newSimulator(fileName, startLine, instructionCount, width);
    if (simulator == NULL) {
        fprintf(stderr, "Error creating new simulator");
        return 1;
    }

    start(simulator);
    freeSimulator(simulator);

    return 0;
}