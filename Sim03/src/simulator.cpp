#include "simulator.h"

/* Constructor for the Simulator class
* Loads the config file for the simulator, if no problems occured then it
* creates the program object (which reads the meta-data file). Also opens the
* output file if needed, and sets precision of double/float outputs
* @param file path for configuration file
*/
Simulator::Simulator( const std::string filePath )
{
    try
    {
        load_config( filePath );
        load_meta_data( metaDataFilePath_ );
    }
    catch( const std::runtime_error& e )
    {
        std::cerr << e.what();
        throw;
    }


    std::cout.precision(6); // precision for printing doubles/floats

    if( logLocation_ == BOTH || logLocation_ == FILE )
    {
        fout_.open( logFilePath_ );
    }
}

Simulator::~Simulator()
{
    if(fout_.is_open())
    {
        fout_.close();
    }
}

template<>
void Simulator::run_helper<std::queue<Program>>(){
    std::unique_ptr<std::queue<Program>> readyQueue(new std::queue<Program>);

    // load programs into queue, setting them to ready
    print("OS: preparing all processes");
    for( Program program : programs_ )
    {
        program.state = READY;
        readyQueue->push(program);
    }

    while( !readyQueue->empty() )
    {
        while( !interrupts_.empty() )
        {
            Interrupt interrupt = interrupts_.front();
            interrupts_.pop();

            if( interrupt.processID != 0 )
            {
                Program blockedProgram = blockedPrograms_.at( interrupt.processID );
                blockedPrograms_.erase( interrupt.processID );  

                blockedProgram.state = READY;                              
                readyQueue->push( blockedProgram );
            }
        }

        print("OS: selecting next process");
        Program currentProgram = readyQueue->front();
        readyQueue->pop();

        process_operation( currentProgram );
    }
}

template<typename QueueType>
void Simulator::run_helper()
{
    std::unique_ptr<QueueType> readyQueue(new QueueType);

    // load programs into queue, setting them to ready
    print("OS: preparing all processes");
    for( Program program : programs_ )
    {
        program.state = READY;
        readyQueue->push(program);
    }

    while( !readyQueue->empty() )
    {
        while( !interrupts_.empty() )
        {
            Interrupt interrupt = interrupts_.front();
            interrupts_.pop();

            if( interrupt.processID != 0 )
            {
                Program blockedProgram = blockedPrograms_.at( interrupt.processID );
                blockedPrograms_.erase( interrupt.processID );  
        
                blockedProgram.state = READY;                              
                readyQueue->push( blockedProgram );
            }
        }

        print("OS: selecting next process");
        Program currentProgram = readyQueue->top();
        readyQueue->pop();

        process_operation( currentProgram );
    }
}

/* Run the simulator and all its programs
*/
void Simulator::run()
{
    // Announce beginning of sim and set starting time point
    start_ = std::chrono::system_clock::now();
    print("Simulator program starting");  

    // Run with the proper queue for the scheduling algorithm
    if( schedulingCode_ == "FIFO-P" )
    {   
        run_helper<FIFO_Q>();
    }

    else if( schedulingCode_ == "RR" )
    {
        run_helper<RR_Q>();
    }

    else // SRTF-P
    {
        run_helper<SRTF_Q>();
    }

    print("Simulator program ending");
}

/* Process program operations. Create a thread for each I/O operation.
* @param program = current program that is being processed
*/
void Simulator::process_operation( Program &program )
{
    const int programID = program.id;
    Operation operation = program.next();

    // If the process is just starting, announce then go on to printing first operation
    if( operation.type == 'A' && operation.description == "start" )
    {
        print("OS: starting process " + std::to_string(programID));
        operation = program.next();
    }


    // Processing operation
    if( operation.type == 'P' )
    {
        print("Process " + std::to_string(programID) + ": start processing action");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration )
        );
        print("Process " + std::to_string(programID) + ": end processing action");

    }

    // Input/Output operation
    else if( operation.type == 'I' || operation.type == 'O' )
    {
        // create a thread for any I/O operation
        std::thread IO_thread( [this, operation, programID](){
            process_IO(operation, programID);
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
        print("OS: removing process " + std::to_string(programID));
    }
}

/* Process I/O operation. This function is always called in a separate thread
* @param operation = current operation that is being processed
* @param programID = id of current program for printing
*/
void Simulator::process_IO( const Operation& operation, const int programID )
{
    if( operation.description == "hard drive" )
    {
        std::string accessType;
        if( operation.type == 'I' )
        {
            accessType = "input";
        }

        else
        {
            accessType = "output";
        }

        print("Process " + std::to_string(programID) + ": start hard drive " + accessType );
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration )
        );
        print("Process " + std::to_string(programID) + ": end hard drive " + accessType );
    }
    else if( operation.description == "keyboard" )
    {
        print("Process " + std::to_string(programID) + ": start keyboard input");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration )
        );
        print("Process " + std::to_string(programID) + ": end keyboard input");
    }
    else if( operation.description == "monitor" )
    {
        print("Process " + std::to_string(programID) + ": start monitor output");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration )
        ); 
        print("Process " + std::to_string(programID) + ": end monitor output");           
    }
    else if( operation.description == "printer" )
    {
        print("Process " + std::to_string(programID) + ": start printer output");
        std::this_thread::sleep_for(
            std::chrono::milliseconds( operation.duration )
        ); 
        print("Process " + std::to_string(programID) + ": end printer output");               
    }
}

/* Prints OS action to file, screen, or both, with elapsed time
* @param Message to be printed
*/
void Simulator::print( const std::string message )
{
    std::chrono::duration<double> elapsedSeconds = std::chrono::system_clock::now()-start_;
    auto time = elapsedSeconds.count();
    if( logLocation_ == BOTH || logLocation_ == SCREEN )
    {
        std::cout << std::fixed << time << " - " << message << std::endl;
    }
    if( logLocation_ == BOTH || logLocation_ == FILE )
    {
        fout_ << std::fixed << time << " - " << message << std::endl;
    }    
}

/* Loads data from the config file
* @param Path to config file
* @except Throws exception if file wasn't found or file format isn't correct
*/
void Simulator::load_config( const std::string filePath )
{
    // attempt opening the file
    std::ifstream fin( filePath, std::ifstream::in );
    if(!fin)
    {
        std::string error = "Error: Unable to open file " + filePath + "\n";
        throw std::runtime_error( error );
    }

    // make sure the first line of the config file is correct
    std::string configFormatLine;
    std::getline(fin, configFormatLine, '\n');
    if( configFormatLine != "Start Simulator Configuration File" )
    {
        throw std::runtime_error( "Error: Incorrect config file format\n" );
    }

    // ignoring everything up to the ':' 
    auto limit = std::numeric_limits<std::streamsize>::max();
    fin.ignore( limit, ':' );

    // make sure the configuration file is for the correct simulator version
    float simVersion;
    fin >> simVersion;
    if( simVersion != simulatorVersion_ )
    {
        std::cerr << std::endl 
            << "Warning: Wrong simulator version." << std::endl            
            << "Expected: " << simulatorVersion_ << std::endl
            << "Given: " <<  simVersion << std::endl
            << "The only reason this doesn't immediately end the program is because the provided \
            test configuration file has its version set to 2.0 instead of 3.0." << std::endl 
            << std::endl;
    }
    fin.ignore( limit, ':' );

    // get the rest of the data from the config file
    fin >> metaDataFilePath_;
    fin.ignore( limit, ':' );

    fin >> schedulingCode_;
    fin.ignore( limit, ':' );
    if( schedulingCode_ != "RR" &&
        schedulingCode_ != "FIFO-P" &&
        schedulingCode_ != "SRTF-P" )
    {
        throw std::runtime_error( "Error: Unrecognized scheduling code\n" );
    }

    fin >> quantum_;
    fin.ignore( limit, ':' );

    fin >> processorCycleTime_;
    fin.ignore( limit, ':' );

    fin >> monitorDisplayTime_;
    fin.ignore( limit, ':' );

    fin >> hardDriveCycleTime_;
    fin.ignore( limit, ':' );

    fin >> printerCycleTime_;
    fin.ignore( limit, ':' );

    fin >> keyboardCycleTime_;
    fin.ignore( limit, ':' );

    std::string logString;
    fin >> std::ws; // ignore the space after :
    std::getline(fin, logString, '\n');

    // transform the log location to enum for easier processing later
    if( logString == "Log to Both")
    {
        logLocation_ = BOTH;
    }
    else if( logString == "Log to File" )
    {
        logLocation_ = FILE;
    }
    else
    {
        logLocation_ = SCREEN;
    }
    fin.ignore( limit, ':' );

    fin >> logFilePath_;

    // make sure the config file ends here
    fin >> configFormatLine;
    if( configFormatLine != "End" )
    {
        throw std::runtime_error( "Error: Incorrect config file format\n" );
    }

    fin.close();
}

/* Loads each operation specified in the meta-data file into queue 
* @param file path for the meta data file
*/
void Simulator::load_meta_data( const std::string filePath )
{

    // string object that's used throughout this function
    std::string input;

    // attempt opening the file
    std::ifstream fin( filePath, std::ifstream::in );
    if(!fin)
    {
        std::string error = "Error: Unable to open file " + filePath + "\n";
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
    int paranthesisLocation;


    while( fin.peek() != 'S' )
    {
        Program newProgram;

        // Get all program data
        do
        {
            // after getline, string looks like this: "A(start)0"   
            std::getline(fin, input, ';');
            paranthesisLocation = input.find(')');

            // parsing Operation object
            operation.type = input.front();        
            operation.description = input.substr(2, paranthesisLocation-2);
            operation.cycles = std::stoi(
                std::string( input.begin()+paranthesisLocation+1, input.end()) 
            );

            // find and set cycle time of the operation
            set_operation_cycle_time(operation);

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

void Simulator::set_operation_cycle_time( Operation &operation )
{
    if( operation.type == 'P' )
    {
        operation.cycleTime = processorCycleTime_;
    }

    else if( operation.type == 'I' || operation.type == 'O' )
    {
        if( operation.description == "hard drive" )
        {
            operation.cycleTime = hardDriveCycleTime_;
        }
        
        else if( operation.description == "keyboard" )
        {
            operation.cycleTime = keyboardCycleTime_;
        }
        else if( operation.description == "monitor" )
        {
            operation.cycleTime = monitorDisplayTime_;
        }

        else if( operation.description == "printer" )
        {
            operation.cycleTime = printerCycleTime_;
        }
    }

    else if( operation.type == 'A' || operation.type == 'S' )
    {
        operation.cycleTime = 0;
    }

    else throw std::runtime_error( "Error: Unrecognized operation type, \
        check meta-data file" );
}
