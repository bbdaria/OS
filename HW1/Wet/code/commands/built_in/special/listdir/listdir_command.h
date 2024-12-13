#ifndef LISTDIR_COMMAND_H_
#define LISTDIR_COMMAND_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <string>
#include "built_in/built_in_command.h"

class ListDirCommand : public BuiltInCommand {
public:
    ListDirCommand(char** args, int words) : m_error(false) {
        if (words > 2) {
            m_error = true;
            m_err_msg = "smash error: listdir: too many arguments";
        }
        else {
            // Default to current directory if no argument is provided.
            m_dirPath = (words == 1) ? "." : args[1];
        }
    }

    ~ListDirCommand() = default;

    void execute() override {
        if (m_error) {
            printErr(m_err_msg);
            printErr("\n");
            return;
        }

        // Attempt to open the directory using syscall
        int dirFd = openDirectory(m_dirPath.c_str());
        if (dirFd == -1) {
            perror("smash error: stat failed");
            return;
        }
        printOut(m_dirPath);
        printOut("\n");
        listDirectory(dirFd, m_dirPath.c_str(), 1); // Start recursive listing with indentation level 1.

        close(dirFd);
    }

private:
    bool m_error;
    std::string m_err_msg;
    std::string m_dirPath;

    int openDirectory(const char* dirPath) {
        struct stat statbuf;
        if (stat(dirPath, &statbuf) == -1 || !S_ISDIR(statbuf.st_mode)) {
            // Return -1 if it's not a directory or stat fails
            return -1;
        }

        // Open directory manually using syscall
        int dirFd = syscall(SYS_open, dirPath, O_RDONLY | O_DIRECTORY);
        return dirFd;
    }

    // Helper to print the name with proper indentation
    void printWithIndent(const std::string& name, int indentLevel) {
        for (int i = 0; i < indentLevel; ++i) {
            printOut("\t");
        }
        printOut(name);
        printOut("\n");
    }

    // Function to list directory contents recursively
    void listDirectory(int dirFd, const char* dirPath, int indentLevel) {
        // Buffer for reading directory entries
        char buffer[4096];
        struct linux_dirent64 {
            ino64_t d_ino;
            off64_t d_off;
            unsigned short d_reclen;
            unsigned char d_type;
            char d_name[];
        };
        ssize_t bytesRead;

        std::vector<std::string> directories;
        std::vector<std::string> files;

        // Read directory entries
        while ((bytesRead = syscall(SYS_getdents64, dirFd, buffer, sizeof(buffer))) > 0) {
            char* ptr = buffer;
            while (ptr < buffer + bytesRead) {
                linux_dirent64* entry = (linux_dirent64*)ptr;

                // Skip "." and ".."
                if (std::strcmp(entry->d_name, ".") == 0 || std::strcmp(entry->d_name, "..") == 0) {
                    ptr += entry->d_reclen;
                    continue;
                }

                // Full path to the entry
                std::string fullPath = std::string(dirPath) + "/" + entry->d_name;

                // Check entry type (using stat to differentiate directories and files)
                struct stat statbuf;
                if (stat(fullPath.c_str(), &statbuf) == 0) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        directories.push_back(entry->d_name);
                    } else {
                        files.push_back(entry->d_name);
                    }
                }

                ptr += entry->d_reclen;
            }
        }

        if (bytesRead == -1) {
            perror("smash error: syscall failed");
            return;
        }

        // Sort directories and files alphabetically
        std::sort(directories.begin(), directories.end());
        std::sort(files.begin(), files.end());

        // Print directories first
        for (const std::string& dirName : directories) {
            printWithIndent(dirName, indentLevel);

            // Open directory for recursive call
            std::string newDirPath = std::string(dirPath) + "/" + dirName;
            int newDirFd = openDirectory(newDirPath.c_str());
            if (newDirFd != -1) {
                listDirectory(newDirFd, newDirPath.c_str(), indentLevel + 1);
                close(newDirFd);
            }
        }

        // Print files
        for (const std::string& fileName : files) {
            printWithIndent(fileName, indentLevel);
        }
    }
};

#endif // LISTDIR_COMMAND_H_