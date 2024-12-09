#ifndef JOBS_COMMAND_H_
#define JOBS_COMMAND_H_

#include <unistd.h> // getcwd
#include "built_in/built_in_command.h"
#include "code/smash/Smash.h"
#include "code/joblist/joblist.h"

class JobsCommand : public BuiltInCommand {
public:
    JobsCommand()=default;
    ~JobsCommand()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        jobsList.printJobsList(this);
    }
};

#endif // JOBS_COMMAND_H_