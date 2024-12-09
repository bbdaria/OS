#ifndef BUILT_IN_COMMAND_H_
#define BUILT_IN_COMMAND_H_

#include "Commands.h"

class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char *cmd_line);

    virtual ~BuiltInCommand() {
    }
};

#endif // BUILT_IN_COMMAND_H_