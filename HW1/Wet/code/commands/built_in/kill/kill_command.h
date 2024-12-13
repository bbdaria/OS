#ifndef KILL_COMMAND_H_
#define KILL_COMMAND_H_

#include <unistd.h> // getcwd
#include <cstring>  // strerror
#include <cstdlib>  // atoi
#include "built_in/built_in_command.h"
#include "code/smash/Smash.h"
#include "code/joblist/joblist.h"
#include "util.cpp"

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
            printErr(m_err_msg);
            printErr("\n");
            return;
        }
        SmallShell& smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();

        if (!jobsList.contains(m_jobId)) {
            printErr("smash error: kill: job-id ");
            printErr(std::to_string(m_jobId));
            printErr(" does not exist");
            printErr("\n");
            return;
        }

        JobsList::JobEntry* jobEntry = jobsList.getJobById(m_jobId);
        int pid = jobEntry->getPID();

        if (kill(pid, m_sigNum) == -1) {
            perror("smash error: kill failed");
            return;
        }

        printOut("signal number ");
        printOut(std::to_string(m_sigNum));
        printOut(" was sent to pid ");
        printOut(std::to_string(pid));
        printOut("\n");
    }

private:
    bool m_error;
    std::string m_err_msg;
    int m_sigNum;
    int m_jobId;
};

#endif // KILL_COMMAND_H_