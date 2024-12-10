#ifndef CHPROMPT_COMMAND_H_
#define CHPROMPT_COMMAND_H_

#include "built_in/built_in_command.h"
#include "code/smash/smash.h"

class Chprompt : public BuiltInCommand {
public:
    Chprompt(const char *cmd_line) : BuiltInCommand(cmd_line) {
        //TODO: give value to m_prompt
    }
    virtual ~Chprompt()=default;

    void execute() override {
        SmallShell &smash = SmallShell::getInstance();
        smash.setStartPrompt(m_prompt);
    }
private:
    std::string m_prompt = "";
};

#endif // CHPROMPT_COMMAND_H_