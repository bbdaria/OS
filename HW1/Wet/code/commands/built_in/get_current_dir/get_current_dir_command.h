#ifndef GET_CURRENT_DIR_COMMAND_H_
#define GET_CURRENT_DIR_COMMAND_H_

#include <unistd.h> // getcwd
#include <iostream> // std::cout and std::cerr
#include "built_in/built_in_command.h"

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}
    ~GetCurrDirCommand() override = default;

    void execute() override {
        char *cwd = getcwd(nullptr, 0);
        if (cwd != nullptr) {
            std::cout << cwd << std::endl;
            free(cwd);
        }
        else {
            // code should NEVER reach here!!!
            std::cerr << "Error: Unable to retrieve current directory." << std::endl;
        }
    }
};

#endif // GET_CURRENT_DIR_COMMAND_H_