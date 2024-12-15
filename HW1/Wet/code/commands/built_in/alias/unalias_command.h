#ifndef UNALIAS_COMMAND_H_
#define UNALIAS_COMMAND_H_

#include <iostream>
#include <string>
#include <vector>
#include "built_in_command.h"
#include "smash.h"

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
            std::cerr << m_err_msg << std::endl;
            return;
        }
        
        for (const std::string& aliasName : m_names) {
            bool removed = removeAliasByName(aliasName);
            if (!removed) {
                // Report error if alias does not exist
                std::cerr << "smash error: unalias: " << aliasName << " alias does not exist" << std::endl;
                return;
            }
        }
    }

private:
    bool m_error;
    std::string m_err_msg;
    std::vector<std::string> m_names; // aliases to remove

    bool removeAliasByName(const std::string &aliasName) {
        auto& aliases = SmallShell::getInstance().getAliases();
        for (auto it = aliases.begin(); it != aliases.end();) {
            auto alias = *it;
            const std::string& name = alias.first;
            if (aliasName.compare(name) == 0) {
                aliases.erase(it);
                return true;
            }
            else {
                ++it;
            }
        }
        return false;
    }
};

#endif // UNALIAS_COMMAND_H_