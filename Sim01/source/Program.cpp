#include "Program.h"

/* Calls fnc to load meta-data file, throws exception if error occurs 
* @param file path for the meta data file
*/
Program::Program( const std::string file_path )
{
    try
    {
        load_meta_data( file_path );
    }
    catch( const std::runtime_error& e )
    {
        std::cerr << e.what();
        throw;
    }
}

Program::~Program()
{
}

/* Loads each operation specified in the meta-data file into queue 
* @param file path for the meta data file
*/
void Program::load_meta_data( const std::string file_path )
{
    // string object that's used throughout this function
    std::string input;

    // attempt opening the file
    std::ifstream fin( file_path, std::ifstream::in );
    if(!fin)
    {
        std::string error = "Error: Unable to open file " + file_path + "\n";
        throw std::runtime_error( error );
    }

    // make sure the first line of the file is correct
    std::getline(fin, input, ':');
    if( input != "Start Program Meta-Data Code" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format" );
    }

    Operation operation;
    int paranthesis_loc;

    // get S(start)0 (sim start) and all program data
    while( input != "A(end)0" )
    {
        // after getline, string looks like this: "S(start)0"
        fin >> std::ws;
        std::getline(fin, input, ';');
        paranthesis_loc = input.find(')');

        // construct Operation object
        operation.type = input.front();        
        operation.description = input.substr(2, paranthesis_loc-2);
        operation.duration = std::stoi(
            std::string( input.begin()+paranthesis_loc+1, input.end()) 
        );

        // insert operation into queue
        operations.push(operation);
    }

    // get S(end)0 (simulator end)
    fin >> std::ws;
    std::getline(fin, input, '.');
    paranthesis_loc = input.find(')');

    // construct Operation object
    operation.type = input.front();
    operation.description = input.substr(2, paranthesis_loc-2);
    operation.duration = std::stoi(
        std::string( input.begin()+paranthesis_loc+1, input.end()) 
    );

    // insert operation into queue
    operations.push(operation);

    // make sure the last line of the file is correct
    fin >> std::ws;
    std::getline(fin, input, '.');
    if( input != "End Program Meta-Data Code" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format" );
    }

    fin.close();
}