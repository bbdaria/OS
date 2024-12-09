#ifndef GET_CURRENT_DIR_COMMAND_H_
#define GET_CURRENT_DIR_COMMAND_H_

#include <unistd.h> // For getcwd
#include <iostream> // For std::cout and std::cerr
#include "built_in/built_in_command.h"

#define PATH_MAX 4096 // reasonable default if not defined

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}
    ~GetCurrDirCommand() override = default;

    void execute() override {
        char buffer[PATH_MAX]; // PATH_MAX is a constant defining the max path length
        if (getcwd(buffer, sizeof(buffer)) != nullptr) {
            std::cout << buffer << std::endl;
        } else {
            std::cerr << "Error: Unable to retrieve current directory." << std::endl;
        }
    }
};

#endif // GET_CURRENT_DIR_COMMAND_H_