/*
* Filename: Program.h
* Brief: Specifications for the Program and Operation objects. 
*/

#ifndef PROGRAM_H
#define PROGRAM_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>

/* Model of program operations, such as processing and I/O */
struct Operation
{
    char type; // S (OS), A (Program), P (Processing), I (Input), or O (Output)
    std::string description; // end, hard drive, keyboard, monitor, run, or start
    int duration; // cycles
};

/* Models a program which the OS can load and run */
class Program
{
public:
    Program( const std::string meta_data_file_path );
    ~Program();
    /* Queue containing all program operations */
    std::queue<Operation> operations;

private:
    // fills queue with program's operations
    void load_meta_data( const std::string file_path );        
};

#endif // PROGRAM_H