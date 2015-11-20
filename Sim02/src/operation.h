/*
* Filename: Operation.h
* Specifications for the Operation object. 
*/

#ifndef OPERATION_H
#define OPERATION_H


/* Model of a program operation, such as processing and I/O */
struct Operation
{
    char type; // S (OS), A (Program), P (Processing), I (Input), or O (Output)
    std::string description; // end, hard drive, keyboard, monitor, run, or start
    int cycles;
    int cycle_duration;
    int duration;
};

#endif // OPERATION_H