#include "Program.h"

/* Calls fnc to load meta-data file, throws exception if error occurs 
* @param file path for the meta data file
*/
Program::Program()
{
}

Program::~Program()
{
}

void Program::add_operation( Operation new_operation )
{
    operations_.push(new_operation);
    remaining_time_ += new_operation.duration;
}

bool Program::done() const
{
    return operations_.empty();
}

Operation Program::next()
{
    Operation next_operation = operations_.front();
    operations_.pop();
    remaining_time_ -= next_operation.duration;

    return next_operation;
}

int Program::remaining_time() const
{
    return remaining_time_;
}

int Program::remaining_operations() const
{
    return operations_.size();
}

bool Program::operator>( const Program &other ) const
{
    return remaining_time_ > other.remaining_time();
}