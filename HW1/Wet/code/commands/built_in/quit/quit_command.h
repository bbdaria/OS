#ifndef QUIT_COMMAND_H_
#define QUIT_COMMAND_H_

#include <unistd.h> // getcwd
#include "built_in/built_in_command.h"
#include "code/smash/Smash.h"
#include "code/joblist/joblist.h"

class QuitCommand : public BuiltInCommand {
public:
    QuitCommand(char** args, int words) : m_kill_jobsList(false) {
        if (words >= 2) {
            std::string kill_str = "kill";
            if (kill_str.compare(args[1])) {
                m_kill_jobsList = true;
            }
        }
    }
    virtual ~QuitCommand()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        if (m_kill_jobsList) {
            jobsList.killAllJobs(this);
        }
    }
private:
    bool m_kill_jobsList;
};

#endif // QUIT_COMMAND_H_