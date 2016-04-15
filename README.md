# Operating System Simulator
This was a school project for my Principles of Operating Systems course. It attemps to simulate a multiprogramming environment by actively scheduling processes, creating async threads for I/O processing, and implementing simple interrupts. While just an exercise, this was incredibly fun to write.

It reads a list of programs and their operations, and then runs them with the specified CPU scheduling algorithm. The <b>meta-data file</b> provides the list of "programs" and their planned operations, while the <b>configuration file</b> is used to set things like the location of the meta-data file, the CPU scheduling algorithm, and CPU cycle times.

My main goal for the project was to play around with C++11 and take the opportunity to use lots and lots of data structures. The entire simulator uses the same code for all three of the implemented scheduling algorithms. The only thing that's different is the queue which holds the programs. Because priority queues and queues have different syntax for adding and removing values, this also has a nice example of explicitly specializing templates in C++.

## Compiling and Running
```bash
make
./sim03 config.cnf
```

## Configuration
### Scheduling codes
<table>
<tr><td>Code<td>Algorithm</td></tr>
<tr><td>FIFO-P</td><td>First In First Out - Preemptive</td></tr>
<tr><td>SRTF-P</td><td>Shortest Remaining Time First - Preemptive</td></tr>
<tr><td>RR</td><td>Round Robin</td></tr>
</table>


### Meta-Data
```bash
make generator
./generator
```
then follow on-screen instructions to generate a meta-data file.

