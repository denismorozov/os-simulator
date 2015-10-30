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

#include "Program.h"

/* OS Simulator. Loads a configuration file and a program to run */
class Simulator
{
public:
    // Constructor loads the config file and initializes Program object
    Simulator( const std::string file_path );
    ~Simulator();

    // Runs the simulation
    void run();

private:

    // Helper function that processes each individual program operation
    void process_operation( Program &program );

    // Helper function that deals with IO, created in a new thread
    void process_IO( const Operation& operation, const int program_id );

    // Prints elapsed time and current OS action
    void print( const std::string message );

    // Returns elapsed time, used by print
    std::chrono::duration<double> elapsed_seconds();
    
    // Loads all config info (used by constructor)
    void load_config( const std::string file_path );

    // fills queue with program's operations
    void load_meta_data( const std::string file_path );  

    // Program object used to store each program's information
    std::vector<Program> programs_;
    std::priority_queue<Program, std::vector<Program>, std::greater<Program>> SRTF_queue_;

    // Declaration of enum for possible simulator output locations
    enum LogLocation
    { 
        SCREEN, FILE, BOTH 
    };

    // File output object used by print() if log_location_ is set to FILE or BOTH
    std::ofstream fout_;

    // Simulator config data, variables declared in the same order as config file
    const float simulator_version_ = 2.0;
    std::string meta_data_file_path_;
    std::string scheduling_code_;
    int processor_cycle_time_;
    int monitor_display_time_;
    int hard_drive_cycle_time_;
    int printer_cycle_time_;
    int keyboard_cycle_time_;
    LogLocation log_location_;
    std::string log_file_path_;

    // Time variables
    std::chrono::time_point<std::chrono::system_clock> start_;
};

#endif // SIMULATOR_H