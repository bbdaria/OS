#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <errno.h>

#define COMMAND_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)

class Command {
public:
    Command() : m_isRedirection(false), m_errPipe(false), m_outPipe(false) {}
    virtual ~Command() = default;

    virtual void execute() = 0;

    void setRedirectionFile(const std::string& file, bool append) {
        m_redirectionFile = file;
        m_redirectionAppend = append;
        m_isRedirection = true;
    }

    void setPipeOut() {
        m_outPipe = true;
        m_errPipe = false;
    }

    void setPipeErr() {
        m_outPipe = false;
        m_errPipe = true;
    }

    void printOut(char chr) {
        std::string str(1, chr);
        printOut(str);
    }
    void printOut(const std::string& str) {
        if (m_outPipe) {
            m_pipeStr.append(str);
            return;
        }
        int fd;
        int originalStdoutFd = -1;

        if (m_isRedirection) {
            originalStdoutFd = dup(STDOUT_FILENO);
            if (originalStdoutFd == -1) {
                perror("smash error: dup failed");
                return;
            }

            // Determine the flags for open()
            int overrideFlag = m_redirectionAppend ? O_APPEND : O_TRUNC;
            int flags = O_WRONLY | O_CREAT | overrideFlag;
            fd = open(m_redirectionFile.c_str(), flags, 0666);
            if (fd < 0) {
                perror("smash error: open failed");
                close(originalStdoutFd);
                return;
            }

            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("smash error: dup2 failed");
                close(fd);
                close(originalStdoutFd);
                return;
            }
            close(fd);
        }

        // Output the str
        std::cout << str;

        // Restore original stdout if redirection was used
        if (m_isRedirection) {
            if (dup2(originalStdoutFd, STDOUT_FILENO) == -1) {
                perror("smash error: dup2 failed");
            }
            close(originalStdoutFd);
        }
    }

    void printErr(const std::string& str) {
        if (m_errPipe) {
            m_pipeStr.append(str);
            return;
        }
        std::cerr << str;
    }

    std::string getPipeStr() {
        return m_pipeStr;
    }

private:
    bool m_isRedirection;
    std::string m_redirectionFile;
    bool m_redirectionAppend; // True for '>>', false for '>'

    bool m_errPipe;
    bool m_outPipe;
    std::string m_pipeStr;
};

#endif // SMASH_COMMAND_H_