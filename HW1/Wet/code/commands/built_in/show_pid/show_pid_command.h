#ifndef SHOW_PID_COMMAND_H_
#define SHOW_PID_COMMAND_H_

#include "../built_in_command.h"
#include "smash/smash.h"

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand()=default;
    virtual ~ShowPidCommand()=default;

    void execute() override {
        // this is built-in command and so will run at smash PCB
        SmallShell &smash = SmallShell::getInstance();
        printOut(smash.getStartPrompt());
        printOut(" pid is ");
        printOut(std::to_string(getpid()));
        printOut("\n");
    }
};

#endif // SHOW_PID_COMMAND_H_