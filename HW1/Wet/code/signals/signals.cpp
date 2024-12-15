#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"
#include "smash.h"

using namespace std;

void ctrlCHandler(int sig_num) {
    SmallShell &smash = SmallShell::getInstance();
    JobsList::JobEntry* foreground = smash.getForeground();

    std::cout << smash.getStartPrompt() << ": got ctrl-C" << std::endl;

    if (foreground != nullptr) {
        int pid = foreground->getPID();
        if (kill(pid, SIGKILL) == -1) {
            perror("smash error: kill failed");
        } else {
            std::cout << smash.getStartPrompt() << ": process " << pid << " was killed";
            std::cout << std::endl;
        }
    }
}