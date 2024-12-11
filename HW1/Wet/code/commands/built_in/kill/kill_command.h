#ifndef KILL_COMMAND_H_
#define KILL_COMMAND_H_

#include <unistd.h> // getcwd
#include <iostream> // std::cout and std::cerr
#include "built_in/built_in_command.h"
#include "code/smash/Smash.h"
#include "code/joblist/joblist.h"

class KillCommand : public BuiltInCommand {
public:
    KillCommand(char** args, int words) : m_error(false) {
        if (words <= 1) {
            m_error = true;
            m_err_msg = "smash error: kill: invalid arguments";
        }
    }
    virtual ~KillCommand()=default;

    void execute() override {
        if (m_error) {
            std::cerr << m_err_msg << std::endl;
            return;
        }
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        int jobId;
        int sig;

        JobsList::JobEntry* jobEntry = jobsList.getJobById(jobId);
        kill(jobEntry->getPID(), sig);
        std::cout << std::endl;
    }
private:
    bool m_error;
    std::string m_err_msg;
};

#endif // KILL_COMMAND_H_