#ifndef FOREGROUND_COMMAND_H_
#define FOREGROUND_COMMAND_H_

#include <unistd.h> // getcwd
#include <sys/wait.h>
#include "built_in_command.h"
#include "smash.h"
#include "joblist.h"

class ForegroundCommand : public BuiltInCommand {
public:
    ForegroundCommand(char** args, int words) : m_error(false) {
        m_isMax = (words == 1);
        if (m_isMax) return;

        std::string id = args[1];
        if (words > 2 || !_isDigitsOnly(id.c_str())) {
            m_err_msg = "smash error: fg: invalid arguments";
            m_error = true;
        }
        else {
            m_jobId = std::stoi(id);
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

        if (m_isMax) {
            m_jobId = jobsList.getMaxId();
        }
        if (m_jobId == 0) {
            m_err_msg = "smash error: fg: jobs list is empty";
            m_error = true;
            std::cerr << m_err_msg << std::endl;
            return;
        }

        JobsList::JobEntry* jobEntry = jobsList.removeJobById(m_jobId);

        if (jobEntry == nullptr) {
            m_err_msg = "smash error: fg: job-id ";
            m_err_msg.append(std::to_string(m_jobId));
            std::string a = " does not exist";
            m_err_msg.append(a.c_str());
            m_error = true;
            std::cerr << m_err_msg << std::endl;
            return;
        }
        smash.setForeGround(jobEntry);
        int fgPID = jobEntry->getPID();
        std::string cmd_line = jobEntry->getCmdLine();
        std::cout << cmd_line << " " << std::to_string(fgPID) << std::endl;

        if (waitpid(fgPID, nullptr, 0) == -1) {
            perror("smash error: waitpid failed");
            return;
        }
    }
private:
    bool m_error;
    std::string m_err_msg;
    
    int m_jobId;
    int m_isMax;
};

#endif // FOREGROUND_COMMAND_H_