#ifndef SIMULATOR_H
#define SIMULATOR_H

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

class Simulator
{
public:

    Simulator( const std::string file_path );
    ~Simulator();
    void run();

private:

    // declaration of enum for possible simulator output locations
    enum LogLocation
    { 
        SCREEN, FILE, BOTH 
    };

    // simulator config data, variables declared in the same order as config file
    const float simulator_version_ = 1.0;
    std::string meta_data_file_path_;
    int processor_cycle_time_;
    int monitor_display_time_;
    int hard_drive_cycle_time_;
    int printer_cycle_time_;
    int keyboard_cycle_time_;
    LogLocation log_location_;
    std::string log_file_path_;

    // time variables
    std::chrono::time_point<std::chrono::system_clock> start_, now_;
    std::chrono::duration<double> elapsed_seconds();

    // program object used to store each program's information
    // will be a queue of programs for next phase
    Program *program_ = nullptr;

    // helper function that processes each individual program operation
    void process_operation( const Operation & operation );

    void print( const std::string message );
    
    // helper function for the constructor to load config
    void load_config( const std::string file_path );

    std::ofstream fout_;
};

#endif // SIMULATOR_H