#ifndef CHANGE_DIR_COMMAND_H_
#define CHANGE_DIR_COMMAND_H_

#include "built_in/built_in_command.h"
#include <unistd.h> // getcwd

class ChangeDirCommand : public BuiltInCommand {
public:
    ChangeDirCommand(char** args, int words) : m_error(false), m_skip(false) {
        if (words <= 1) {
            m_skip = true;
        }
        else if (words > 2) {
            m_err_msg = "smash error: cd: too many arguments";
            m_error = true;
        }
        else {
            m_dir = args[1];
            SmallShell& smash = SmallShell::getInstance();
            if (m_dir.compare("-") && smash.plastPwd()==nullptr) {
                m_err_msg = "smash error: cd: OLDPWD not set";
                m_error = true;
            }
        }
    }
    virtual ~ChangeDirCommand() = default;

    void execute() override {
        if (m_error) {
            printErr(m_err_msg);
            printErr("\n");
            return;
        }
        // this is built-in command and so will run at smash PCB
        // update smash last-working-directory
        char *currDir = getcwd(nullptr, 0);
        SmallShell& smash = SmallShell::getInstance();
        smash.updatePlastPwd(currDir);

        if (m_skip) return;

        if (m_dir.compare("-")) {
            m_dir = smash.plastPwd();
        }
        
        int result = chdir(m_dir.c_str());
        if (!result) perror("smash error: chdir failed");
    }

private:
    bool m_error;
    bool m_skip;
    std::string m_err_msg;
    std::string m_dir;
};

#endif // CHANGE_DIR_COMMAND_H_