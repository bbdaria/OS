#ifndef BUILT_IN_COMMAND_H_
#define BUILT_IN_COMMAND_H_

#include "Commands.h"

class BuiltInCommand : public Command {
public:
    BuiltInCommand()=default;
    BuiltInCommand(const std::string& file, bool append) : Command(file, append) {}
    ~BuiltInCommand()=default;
};

#endif // BUILT_IN_COMMAND_H_