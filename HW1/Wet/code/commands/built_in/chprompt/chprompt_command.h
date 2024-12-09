#ifndef CHPROMPT_COMMAND_H_
#define CHPROMPT_COMMAND_H_

#include "built_in/built_in_command.h"
#include "code/smash/smash.cpp"

class Chprompt : public BuiltInCommand {
public:
    std::string m_prompt = "";
    Chprompt(const char *cmd_line) {
        // update m_prompt according to the second word in cmd_line
    }

    virtual ~Chprompt()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        smash.setStartPrompt(m_prompt);
    }
};

#endif // CHPROMPT_COMMAND_H_