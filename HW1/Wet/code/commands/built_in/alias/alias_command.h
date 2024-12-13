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
        std::string cmd_s(m_cmd_line);

        if (_trim(cmd_s).compare("alias") == 0) {
            // print alias list
            auto& aliases = SmallShell::getInstance().getAliases();
            for (const auto &alias : aliases) {
                std::string name = alias.first;
                std::pair<std::string,char> val = alias.second;
                printOut(name);
                printOut("=");
                printOut(val.second);
                printOut(val.first);
                printOut(val.second);
                printOut(std::endl);
            }
            return;
        }
        
        std::string name;
        std::string command;
        char quotationMarks;
        bool validFormat = isValidAliasFormat(cmd_s, name, command, &quotationMarks);
        if (!validFormat) {
            printErr("smash error: alias: invalid alias format");
            printErr(std::endl);
            return;
        }
        else {
            // Validate and add alias
            auto& aliases = SmallShell::getInstance().getAliases();
            if (isReservedCommand(name) || aliases.find(name) != aliases.end()) {
                printErr("smash error: alias: ");
                printErr(name);
                printErr(" already exists or is a reserved command");
                printErr(std::endl);
                return;
            }
            // Add alias
            aliases[name] = std::pair<std::string, char>(command, quotationMarks);
        }
    }

private:
    bool isReservedCommand(const std::string &name) {
        // List of reserved commands
        auto reserved = SmallShell::getInstance().getReservedWords();
        return reserved.find(name) != reserved.end();
    }

    static bool isValidAliasFormat(const std::string &cmdStr, std::string& name, std::string& command, char& quotationMarks) {
        std::regex aliasRegex("^alias  [a-zA-Z0-9_]+='[^']*'$", std::regex::extended);
        std::smatch match;

        if (std::regex_match(cmdStr, match, aliasRegex)) {
            name = match[1];
            command = match[2];

            // alias <name>= and then ' or ` or ` or ’...
            std::size_t namePos = cmdStr.find(name);

            quotationMarks = cmdStr.at(namePos+name.size()+1);
            return true;
        }
        return false;
    }
};

#endif // ALIAS_COMMAND_H_