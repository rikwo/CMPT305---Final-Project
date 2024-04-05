#include "input.h"

// Function to initialize a queue
InstructionQueue* initQueue() {
    InstructionQueue* queue = (InstructionQueue*)malloc(sizeof(InstructionQueue));
    if (queue == NULL) {
        // Memory allocation failed
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Function to push an instruction into the instruction queue
void push(InstructionQueue* queue, Instruction instr) {
    // Create a new node for the instruction
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed for new instruction node\n");
        return;
    }
    
    // Set the instruction in the node
    newNode->instr = instr;
    newNode->next = NULL;

    // If the queue is empty, set the new node as both front and rear
    if (isEmpty(*queue)) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        // Otherwise, add the new node to the rear of the queue
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}


// Function to populate a queue with instructions from an input file
void populateQueue(InstructionQueue* queue, const char* fileName, int startLine, int instrCount) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", fileName);
        return;
    }
    char buffer[256];
    int currentLine = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL && instrCount > 0) {
        if (currentLine >= startLine) {
            char pc[10]; // Assuming PC is a hexadecimal value and at most 10 characters
            int type;
            char deps[256] = ""; // Assuming dependency list can be at most 256 characters
        
            // Parse instruction details from the buffer
            if (sscanf(buffer, "%9[^,],%d,%255[^\n]", pc, &type, deps)  >= 2) {
                // Create Instruction object
                Instruction instr;
                instr.programCounter = strdup(pc);
                instr.type = (instrType)type;
                instr.num_dependents = 0;
                instr.dependents = NULL;
                // If there are dependencies, parse them
                if (strlen(deps) > 0) {
                   // Create a copy of the dependency string to tokenize
                    char deps_copy[256];
                    strcpy(deps_copy, deps);
                    
                    // Count the number of dependencies
                    int num_deps = 0;
                    char* token = strtok(deps_copy, ",");
                    while (token != NULL) {
                        num_deps++;
                        token = strtok(NULL, ",");
                    }
                    // Allocate memory for dependencies
                    instr.dependents = (char**)malloc(num_deps * sizeof(char*));
                    instr.num_dependents = num_deps;

                    // Parse dependencies and copy into the instruction object
                    token = strtok(deps, ",");
                    int i = 0;
                    while (token != NULL) {
                        instr.dependents[i++] = strdup(token);
                        token = strtok(NULL, ",");
                    } }
                else {
                    // Add an empty string to dependents if no dependencies are present
                    instr.num_dependents = 0;
                    instr.dependents = NULL;
                }
                // Push the instruction into the queue
                push(queue, instr);
                instrCount--;
            } else {
                fprintf(stderr, "Error parsing instruction at line %d\n", currentLine);
            }
        }
        memset(buffer, 0, sizeof(buffer));
        currentLine++;
    }
    fclose(file);
}

// Function to remove the front instruction
void pop(InstructionQueue* queue) {
    if (isEmpty(*queue)) {
        return;
    }
    QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
}

// Function to get the front instruction
Instruction getFront(const InstructionQueue* queue) {
    Instruction emptyInstr = {"", Integer, NULL, 0, 0};
    if (isEmpty(*queue)) {
        return emptyInstr;
    }
    return queue->front->instr;
}

// Function to check if queue is empty
bool isEmpty(const InstructionQueue queue) {
    return queue.front == NULL;
}

// Function to free memory allocated to queue
void freeQueue(InstructionQueue* queue) {
    while (!isEmpty(*queue)) {
        pop(queue);
    }
    free(queue);
}
