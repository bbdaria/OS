#ifndef UNALIAS_COMMAND_H_
#define UNALIAS_COMMAND_H_

#include <iostream>
#include <string>
#include <vector>
#include "../built_in_command.h"
#include "smash/Smash.h"

class UnaliasCommand : public BuiltInCommand {
public:
    UnaliasCommand(char** args, int words) : m_error(false) {
        if (words <= 1) {
            m_err_msg = "smash error: unalias: not enough arguments";
            m_error = true;
        } else {
            for (int i = 1; i < words; ++i) {
                m_names.emplace_back(args[i]);
            }
        }
    }

    void execute() override {
        if (m_error) {
            printErr(m_err_msg);
            printErr("\n");
            return;
        }

        SmallShell& smash = SmallShell::getInstance();
        auto& aliases = smash.getAliases();

        for (const std::string& alias : m_names) {
            if (aliases.erase(alias) == 0) {
                // Report error if alias does not exist
                printErr("smash error: unalias: ");
                printErr(alias);
                printErr(" alias does not exist");
                printErr("\n");
                return;
            }
        }
    }

private:
    bool m_error;
    std::string m_err_msg;
    std::vector<std::string> m_names; // aliases to remove
};

#endif // UNALIAS_COMMAND_H_