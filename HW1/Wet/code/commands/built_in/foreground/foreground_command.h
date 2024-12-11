#ifndef FOREGROUND_COMMAND_H_
#define FOREGROUND_COMMAND_H_

#include <unistd.h> // getcwd
#include <iostream> // std::cout and std::cerr
#include <sys/wait.h>
#include "built_in/built_in_command.h"
#include "code/smash/Smash.h"
#include "code/joblist/joblist.h"

class ForegroundCommand : public BuiltInCommand {
public:
    ForegroundCommand(char** args, int words) : m_error(false) {
        if (words > 2 || (!_isDigitsOnly(args[1]))) {
            m_err_msg = "smash error: fg: invalid arguments";
            m_error = true;
        }
        else {
            SmallShell &smash = SmallShell::getInstance();
            JobsList& jobsList = smash.getJobsList();
            if (words == 2) {
                m_jobId = std::stoi(args[1]);
                if (!jobsList.contains(m_jobId)) {
                    m_err_msg = "smash error: fg: job-id ";
                    m_err_msg.append(args[1]);
                    std::string a = " does not exist";
                    m_err_msg.append(a.c_str());
                    m_error = true;
                }
            }
            else {
                m_jobId = jobsList.getMaxId();
            }
            if (!m_error) {
                if (jobsList.empty()) {
                    m_err_msg = "smash error: fg: jobs list is empty";
                    m_error = true;
                }
            }
        }
    }

    ~ForegroundCommand() = default;

    void execute() override {
        if (m_error) {
            std::cerr << m_err_msg << std::endl;
            return;
        }
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        JobsList::JobEntry* jobEntry = jobsList.getJobById(m_jobId);
        smash.setForeGround(jobEntry);
        jobEntry->printForegroundJob();

        if (waitpid(jobEntry->getPID(), nullptr) == -1) {
            perror("smash error: waitpid failed");
            return;
        }
        std::cout << std::endl;
    }
private:
    bool m_error;
    std::string m_err_msg;
    int m_jobId;
};

#endif // FOREGROUND_COMMAND_H_