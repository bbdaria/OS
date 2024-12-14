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
    Command() : m_isRedirection(false) {}
    virtual ~Command() {
        if (m_isRedirection) {
            stdRevertRedirect();
        }
    }

    virtual void execute() = 0;

    void redirectIO(std::pair<bool,std::pair<bool,std::string>> redirectIO) {
        m_isRedirection = redirectIO.first;
        if (m_isRedirection) {
            m_redirectionAppend = redirectIO.second.first;
            m_redirectionFile = redirectIO.second.second;
            m_stdFlag = STDOUT_FILENO;
            stdRedirect();
        }
    }

    void pipeRedirect(bool isErrPipe, std::string file) {
        m_isRedirection = true;
        m_redirectionFile = file;
        m_stdFlag = isErrPipe ? STDERR_FILENO : STDOUT_FILENO;
        stdRedirect();
    }

    void printOut(char chr) {
        std::string str(1, chr);
        printOut(str);
    }
    void printOut(const std::string& str) {
        if (str.compare("\n") == 0) {
            std::cout << std::endl;
        }
        else {
            // Output the str
            std::cout << str;
        }
    }

    void printErr(const std::string& str) {
        std::cerr << str;
    }
private:
    bool m_isRedirection;
    std::string m_redirectionFile;
    bool m_redirectionAppend; // True for '>>', false for '>'

    int m_originalStdoutFd = -1;
    int m_stdFlag = STDOUT_FILENO;
    
    void stdRevertRedirect() {
        // Restore original stdout if redirection was used
        if (m_isRedirection) {
            if (dup2(m_originalStdoutFd, m_stdFlag) == -1) {
                perror("smash error: dup2 failed");
            }
            close(m_originalStdoutFd);
        }
    }

    void stdRedirect() {
        int fd;
        m_originalStdoutFd = -1;

        if (m_isRedirection) {
            m_originalStdoutFd = dup(m_stdFlag);
            if (m_originalStdoutFd == -1) {
                perror("smash error: dup failed");
                return;
            }

            // Determine the flags for open()
            int overrideFlag = m_redirectionAppend ? O_APPEND : O_TRUNC;
            int flags = O_WRONLY | O_CREAT | overrideFlag;
            fd = open(m_redirectionFile.c_str(), flags, 0666);
            if (fd < 0) {
                perror("smash error: open failed");
                close(m_originalStdoutFd);
                return;
            }

            if (dup2(fd, m_stdFlag) == -1) {
                perror("smash error: dup2 failed");
                close(fd);
                close(m_originalStdoutFd);
                return;
            }
            close(fd);
        }
    }
};

#endif // SMASH_COMMAND_H_