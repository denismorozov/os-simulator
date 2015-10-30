#include "Program.h"


Program::Program()
{
}

Program::~Program()
{
}

/* Add an operation to queue, updating remaining time
* @param operation to be added to queue
*/
void Program::add_operation( Operation new_operation )
{
    operations_.push(new_operation);
    remaining_time_ += new_operation.duration;
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
    Operation next_operation = operations_.front();
    operations_.pop();
    remaining_time_ -= next_operation.duration;

    return next_operation;
}

/* remaining time getter
*/
int Program::remaining_time() const
{
    return remaining_time_;
}

/* remaining number of operations getter
*/
int Program::remaining_operations() const
{
    return operations_.size();
}

/* comparison operator so the programs can be used in a priority queue
*/
bool Program::operator>( const Program &other ) const
{
    return remaining_time_ > other.remaining_time();
}