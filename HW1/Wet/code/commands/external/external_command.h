#ifndef EXTERNAL_COMMAND_H_
#define EXTERNAL_COMMAND_H_

#include "Commands.h"
#include "util.cpp"

class ExternalCommand : public Command {
public:
    ExternalCommand(char *original_cmd_line) : m_original_cmd_line(original_cmd_line) {
        m_background = _isBackgroundComamnd(original_cmd_line);
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
                actualExecute();
                exit(0);
            }
            else {
                SmallShell& smash = SmallShell::getInstance();
                JobsList& jobsList = smash.getJobsList();
                jobsList.addJob(m_original_cmd_line.c_str(), pid, false);
            }
        }
        else {
            actualExecute();
        }
    }

    virtual void actualExecute() = 0;
private:
    bool m_background;
protected:
    std::string m_original_cmd_line;
};

#endif // EXTERNAL_COMMAND_H_