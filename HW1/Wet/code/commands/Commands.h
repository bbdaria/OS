#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define COMMAND_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class Command {
public:
    Command() : m_isRedirection(false) {}
    Command(const std::string& file, bool append) : m_isRedirection(true) {
        setRedirectionFile(file, append);
    }
    virtual ~Command() = default;

    virtual void execute() = 0;

    // Print output either to the terminal or to a file based on redirection
    void printOut(const std::string str) {
        int fd;
        int originalStdoutFd = -1;

        if (m_isRedirection) {
            // Save the original standard output (stdout) to restore later
            originalStdoutFd = dup(STDOUT_FILENO);
            if (originalStdoutFd == -1) {
                perror("smash error: dup failed");
                return;
            }

            // Open the file for redirection (write only, create if doesn't exist)
            int overrideFlag = m_redirectionAppend ? O_APPEND : O_TRUNC;
            fd = open(m_redirectionFile.c_str(), O_WRONLY | O_CREAT | overrideFlag, 0666);
            if (fd < 0) {
                perror("smash error: open failed");
                close(originalStdoutFd);
                return;
            }

            // Redirect stdout to the file
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("smash error: dup2 failed");
                close(fd);
                close(originalStdoutFd);
                return;
            }
            close(fd);  // Close the file descriptor after redirection
        }

        // Output the string (either to the file or the terminal based on redirection)
        std::cout << str;

        // If redirection was performed, restore the original stdout
        if (m_isRedirection) {
            if (dup2(originalStdoutFd, STDOUT_FILENO) == -1) {
                perror("smash error: dup2 failed");
            }

            close(originalStdoutFd);
        }
    }

private:
    bool m_isRedirection;
    std::string m_redirectionFile;
    bool m_redirectionAppend;  // True for '>>', false for '>'

    // Sets the file for redirection and whether it should append or overwrite
    void setRedirectionFile(const std::string& file, bool append) {
        m_redirectionFile = file;
        m_redirectionAppend = append;
        m_isRedirection = true;
    }
};

#endif // SMASH_COMMAND_H_