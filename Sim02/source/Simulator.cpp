#include "Simulator.h"

/* Constructor for the Simulator class
* Loads the config file for the simulator, if no problems occured then it
* creates the program object (which reads the meta-data file). Also opens the
* output file if needed, and sets precision of double/float outputs
* @param file path for configuration file
*/
Simulator::Simulator( const std::string file_path )
{
    try
    {
        load_config( file_path );
        program_ = new Program();
        load_meta_data( meta_data_file_path_ );
    }
    catch( const std::runtime_error& e )
    {
        std::cerr << e.what();
        throw;
    }


    std::cout.precision(6); // precision for printing doubles/floats

    if( log_location_ == BOTH || log_location_ == FILE )
    {
        fout_.open( log_file_path_ );
    }
}

Simulator::~Simulator()
{
    if(program_ != nullptr)
    {
        delete program_;
    }

    if(fout_.is_open())
    {
        fout_.close();
    }
}

/* Go through all the operations in the program's queue
*/
void Simulator::run()
{

    start_ = std::chrono::system_clock::now();
    print("Simulator program starting");

    while( program_->operations.size() != 0)
    {
        process_operation( program_->operations.front() );
        program_->operations.pop();
    }

    print("Simulator program ending");
}

/* Process program operations. Create a thread for each I/O operation.
* @param operation = current operation that is being processed
*/
void Simulator::process_operation( const Operation &operation )
{
    // Program operation 
    if( operation.type == 'A' )
    {
        if( operation.description == "start" )
        {
            print("OS: preparing process 1");
            print("OS: starting process 1");
        }
        else
        {
            print("OS: removing process 1");
        }
    }

    // Processing operation
    else if( operation.type == 'P' )
    {
        print("Process 1: start processing action");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * processor_cycle_time_ )
        );
        print("Process 1: end processing action");

    }

    // Input/Output operation
    else if( operation.type == 'I' || operation.type == 'O' )
    {
        // create a thread for any I/O operation
        std::thread IO_thread( [this, operation](){ process_IO(operation); });

        // wait for IO to finish execution
        if( IO_thread.joinable() )
        {
            IO_thread.join();
        }
    }
}

/* Process I/O operation. This function is always called in a separate thread
* @param operation = current operation that is being processed
*/
void Simulator::process_IO( const Operation& operation )
{
    if( operation.description == "hard drive" )
    {
        std::string access_type;
        if( operation.type == 'I' )
        {
            access_type = "input";
        }

        else
        {
            access_type = "output";
        }

        print("Process 1: start hard drive " + access_type );
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * hard_drive_cycle_time_ )
        );
        print("Process 1: end hard drive " + access_type );
    }
    else if( operation.description == "keyboard" )
    {
        print("Process 1: start keyboard input");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * keyboard_cycle_time_)
        );
        print("Process 1: end keyboard input");
    }
    else if( operation.description == "monitor" )
    {
        print("Process 1: start monitor output");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * monitor_display_time_ )
        ); 
        print("Process 1: end monitor output");           
    }
    else if( operation.description == "printer" )
    {
        print("Process 1: start printer output");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * printer_cycle_time_ )
        ); 
        print("Process 1: end printer output");               
    }
}

/* Prints OS action to file, screen, or both, with elapsed time
* @param Message to be printed
*/
void Simulator::print( const std::string message )
{
    auto time = elapsed_seconds().count();
    if( log_location_ == BOTH || log_location_ == SCREEN )
    {
        std::cout << std::fixed << time << " - " << message << std::endl;
    }
    if( log_location_ == BOTH || log_location_ == FILE )
    {
        fout_ << std::fixed << time << " - " << message << std::endl;
    }    
}

/* Calculates elapsed time from the beginning of the simulation
* @return Time difference
*/
std::chrono::duration<double> Simulator::elapsed_seconds()
{
    return std::chrono::system_clock::now()-start_;
}

/* Loads data from the config file
* @param Path to config file
* @except Throws exception if file wasn't found or file format isn't correct
*/
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
    else if( log_string == "Log to File" )
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

/* Loads each operation specified in the meta-data file into queue 
* @param file path for the meta data file
*/
void Simulator::load_meta_data( const std::string file_path )
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

    // make sure the beginning of the file is correct
    std::getline(fin, input, ';');
    if( input != "Start Program Meta-Data Code:\nS(start)0" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format" );
    }

    Operation operation;
    int paranthesis_loc;

    // Get all program data
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
        program_->add_operation(operation);
    }

    // Make sure the simulator end flag is there
    fin >> std::ws;
    std::getline(fin, input, '.');
    if( input != "S(end)0" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format" );
    }

    // make sure the last line of the file is correct
    fin >> std::ws;
    std::getline(fin, input, '.');
    if( input != "End Program Meta-Data Code" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format" );
    }

    fin.close();
}