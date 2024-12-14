#ifndef KILL_COMMAND_H_
#define KILL_COMMAND_H_

#include <unistd.h> // getcwd
#include <cstring>  // strerror
#include <cstdlib>  // atoi
#include "../built_in_command.h"
#include "smash/smash.h"
#include "joblist/joblist.h"
#include "util.h"

class KillCommand : public BuiltInCommand {
public:
    KillCommand(char** args, int words) : m_error(false) {
        if (words != 3 || args[1][0] != '-' || !_isDigitsOnly(args[1] + 1) || !_isDigitsOnly(args[2])) {
            m_error = true;
            m_err_msg = "smash error: kill: invalid arguments";
            return;
        }

        m_sigNum = std::atoi(args[1] + 1); // Extract signal number (skip the '-' character)
        m_jobId = std::atoi(args[2]);

        if (m_sigNum <= 0) {
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
        SmallShell& smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();

        if (!jobsList.contains(m_jobId)) {
            std::cerr << "smash error: kill: job-id " << std::to_string(m_jobId) << " does not exist";
            std::cerr << std::endl;
            return;
        }

        JobsList::JobEntry* jobEntry = jobsList.getJobById(m_jobId);
        int pid = jobEntry->getPID();

        if (kill(pid, m_sigNum) == -1) {
            perror("smash error: kill failed");
            return;
        }

        std::cout << "signal number " << std::to_string(m_sigNum) << " was sent to pid " << std::to_string(pid);
        std::cout << std::endl;
    }

private:
    bool m_error;
    std::string m_err_msg;
    int m_sigNum;
    int m_jobId;
};

#endif // KILL_COMMAND_H_