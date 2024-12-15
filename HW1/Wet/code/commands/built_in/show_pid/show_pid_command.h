#ifndef SHOW_PID_COMMAND_H_
#define SHOW_PID_COMMAND_H_

#include "built_in_command.h"
#include "smash.h"

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand()=default;
    virtual ~ShowPidCommand()=default;

    void execute() override {
        // this is built-in command and so will run at smash PCB
        SmallShell &smash = SmallShell::getInstance();
        std::cout << smash.getStartPrompt() << " pid is " << std::to_string(getpid()) << std::endl;
    }
};

#endif // SHOW_PID_COMMAND_H_