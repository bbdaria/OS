#ifndef FOREGROUND_COMMAND_H_
#define FOREGROUND_COMMAND_H_

#include <unistd.h> // getcwd
#include <sys/wait.h>
#include "../built_in_command.h"
#include "smash/smash.h"
#include "joblist/joblist.h"

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
            printErr(m_err_msg);
            printErr("\n");
            return;
        }
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        JobsList::JobEntry* jobEntry = jobsList.getJobById(m_jobId);
        smash.setForeGround(jobEntry);
        
        int fgPID = jobEntry->getPID();
        std::string cmd_line = jobEntry->getCmdLine();
        printOut(cmd_line);
        printOut(" ");
        printOut(std::to_string(fgPID));

        if (waitpid(fgPID, nullptr, 0) == -1) {
            perror("smash error: waitpid failed");
            return;
        }
        printOut("\n");
    }
private:
    bool m_error;
    std::string m_err_msg;
    int m_jobId;
};

#endif // FOREGROUND_COMMAND_H_