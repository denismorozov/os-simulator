#include "program.h"


Program::Program()
{
}

Program::~Program()
{
}

/* Add an operation to queue, updating remaining time
* @param operation to be added to queue
*/
void Program::add_operation( Operation newOperation )
{
    newOperation.duration = newOperation.cycleTime * newOperation.cycles;
    operations_.push_back(newOperation);    
    remainingProgramTime_ += newOperation.duration;
}

/* Add an earlier operation to the front of operation queue
* Needed for when an operation began running but then got interrupted
* @param operation to be placed in front of queue
*/
void Program::return_operation( Operation operation )
{
    operations_.push_front(operation);
    remainingProgramTime_ += operation.duration;
}

/* Returns true if all operations were completed
*/
bool Program::done() const
{
    return operations_.empty();
}

/* Returns next program operation
* Pops the operation off the queue and updates program remaining time
* @return next operation
*/
Operation Program::next()
{
    Operation nextOperation = operations_.front();
    operations_.pop_front();
    remainingProgramTime_ -= nextOperation.duration;
    return nextOperation;
}

/* remaining time getter
*/
int Program::remaining_time() const
{
    return remainingProgramTime_;
}

/* remaining number of operations getter
*/
int Program::remaining_operations() const
{
    return operations_.size();
}
