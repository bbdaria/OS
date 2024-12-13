#ifndef SIMPLE_EXTERNAL_COMMAND_H_
#define SIMPLE_EXTERNAL_COMMAND_H_

#include "../external_command.h"
#include "util.h"

class SimpleExternalCommand : public ExternalCommand {
public:
    SimpleExternalCommand(const char *original_cmd_line, std::string& cmd_line)
        : ExternalCommand(original_cmd_line, cmd_line) {}

    void actualExecute(JobsList::JobEntry* childJob) override {
        int pid = fork();
        if (pid < 0) {
            perror("smash error: fork failed");
            return;
        }
        if (pid == 0) {
            // child

            // Parse command and arguments
            char* args[COMMAND_MAX_ARGS + 1] = {nullptr};
            char* cmdCStr = strdup(m_cmd_line.c_str());
            int numArgs = _parseCommandLine(cmdCStr, args);
            if (numArgs < 0) {
                perror("smash error: parsing command line failed");
                free(cmdCStr);
                exit(1);
            }

            execvp(args[0], args);
            perror("smash error: execvp failed");
            free(cmdCStr);
            exit(1);
        }
        else {
            if (childJob) childJob->setPID(pid);
            // Parent process waits for the child process to finish
            if (waitpid(pid, nullptr, WUNTRACED) < 0) {
                perror("smash error: waitpid failed");
            }
        }
    }
};

#endif // SIMPLE_EXTERNAL_COMMAND_H_