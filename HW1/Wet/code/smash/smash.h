#ifndef SMASH_H_
#define SMASH_H_

#include <iostream>
#include "code/commands/Commands.h"
#include "code/joblist/joblist.h"

class SmallShell {
private:
    SmallShell() =default;
    ~SmallShell()=default;

    static const std::string DEFAULT_START_PROMPT;

    JobsList m_jobsList;
    std::string m_start_prompt = DEFAULT_START_PROMPT;

public:
    SmallShell(SmallShell const &) = delete; // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator

    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Command* createCommand(const char *cmd_line);
    void executeCommand(const char *cmd_line);

    void setStartPrompt(std::string& s) {
        m_start_prompt = (s == "" ? DEFAULT_START_PROMPT : s);
    }

    std::string getStartPrompt() const {
        return m_start_prompt;
    }

    void launch() {
        while (true) {
            std::cout << m_start_prompt << "> ";
            std::string cmd_line;
            std::getline(std::cin, cmd_line);
            executeCommand(cmd_line.c_str());
        }
    }
};

const std::string SmallShell::DEFAULT_START_PROMPT = "smash";

#endif //SMASH_H_