#ifndef JOBS_COMMAND_H_
#define JOBS_COMMAND_H_

#include <unistd.h> // getcwd
#include <iostream> // std::cout and std::cerr
#include "built_in/built_in_command.h"
#include "code/smash/Smash.h"
#include "code/joblist/joblist.h"

class JobsCommand : public BuiltInCommand {
public:
    JobsCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}
    ~JobsCommand() = default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        jobsList.printJobsList();
    }
};

#endif // JOBS_COMMAND_H_