#ifndef WHOAMI_COMMAND_H_
#define WHOAMI_COMMAND_H_

#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../built_in_command.h"

class WhoamiCommand : public BuiltInCommand {
public:
    WhoamiCommand()=default;
    ~WhoamiCommand()=default;

    void execute() override {
        uid_t uid = geteuid();
        
        // Open /etc/passwd file manually using system calls
        int fd = open("/etc/passwd", O_RDONLY);
        if (fd == -1) {
            perror("smash error: open failed");
            return;
        }

        const size_t bufferSize = 4096;
        char buffer[bufferSize];
        ssize_t bytesRead;
        std::string currentLine;

        while ((bytesRead = read(fd, buffer, bufferSize)) > 0) {
            for (ssize_t i = 0; i < bytesRead; ++i) {
                char c = buffer[i];
                if (c == '\n') {
                    // Process the current line (line contains one user entry)
                    if (processLine(currentLine, uid)) {
                        close(fd);
                        return;
                    }
                    currentLine.clear(); // Clear the line buffer for the next line
                } else {
                    currentLine.push_back(c); // Add char to the current line
                }
            }
        }

        // If no user is found or an error occurs while reading the file
        perror("smash error: user not found in /etc/passwd");
        close(fd);
    }

private:
    bool processLine(const std::string& line, uid_t uid) {
        size_t pos = 0;
        size_t nextPos = line.find(':', pos);
        if (nextPos == std::string::npos) return false;
        std::string username = line.substr(pos, nextPos - pos);

        pos = nextPos + 1;
        nextPos = line.find(':', pos);
        if (nextPos == std::string::npos) return false;
        std::string password = line.substr(pos, nextPos - pos); // Unused

        pos = nextPos + 1;
        nextPos = line.find(':', pos);
        if (nextPos == std::string::npos) return false;
        std::string uidStr = line.substr(pos, nextPos - pos);

        // Convert uidStr to an integer and compare with current process uid
        if (std::stoi(uidStr) == uid) {
            // Get the home directory
            pos = nextPos + 1;
            nextPos = line.find(':', pos);
            if (nextPos == std::string::npos) return false;
            std::string homedir = line.substr(pos, nextPos - pos);

            printOut(username);
            printOut(" ");
            printOut(homedir);
            printOut("\n");
            return true;
        }
        return false;
    }
};

#endif // WHOAMI_COMMAND_H_