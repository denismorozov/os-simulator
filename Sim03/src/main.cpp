/* Sim02
*
* Filename: Main.cpp
*
* Description: This operating system simulator runs a batch of program. Simulator
* settings are specified in the configuration file.
*
* Compilation: Use the provided makefile.
*   $ make
*
* Usage: The executable requires the configuration file as a command line 
* argument.
*   $ ./sim02 config_file.cnf
*/

/* Dependencies */
#include <iostream>
#include "simulator.h"

/* Main */
int main(const int argc, char const *argv[])
{
    // Check to see if a configuration file was provided
    if( argc != 2 )
    {
        std::cerr 
        << "Error: Incorrect number of command line arguments" << std::endl
        << "Example usage: " << argv[0] << " config_file.cnf" << std::endl;        
        return EXIT_FAILURE;
    }

    // Run the simulation with provided config file
    try
    {
        Simulator simulator( argv[1] );
        simulator.run();
    }
    
    // Catch unfixable errors, e.g. file not found
    catch( ... )
    {
        std::cerr << "Exiting program due to error" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}