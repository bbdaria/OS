#ifndef FOREGROUND_COMMAND_H_
#define FOREGROUND_COMMAND_H_

#include <unistd.h> // getcwd
#include <iostream> // std::cout and std::cerr
#include "built_in/built_in_command.h"
#include "code/smash/Smash.h"
#include "code/joblist/joblist.h"

class ForegroundCommand : public BuiltInCommand {
public:
    ForegroundCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {
        m_cmd_line = cmd_line;
    }

    ~ForegroundCommand() = default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        // find id or null id
        bool isIdInput = false;
        // jobsList.getJobById();
        if (isIdInput) {

        }
        
    }
private:
    std::string m_cmd_line;
};

#endif // FOREGROUND_COMMAND_H_