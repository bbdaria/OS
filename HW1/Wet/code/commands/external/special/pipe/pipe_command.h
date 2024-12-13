#ifndef PIPE_COMMAND_H_
#define PIPE_COMMAND_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../../external_command.h"

class PipeCommand : public ExternalCommand {
public:
    PipeCommand(char *original_cmd_line, std::string& cmd_line)
        : ExternalCommand(original_cmd_line, cmd_line) {

        // Split the command line into two parts: before and after the pipe
        size_t pipePos = cmd_line.find('|');
        int splitSize = 1;
        if (pipePos != std::string::npos) {
            if (cmd_line[pipePos + 1] == '&') {
                m_isErrPipe = true;
                splitSize++;  // Skip over the '&' in "|&"
            }
            m_leftCmdLine = cmd_line.substr(0, pipePos);  // Command before the pipe
            m_rightCmdLine = cmd_line.substr(pipePos + splitSize);  // Command after the pipe
        }
    }

    void actualExecute() override {
        // Create the two commands from the left and right parts of the pipe
        Command* leftCmd = SmallShell::createCommand(m_leftCmdLine);

        if (m_isErrPipe) {
            leftCmd->setPipeErr();
        }
        else {
            leftCmd->setPipeOut();
        }
        leftCmd->execute();

        m_rightCmdLine.append(leftCmd->getPipeStr());
        Command* rightCmd = SmallShell::createCommand(m_rightCmdLine);
        rightCmd->execute();
    }

private:
    std::string m_leftCmdLine;
    std::string m_rightCmdLine;
    bool m_isErrPipe = false;
};

#endif // PIPE_COMMAND_H_