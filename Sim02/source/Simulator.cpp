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
    if(fout_.is_open())
    {
        fout_.close();
    }
}

/* Run the simulator and all its programs
*/
void Simulator::run()
{
    // Announce beginning of sim and set starting time point
    start_ = std::chrono::system_clock::now();
    print("Simulator program starting");  

    // First In First Out scheduling
    if( scheduling_code_ == "FIFO" )
    {
        print("OS: preparing all processes");
        for( Program program : programs_ )
        {
            program.state = READY;
        }

        int program_counter = 0;
        for( Program program : programs_ )
        {
            print("OS: selecting next process");
            program_counter++;

            program.id = program_counter;            
            program.state = RUNNING;

            while( !program.done() )
            {
                process_operation( program );
            }

            program.state = EXIT;
        }
    }

    // Shortest Remaining Time First - Non Preemptive
    // Also satisfies Shortest Job First
    else
    {
        print("OS: preparing all processes");
        for( Program program : programs_ )
        {
            program.state = READY;
            SRTF_queue_.push(program);
        }

        int program_counter = 0;
        while( !SRTF_queue_.empty() )
        {
            print("OS: selecting next process");

            // remove program with shortest remaining time from queue
            Program program = SRTF_queue_.top(); 
            SRTF_queue_.pop();

            // simple way of telling whether the program ran before (so id
            // doesn't get changed)
            if( program.id == 0 )
            {
                program_counter++;
                program.id = program_counter;
            }

            program.state = RUNNING;
            process_operation( program );

            if( program.done() )
            {
                program.state = EXIT;
            }

            else
            {
                program.state = READY;
                SRTF_queue_.push( program );
            }
        }
    }

    print("Simulator program ending");
}

/* Process program operations. Create a thread for each I/O operation.
* @param program = current program that is being processed
*/
void Simulator::process_operation( Program &program )
{
    const int program_id = program.id;
    Operation operation = program.next();

    // If the process is just starting, announce then go on to printing first operation
    if( operation.type == 'A' && operation.description == "start" )
    {
        print("OS: starting process " + std::to_string(program_id));
        operation = program.next();
    }


    // Processing operation
    if( operation.type == 'P' )
    {
        print("Process " + std::to_string(program_id) + ": start processing action");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * processor_cycle_time_ )
        );
        print("Process " + std::to_string(program_id) + ": end processing action");

    }

    // Input/Output operation
    else if( operation.type == 'I' || operation.type == 'O' )
    {
        // create a thread for any I/O operation
        std::thread IO_thread( [this, operation, program_id](){
            process_IO(operation, program_id);
        });

        // wait for IO to finish execution
        if( IO_thread.joinable() )
        {
            IO_thread.join();
        }
    }


    // if only one operation remains, it must be the program end announcement
    if( program.remaining_operations() == 1 )
    {
        program.next(); // just to pop the last item off
        print("OS: removing process " + std::to_string(program_id));
    }
}

/* Process I/O operation. This function is always called in a separate thread
* @param operation = current operation that is being processed
* @param program_id = id of current program for printing
*/
void Simulator::process_IO( const Operation& operation, const int program_id )
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

        print("Process " + std::to_string(program_id) + ": start hard drive " + access_type );
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * hard_drive_cycle_time_ )
        );
        print("Process " + std::to_string(program_id) + ": end hard drive " + access_type );
    }
    else if( operation.description == "keyboard" )
    {
        print("Process " + std::to_string(program_id) + ": start keyboard input");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * keyboard_cycle_time_)
        );
        print("Process " + std::to_string(program_id) + ": end keyboard input");
    }
    else if( operation.description == "monitor" )
    {
        print("Process " + std::to_string(program_id) + ": start monitor output");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * monitor_display_time_ )
        ); 
        print("Process " + std::to_string(program_id) + ": end monitor output");           
    }
    else if( operation.description == "printer" )
    {
        print("Process " + std::to_string(program_id) + ": start printer output");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration * printer_cycle_time_ )
        ); 
        print("Process " + std::to_string(program_id) + ": end printer output");               
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
        throw std::runtime_error( "Error: Incorrect config file format\n" );
    }

    // ignoring everything up to the ':' 
    auto limit = std::numeric_limits<std::streamsize>::max();
    fin.ignore( limit, ':' );

    // make sure the configuration file is for the correct simulator version
    float sim_version;
    fin >> sim_version;
    if( sim_version != simulator_version_ )
    {
        throw std::runtime_error( "Error: Wrong simulator version\n" );
    }
    fin.ignore( limit, ':' );

    // get the rest of the data from the config file
    fin >> meta_data_file_path_;
    fin.ignore( limit, ':' );

    fin >> scheduling_code_;
    fin.ignore( limit, ':' );
    if( scheduling_code_ != "FIFO" &&
        scheduling_code_ != "SJF" &&
        scheduling_code_ != "SRTF-N" )
    {
        throw std::runtime_error( "Error: Unrecognized scheduling code\n" );
    }

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
        throw std::runtime_error( "Error: Incorrect config file format\n" );
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
    fin >> std::ws;
    if( input != "Start Program Meta-Data Code:\nS(start)0" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format: \
            Simulator start flag is missing\n" );
    }

    Operation operation;
    int paranthesis_loc;


    while( fin.peek() != 'S' )
    {
        Program newProgram;

        // Get all program data
        do
        {
            // after getline, string looks like this: "A(start)0"   
            std::getline(fin, input, ';');
            paranthesis_loc = input.find(')');

            // construct Operation object
            operation.type = input.front();        
            operation.description = input.substr(2, paranthesis_loc-2);
            operation.duration = std::stoi(
                std::string( input.begin()+paranthesis_loc+1, input.end()) 
            );

            // insert operation into queue
            newProgram.add_operation(operation);

            // eat whitespace
            fin >> std::ws;
        } while( input != "A(end)0" );

        // insert the complete program into list of programs
        programs_.push_back(newProgram);
    }

    // Make sure the simulator end flag is there
    std::getline(fin, input, '.');
    fin >> std::ws;
    if( input != "S(end)0" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format: \
            Simulator end flag is missing\n" );
    }

    // make sure the last line of the file is correct
    std::getline(fin, input, '.');
    if( input != "End Program Meta-Data Code" )
    {
        throw std::runtime_error( "Error: Incorrect meta-data file format: \
            Meta-Data file does not end after simulator operations end\n" );
    }

    fin.close();
}