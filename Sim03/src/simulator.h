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
#include <map>
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

    /***** Helper functions *****/

    template<typename QueueType>
    void run_helper();

    // Helper function that processes each individual program operation
    void process_operation( Program &program );

    // Helper function that deals with IO, created in a new thread
    void process_IO( const Operation& operation, const int programID );

    // Prints elapsed time and current OS action
    void print( const std::string message );

    // Loads all config info (used by constructor)
    void load_config( const std::string filePath );

    // fills queue with program's operations
    void load_meta_data( const std::string filePath );

    // helper function to get proper cycle time
    void set_operation_cycle_time( Operation &operation );


    /***** Structures *****/

    // Incredibly simple "Interrupt"
    struct Interrupt
    {
        Interrupt(int PID = 0) : processID(PID) {}
        // if an interrupt has PID of 0 then it must have been a quantum interrupt
        // otherwise, it is an I/O event of specified processID
        int processID;
    };
    std::queue<Interrupt> interrupts_;

    // All the program's information
    std::vector<Program> programs_;

    // Currently blocked programs
    std::map<int,Program> blockedPrograms_; 

    // Scheduling
    struct FIFOComparator{
        bool operator()( const Program &left, const Program &right ){
            return left.id > right.id;
        }
    };
    struct SRTFComparator{
        bool operator()( const Program &left, const Program &right ){
            return left.remaining_time() > right.remaining_time();
        }
    };
    using RR_Q = std::queue<Program>;
    using FIFO_Q = std::priority_queue<Program, std::vector<Program>, FIFOComparator>;
    using SRTF_Q = std::priority_queue<Program, std::vector<Program>, SRTFComparator>;   


    /***** Simulator config data *****/

    //  variables declared in the same order as config file
    const float simulatorVersion_ = 3.0;
    std::string metaDataFilePath_;
    std::string schedulingCode_;
    int quantum_;
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


    /***** Other simulator variables *****/

    // Time variable to keep track of the beginning of the simulation
    std::chrono::time_point<std::chrono::system_clock> start_;

    // File output object used by print() if log_location_ is set to FILE or BOTH
    std::ofstream fout_;
};

#endif // SIMULATOR_H