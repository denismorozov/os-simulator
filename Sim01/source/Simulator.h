#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <queue>

#include "Program.h"

class Simulator
{
public:

    Simulator( const std::string filePath );
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

    // program object used to store each program's information
    // will be a queue of programs for next phase
    Program *program_ = nullptr;

    // helper function for the constructor
    void load_config( const std::string filePath );
};

#endif // SIMULATOR_H