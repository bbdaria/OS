#ifndef QUIT_COMMAND_H_
#define QUIT_COMMAND_H_

#include <unistd.h> // getcwd
#include "built_in_command.h"
#include "smash.h"
#include "joblist.h"

class QuitCommand : public BuiltInCommand {
public:
    QuitCommand(char** args, int words) : m_kill_jobsList(false) {
        if (words >= 2) {
            std::string kill_str = "kill";
            if (kill_str.compare(args[1]) == 0) {
                m_kill_jobsList = true;
            }
        }
    }
    virtual ~QuitCommand()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        JobsList& jobsList = smash.getJobsList();
        if (m_kill_jobsList) {
            std::string prompt = smash.getStartPrompt();
            std::string number = std::to_string(jobsList.size());
            std::cout << prompt << ": sending SIGKILL signal to " << number << " jobs:" << std::endl;
            jobsList.killAllJobs();
        }
        smash.quit();
    }
private:
    bool m_kill_jobsList;
};

#endif // QUIT_COMMAND_H_