#ifndef PROGRAM_H
#define PROGRAM_H

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>

struct Operation
{
    char type;
    std::string description;
    int duration;
};

class Program
{
public:
    Program( const std::string file_path );
    ~Program();
    std::queue<Operation> operations;

private:
    // fills queue with program's operations
    void load_meta_data( const std::string file_path );        
};

#endif // PROGRAM_H