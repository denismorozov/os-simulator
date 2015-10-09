#include "Simulator.h"

Simulator::Simulator( const std::string file_path )
{
    try
    {
        load_config( file_path );
    }
    catch( const std::runtime_error& e )
    {
        std::cerr << e.what();

        // can't continue if there was a problem reading config file
        throw;
    }

    program_ = new Program( meta_data_file_path_ );
}

Simulator::~Simulator()
{
    if(program_ != nullptr)
    {
        delete program_;
    }
}

void Simulator::run()
{

}

void Simulator::load_config( const std::string file_path )
{
    // attempt opening the file
    std::ifstream fin( file_path, std::ifstream::in );
    if(!fin)
    {
        std::string error = "Error: Unable to open file " + file_path + "\n";
        throw std::runtime_error( error );
    }

    // make sure the first line of the config file is correct
    std::string config_format_line;
    std::getline(fin, config_format_line, '\n');
    if( config_format_line != "Start Simulator Configuration File" )
    {
        throw std::runtime_error( "Error: Incorrect config file format" );
    }

    // only interested in ignoring everything up to the ':' 
    auto limit = std::numeric_limits<std::streamsize>::max();
    fin.ignore( limit, ':' );

    // make sure the configuration file is for the correct simulator version
    float sim_version;
    fin >> sim_version;
    if( sim_version != simulator_version_ )
    {
        throw std::runtime_error( "Error: Wrong simulator version" );
    }
    fin.ignore( limit, ':' );

    // get the rest of the data from the config file
    fin >> meta_data_file_path_;
    fin.ignore( limit, ':' );

    fin >> processor_cycle_time_;
    fin.ignore( limit, ':' );

    fin >> monitor_display_time_;
    fin.ignore( limit, ':' );

    fin >> hard_drive_cycle_time_;
    fin.ignore( limit, ':' );

    fin >> printer_cycle_time_;
    fin.ignore( limit, ':' );

    fin >> keyboard_cycle_time_;
    fin.ignore( limit, ':' );

    std::string log_string;
    fin >> std::ws; // ignore the space after :
    std::getline(fin, log_string, '\n');

    // transform the log location to enum for easier processing later
    if( log_string == "Log to Both")
    {
        log_location_ = BOTH;
    }
    else if( log_string == "File" )
    {
        log_location_ = FILE;
    }
    else
    {
        log_location_ = SCREEN;
    }
    fin.ignore( limit, ':' );

    fin >> log_file_path_;

    // make sure the config file ends here
    fin >> config_format_line;
    if( config_format_line != "End" )
    {
        throw std::runtime_error( "Error: Incorrect config file format" );
    }

    fin.close();
}
