#ifndef CHANGE_DIR_COMMAND_H_
#define CHANGE_DIR_COMMAND_H_

#include "../built_in_command.h"
#include <unistd.h> // getcwd
#include <cstring>  // strerror
#include <cstdlib>  // free

class ChangeDirCommand : public BuiltInCommand {
public:
    ChangeDirCommand(char** args, int words) : m_error(false), m_skip(false) {
        if (words <= 1) {
            m_skip = true; // No directory specified
        }
        else if (words > 2) {
            m_err_msg = "smash error: cd: too many arguments";
            m_error = true;
        }
        else {
            m_dir = args[1];
            SmallShell& smash = SmallShell::getInstance();
            if (m_dir.compare("-")==0 && smash.plastPwd()==nullptr) {
                m_err_msg = "smash error: cd: OLDPWD not set";
                m_error = true;
            }
        }
    }
    virtual ~ChangeDirCommand() = default;

    void execute() override {
        if (m_error) {
            std::cerr << m_err_msg << std::endl;
            return;
        }
        // this is built-in command and so will run at smash PCB
        // update smash last-working-directory
        char *currDir = getcwd(nullptr, 0);
        if (!currDir) {
            perror("smash error: getcwd failed");
            return;
        }

        SmallShell& smash = SmallShell::getInstance();

        if (m_skip) {
            smash.updatePlastPwd(currDir);
            return;
        }

        if (m_dir.compare("-") == 0) {
            m_dir = smash.plastPwd();
        }

        // Attempt to change the directory
        if (chdir(m_dir.c_str()) != 0) {
            perror("smash error: chdir failed");
        }
        else {
            smash.updatePlastPwd(currDir);
        }
        free(currDir);
    }

private:
    bool m_error;
    bool m_skip;
    std::string m_err_msg;
    std::string m_dir;
};

#endif // CHANGE_DIR_COMMAND_H_