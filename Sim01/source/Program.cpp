#include "Program.h"

Program::Program( const std::string file_path )
{
    try
    {
        load_meta_data( file_path );
    }
    catch( const std::runtime_error& e )
    {
        std::cerr << e.what();

        // can't continue if there was a problem reading the file
        throw;
    }
}

Program::~Program()
{

}

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
    std::cout << input << std::endl;
    if( input != "Start Program Meta-Data Code" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format" );
    }

    while( input != "A(end)0" )
    {
        fin >> std::ws;
        std::getline(fin, input, ';');

        std::cout << input << std::endl;
    }

    // get S(end)0 (simulator end)
    fin >> std::ws;
    std::getline(fin, input, '.');
    std::cout << input << std::endl;

    // make sure the last line of the file is correct
    fin >> std::ws;
    std::getline(fin, input, '.');
    std::cout << input << std::endl;
    if( input != "End Program Meta-Data Code" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format" );
    }

    fin.close();
}