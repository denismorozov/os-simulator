#include "Program.h"

/* Calls fnc to load meta-data file, throws exception if error occurs 
* @param file path for the meta data file
*/
Program::Program()
{
    try
    {
        
    }
    catch( const std::runtime_error& e )
    {
        std::cerr << e.what();
        throw;
    }
}

void Program::add_operation( Operation operation )
{
    operations.push(operation);
    running_time += operation.duration;
}

Program::~Program()
{
}

