#ifndef JOBS_COMMAND_H_
#define JOBS_COMMAND_H_

#include <unistd.h> // getcwd
#include "../built_in_command.h"
#include "smash/smash.h"
#include "joblist/joblist.h"

class JobsCommand : public BuiltInCommand {
public:
    JobsCommand()=default;
    ~JobsCommand()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        jobsList.printJobsList();
    }
};

#endif // JOBS_COMMAND_H_