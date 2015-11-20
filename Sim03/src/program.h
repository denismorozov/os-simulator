/*
* Filename: Program.h
* Specifications for the Program object. Represents a Process Control Block.
*/

#ifndef PROGRAM_H
#define PROGRAM_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <deque>

#include "operation.h"

/* Possible PCB states */
enum State
{
    START, BLOCKED, READY, RUNNING, EXIT
};
    
/* Models a program which the OS can load and run */
class Program
{
public:
    Program();
    ~Program();

    /* Add a new operation to the back of the queue */
    void add_operation( Operation operation );

    /* Return operation to the front of the queue */
    void return_operation( Operation operation );

    /* Pops next operation and returns it */
    Operation next();

    /* Information */
    bool done() const;
    int remaining_time() const;
    int remaining_operations() const;

    /* Public members */        
    State state = START; // Current program state
    int id = 0; // ID of program, != 0 if program already started

private:
    // Queue containing all program operations
    std::deque<Operation> operations_; 
    
    // Remaining running time of program
    int remainingProgramTime_ = 0; 
};

#endif // PROGRAM_H