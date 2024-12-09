#ifndef CHANGE_DIR_COMMAND_H_
#define CHANGE_DIR_COMMAND_H_

#include "built_in/built_in_command.h"
#include <unistd.h> // For getcwd
#include <iostream> // For std::cout and std::cerr

class ChangeDirCommand : public BuiltInCommand {
    std::string m_dir = "";
    ChangeDirCommand(const char *cmd_line, char **plastPwd) : BuiltInCommand(cmd_line) {
        //TODO: give value to m_dir
    }
    virtual ~ChangeDirCommand() = default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        smash.setStartPrompt(m_dir);
    }
};

#endif // CHANGE_DIR_COMMAND_H_