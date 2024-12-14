#ifndef PIPE_COMMAND_H_
#define PIPE_COMMAND_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include "../../external_command.h"

class PipeCommand : public ExternalCommand {
public:
    PipeCommand(const char *original_cmd_line, std::string& cmd_line, bool background)
        : ExternalCommand(original_cmd_line, cmd_line, background) {
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

    void actualExecute(JobsList::JobEntry* childJob) override {
        SmallShell& smash = SmallShell::getInstance();
        
        char tempFileTemplate[] = "/tmp/smash_pipe_output_XXXXXX";
        int tempFd = mkstemp(tempFileTemplate);
        if (tempFd < 0) {
            perror("smash error: mkstemp failed");
            return;
        }
        close(tempFd);
        std::string tempFile(tempFileTemplate);

        // Create and execute the left command
        std::string leftCmd_s;
        smash.preProcessCmdLine(m_leftCmdLine.c_str(), leftCmd_s); // ignore &
        Command* leftCmd = smash.createCommand(m_leftCmdLine.c_str(), leftCmd_s, false, false);
        leftCmd->pipeRedirect(m_isErrPipe, tempFile);
        leftCmd->execute();
        delete leftCmd;

        std::string fileContent(readFile(tempFile));

        // Delete the temporary file
        unlink(tempFile.c_str());

        if (fileContent.compare("\0") == 0) {
            return;
        }

        m_rightCmdLine.append(fileContent);
        smash.executeCommand(m_rightCmdLine.c_str());
    }

private:
    std::string m_leftCmdLine;
    std::string m_rightCmdLine;
    bool m_isErrPipe = false;

    std::string readFile(const std::string& file) {
        int fd = open(file.c_str(), O_RDONLY);
        if (fd < 0) {
            perror("smash error: open failed");
            return "\0";
        }

        // Read the file content using a buffer
        constexpr size_t BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        std::string fileContent;
        ssize_t bytesRead;
        while ((bytesRead = read(fd, buffer, BUFFER_SIZE)) > 0) {
            fileContent.append(buffer, bytesRead);
        }

        if (bytesRead < 0) {
            perror("smash error: failed to read temporary file");
            close(fd);
            return "\0";
        }
        close(fd);

        return fileContent;
    }
};

#endif // PIPE_COMMAND_H_