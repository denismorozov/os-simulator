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
#include <queue>

#include "operation.h"

/* Possible PCB states */
enum State
{
    START, READY, RUNNING, EXIT
};
    
/* Models a program which the OS can load and run */
class Program
{
public:
    Program();
    ~Program();

    /* Set */
    void add_operation( Operation operation );

    /* Get */
    bool done() const;
    Operation next(); // pops queue
    int remaining_time() const;
    int remaining_operations() const;

    /* Overloaded operator for comparisons */
    bool operator>( const Program &other ) const;

    /* Public members */        
    State state = START; // Current program state
    int id = 0; // ID of program, != 0 if program already started

private:
    // Queue containing all program operations
    std::queue<Operation> operations_; 
    
    // Remaining running time of program
    int remaining_time_ = 0; 
};

#endif // PROGRAM_H