#ifndef COMPLEX_EXTERNAL_COMMAND_H_
#define COMPLEX_EXTERNAL_COMMAND_H_

#include "../external_command.h"
#include "util.h"

class ComplexExternalCommand : public ExternalCommand {
public:
    ComplexExternalCommand(const char *original_cmd_line, std::string& cmd_line, bool background)
        : ExternalCommand(original_cmd_line, cmd_line, background) {}

    void actualExecute(JobsList::JobEntry* childJob) override {
        int pid = fork();
        if (pid < 0) {
            perror("smash error: fork failed");
            return;
        }
        if (pid == 0) {
            // child

            // Prepare arguments for bash
            char* args[] = {
                const_cast<char*>("/bin/bash"),
                const_cast<char*>("-c"),
                const_cast<char*>(m_cmd_line.c_str()),
                nullptr
            };

            execv("/bin/bash", args);
            perror("smash error: execv failed");
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

#endif // COMPLEX_EXTERNAL_COMMAND_H_