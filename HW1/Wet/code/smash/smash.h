#ifndef SMASH_H_
#define SMASH_H_

#include <iostream>
#include "code/commands/Commands.h"

static std::string DEFAULT_START_PROMPT = "smash> ";

class SmallShell {
private:
    SmallShell();
    std::string m_start_prompt = DEFAULT_START_PROMPT;

public:
    Command *CreateCommand(const char *cmd_line);

    SmallShell(SmallShell const &) = delete; // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator
    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    ~SmallShell();

    void executeCommand(const char *cmd_line);

    void launch() {
        while (true) {
            std::cout << m_start_prompt;
            std::string cmd_line;
            std::getline(std::cin, cmd_line);
            executeCommand(cmd_line.c_str());
        }
    }
};

#endif //SMASH_H_