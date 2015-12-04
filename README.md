# Operating System Simulator
Project I made for my Principles of Operating Systems course which simulates a multiprogramming environment.
A configuration file is used to set things like the scheduling algorithm, cycle times, and location of the meta-data file.
The meta-data file provides a list of "programs" and the program's planned operations.

My personal goals for the project included following good design and programming practices, 
experimenting with as many C++11 features as possible, practice using threads, 
and take advantage of operating systems' need for lots and lots of data structures.

## Compiling and Running
make
./sim03 config.cnf

## Configuration
  <h3>Scheduling codes</h3>
  FIFO-P - First In First Out - Preemptive
  SRTF-P - Shortest Remaining Time First - Preemptive
  RR - Round Robin

  ### Meta-Data
  make generator
  ./generator
  then follow on-screen instructions to generate a meta-data file.

## Testing
make tests
./test

## To-Do
- All data structures besides the initial vector should contain pointers
- Tests
