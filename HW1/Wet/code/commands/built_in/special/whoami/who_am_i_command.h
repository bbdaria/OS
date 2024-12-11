#ifndef WHOAMI_COMMAND_H_
#define WHOAMI_COMMAND_H_

#include <pwd.h>        // getpwuid
#include <unistd.h>     // geteuid
#include <iostream>     // std::cout, std::cerr
#include "built_in/built_in_command.h"

class WhoamiCommand : public BuiltInCommand {
public:
    WhoamiCommand()=default;
    ~WhoamiCommand()=default;

    void execute() override {
        int uid = geteuid(); // geteuid should never fail
        if (uid == -1) {
            perror("smash error: geteuid failed");
            return;
        }
        struct passwd* pw = getpwuid(uid);
        if (!pw) {
            perror("smash error: getpwuid failed");
            return;
        }
        std::cout << pw->pw_name << " " << pw->pw_dir << std::endl;
    }
};

#endif // WHOAMI_COMMAND_H_