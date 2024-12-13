#ifndef GET_CURRENT_DIR_COMMAND_H_
#define GET_CURRENT_DIR_COMMAND_H_

#include <unistd.h> // getcwd
#include "built_in/built_in_command.h"

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand()=default;
    ~GetCurrDirCommand()=default;

    void execute() override {
        char *cwd = getcwd(nullptr, 0);
        if (cwd != nullptr) {
            printOut(cwd);
            printOut(std::endl);
            free(cwd);
        }
        else {
            // code should NEVER reach here!!!
            perror("smash error: getcwd failed");
        }
    }
};

#endif // GET_CURRENT_DIR_COMMAND_H_