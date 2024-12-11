#ifndef CHPROMPT_COMMAND_H_
#define CHPROMPT_COMMAND_H_

#include "built_in/built_in_command.h"
#include "code/smash/smash.h"

class Chprompt : public BuiltInCommand {
public:
    Chprompt(char** args, int words) : m_prompt("") {
        if (words >= 2) {
            m_prompt = args[1];
        }
    }
    virtual ~Chprompt()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        smash.setStartPrompt(m_prompt);
    }
private:
    std::string m_prompt;
};

#endif // CHPROMPT_COMMAND_H_