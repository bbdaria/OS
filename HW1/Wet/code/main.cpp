#include <unistd.h>
// #include <sys/wait.h>
#include <signal.h> // library usage
#include "signals/signals.h"
#include "smash/smash.h"

int main(int argc, char *argv[]) {
    if (signal(SIGINT, ctrlCHandler) == SIG_ERR) {
        perror("smash error: failed to set ctrl-C handler");
    }

    SmallShell &smash = SmallShell::getInstance();
    smash.launch();
    return 0;
}