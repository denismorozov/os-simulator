#ifndef PROGRAM_H
#define PROGRAM_H

#include <queue>
#include <string>
#include <stdexcept>

struct Operation
{
    enum type
    {
        S, // Simulation
        A, // Program
        O, // Output
        I, // Input
        P // Process
    };
    int duration;
    std::string description;
};

class Program
{
public:
    Program( const std::string filePath );
    ~Program();
    std::queue<Operation> operations;

private:
    // fills queue with program's operations
    void loadMetaData( const std::string filePath );        
};

#endif // PROGRAM_H