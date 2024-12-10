#ifndef CHANGE_DIR_COMMAND_H_
#define CHANGE_DIR_COMMAND_H_

#include "built_in/built_in_command.h"
#include <unistd.h> // getcwd
#include <iostream> // std::cout and std::cerr

class ChangeDirCommand : public BuiltInCommand {
public:
    ChangeDirCommand(const char *cmd_line, char **plastPwd) : BuiltInCommand(cmd_line) {
        std::string cmd_str = cmd_line;
    }
    virtual ~ChangeDirCommand() = default;

    void execute() override {
        // this is built-in command and so will run at smash PCB
        char *currDir = getcwd(nullptr, 0);
        bool changed = false;
        
        // TODO: add logic (and apply change if needed) here

        if (changed) {
            if (plastPwd != nullptr && *plastPwd != nullptr) delete plastPwd;
            *plastPwd = currDir;
        }
        else {
            delete currDir;
        }
    }

private:
    std::string cmd_line = "";
    char **plastPwd = nullptr; // Pointer to the last working directory
};

#endif // CHANGE_DIR_COMMAND_H_