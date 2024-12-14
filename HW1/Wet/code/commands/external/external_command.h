#ifndef EXTERNAL_COMMAND_H_
#define EXTERNAL_COMMAND_H_

#include "commands/Commands.h"
#include "smash/smash.h"
#include "util.h"

class ExternalCommand : public Command {
public:
    ExternalCommand(const char *original_cmd_line, std::string& cmd_line, bool background)
            : m_background(background), m_original_cmd_line(original_cmd_line), m_cmd_line(cmd_line) {
    }
    ~ExternalCommand()=default;

    static bool isComplexCommand(const std::string& cmdLine) {
        return cmdLine.find('*') != std::string::npos || cmdLine.find('?') != std::string::npos;
    }

    void execute() {
        if (m_background) {
            int pid = fork();
            if (pid < 0) {
                perror("smash error: fork failed");
                return;
            }
            if (pid == 0) {
                // child
                setpgrp(); // Create a new process group for the child
                actualExecute(nullptr);
                exit(0);
            }
            else {
                SmallShell& smash = SmallShell::getInstance();
                JobsList& jobsList = smash.getJobsList();
                jobsList.addJob(m_original_cmd_line.c_str(), pid, false);
            }
        }
        else {
            SmallShell& smash = SmallShell::getInstance();
            JobsList::JobEntry* fgJob = new JobsList::JobEntry(m_original_cmd_line.c_str());
            smash.setForeGround(fgJob);
            actualExecute(fgJob);
        }
    }

    virtual void actualExecute(JobsList::JobEntry* childJob) = 0;
private:
    bool m_background;
    std::string m_original_cmd_line;
protected:
    std::string m_cmd_line;
};

#endif // EXTERNAL_COMMAND_H_