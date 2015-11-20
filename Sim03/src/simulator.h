/*
* Filename: Simulator.h
* Specifications for the Simulator object
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cassert>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <thread>
#include <queue>

#include "operation.h"
#include "program.h"

/* OS Simulator. Loads a configuration file and a program to run */
class Simulator
{
public:
    // Constructor loads the config file and initializes Program object
    Simulator( const std::string filePath );
    ~Simulator();

    // Runs the simulation
    void run();

private:

    /*** Helper functions ***/

    // Helper function that processes each individual program operation
    void process_operation( Program &program );

    // Helper function that deals with IO, created in a new thread
    void process_IO( const Operation& operation, const int programID );

    // Prints elapsed time and current OS action
    void print( const std::string message );

    // Returns elapsed time, used by print
    std::chrono::duration<double> elapsed_seconds();
    
    // Loads all config info (used by constructor)
    void load_config( const std::string filePath );

    // fills queue with program's operations
    void load_meta_data( const std::string filePath );
    // helper function to get proper cycle time
    void set_operation_cycle_time( Operation &operation );

    // Program object used to store each program's information
    std::vector<Program> programs_;
    std::priority_queue<Program, std::vector<Program>, std::greater<Program>> SRTF_queue_;

    // File output object used by print() if log_location_ is set to FILE or BOTH
    std::ofstream fout_;

    // Simulator config data, variables declared in the same order as config file
    const float simulatorVersion_ = 2.0;
    std::string metaDataFilePath_;
    std::string schedulingCode_;
    int processorCycleTime_;
    int monitorDisplayTime_;
    int hardDriveCycleTime_;
    int printerCycleTime_;
    int keyboardCycleTime_;
    enum LogLocation
    { 
        SCREEN, FILE, BOTH 
    };
    LogLocation logLocation_;
    std::string logFilePath_;

    // Time variables
    std::chrono::time_point<std::chrono::system_clock> start_;
};

#endif // SIMULATOR_H