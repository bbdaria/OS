#ifndef BUILT_IN_COMMAND_H_
#define BUILT_IN_COMMAND_H_

#include "commands/Commands.h"

class BuiltInCommand : public Command {
public:
    BuiltInCommand()=default;
    ~BuiltInCommand()=default;
};

#endif // BUILT_IN_COMMAND_H_