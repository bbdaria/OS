#ifndef ALIAS_COMMAND_H_
#define ALIAS_COMMAND_H_

#include <iostream>
#include <regex>
#include <string>
#include "../built_in_command.h"
#include "smash/Smash.h"

class AliasCommand : public BuiltInCommand {
public:
    AliasCommand(char *cmd_line) : m_cmd_line(cmd_line) {}

    void execute() override {
        std::string cmdStr(m_cmd_line);

        if (cmdStr == "alias") {
            // No arguments: List all aliases
            listAliases();
        } else if (cmdStr.substr(0, 7) == "unalias") {
            handleUnaliasCommand(cmdStr);
        } else {
            // Validate and add alias
            handleAliasCommand();
        }
    }

private:
    std::string m_cmd_line;

    bool isReservedCommand(const std::string &name) {
        // List of reserved commands
        auto reserved = SmallShell::getInstance().getReservedWords();
        return reserved.find(name) != reserved.end();
    }

    static bool isValidAliasFormat(const std::string &cmdStr, std::string &name, std::string &command) {
        std::regex aliasRegex("^alias ([a-zA-Z0-9_]+)='([^']*)'$", std::regex::extended);
        std::smatch match;

        if (std::regex_match(cmdStr, match, aliasRegex)) {
            name = match[1];
            command = match[2];
            return true;
        }
        return false;
    }

    void handleAliasCommand() {
        std::string name, command;
        if (!isValidAliasFormat(m_cmd_line, name, command)) {
            std::cerr << "smash error: alias: invalid alias format" << std::endl;
            return;
        }

        auto& aliases = SmallShell::getInstance().getAliases();
        if (isReservedCommand(name) || aliases.find(name) != aliases.end()) {
            std::cerr << "smash error: alias: " << name << " already exists or is a reserved command" << std::endl;
            return;
        }

        // Add alias
        aliases[name] = command;
    }

    void handleUnaliasCommand(const std::string& cmdStr) {
        std::regex unaliasRegex("^unalias ([a-zA-Z0-9_]+)$");
        std::smatch match;

        if (!std::regex_match(cmdStr, match, unaliasRegex)) {
            std::cerr << "smash error: unalias: invalid unalias format" << std::endl;
            return;
        }

        std::string name = match[1];
        auto& aliases = SmallShell::getInstance().getAliases();

        if (aliases.find(name) == aliases.end()) {
            std::cerr << "smash error: unalias: " << name << " does not exist" << std::endl;
            return;
        }

        // Remove alias
        aliases.erase(name);
    }

    void listAliases() {
        auto& aliases = SmallShell::getInstance().getAliases();
        for (const auto &alias : aliases) {
            std::cout << alias.first << "='" << alias.second << "'" << std::endl;
        }
    }
};

#endif // ALIAS_COMMAND_H_