#ifndef EXTERNAL_COMMAND_H_
#define EXTERNAL_COMMAND_H_

#include "Commands.h"
#include "util.cpp"
#include <iostream> // std::cout and std::cerr

class ExternalCommand : public Command {
public:
    ExternalCommand(char *cmd_line) : Command() {
        m_background = _isBackgroundComamnd(cmd_line);
        _removeBackgroundSign(cmd_line);
        m_cmd_line = _trim(cmd_line);
    }
    ~ExternalCommand()=default;

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
                jobsList.addJob(m_cmd_line.c_str(), pid, false);
                std::cout << std::endl;
            }
        }
        else {
            actualExecute();
        }
    }

    virtual void actualExecute() = 0;
private:
    bool m_background;
    std::string m_cmd_line;
};

#endif // EXTERNAL_COMMAND_H_