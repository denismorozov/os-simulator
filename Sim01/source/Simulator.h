#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <queue>
#include <fstream>

#include "Program.h"

class Simulator
{
public:
    Simulator( const std::string filePath );
    ~Simulator();

    void run();

private:
    // simulator config data
    int processor_cycle_time;
    int monitor_display_time;
    int hard_drive_cycle_time;
    int printer_cycle_time;
    int keyboard_cycle_time;
    int log_location; // 0 for screen, 1 for file, 2 for both 
    int log_file_path;

    // program object used to store each program's information
    // will be a queue of programs for next phase
    Program *program;

    // helper function for the constructor
    void loadConfig( const std::string filePath );
};

#endif // SIMULATOR_H