#include "operation.h"


/* Run a single cycle
* Decrements duration by cycle_time and cycle by one
* @return cycle time so operating system knows how long its going to run for
*/ 
int Operation::run()
{
    duration -= cycleTime;
    cycles--;
    return cycleTime;
}

/* Operation done executing?
* @return true if no more cycles remaining
*/
bool Operation::done()
{
    return cycles == 0;
}