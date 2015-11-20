/*
* Filename: Operation.h
* Specifications for the Operation object. 
*/

#ifndef OPERATION_H
#define OPERATION_H

#include <string>


/* Model of a program operation, such as processing and I/O */
class Operation
{
public:
    // Run a single cycle
    int run();

    // Operation complete?
    bool done();

    // status
    int cycles;
    int duration = 0; // remaining cycles * time per cycle

    // Operation information
    char type; // S (OS), A (Program), P (Processing), I (Input), or O (Output)
    std::string description; // end, hard drive, keyboard, monitor, run, printer, or start
    int cycleTime;
};

#endif // OPERATION_H
