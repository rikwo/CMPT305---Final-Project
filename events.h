#include "instruction.c"

#ifndef STAGE_ENUM
#define STAGE_ENUM
typedef enum { //stages of the 5 stage cycle represented numerically
    IF = 0,
    ID = 1,
    EX = 2,
    MEM = 3,
    WB = 4
} Stage;
#endif

typedef struct {
    Stage stage; //current stage of the instruction
    Instruction instr; //instruction associated with event
} Event;

Event newEvent(Stage stage, Instruction instr) {
    Event event;
    event.stage = stage;
    event.instr = instr;
    return event;
}