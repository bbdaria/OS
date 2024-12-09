#ifndef SHOW_PID_COMMAND_H_
#define SHOW_PID_COMMAND_H_

#include "built_in/built_in_command.h"
#include "code/smash/smash.h"

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char *cmd_line) : BuiltInCommand(cmd_line){}

    virtual ~ShowPidCommand()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        std::cout << smash.getStartPrompt() << " pid is " << getpid() << std::endl;
    }
};

#endif // SHOW_PID_COMMAND_H_