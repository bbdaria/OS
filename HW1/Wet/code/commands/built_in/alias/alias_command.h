#ifndef ALIAS_COMMAND_H_
#define ALIAS_COMMAND_H_

#include <iostream>
#include <regex>
#include <string>
#include "../built_in_command.h"
#include "smash/smash.h"

class AliasCommand : public BuiltInCommand {
public:
    AliasCommand(const char *cmd_line) : m_cmd_line(cmd_line) {}

    void execute() override {
        std::string cmd_s(m_cmd_line);
        if (_trim(cmd_s).compare("alias") == 0) {
            // print alias list
            SmallShell& smash = SmallShell::getInstance();
            std::map<std::string, std::pair<std::string,char>>& aliases = smash.getAliases();
            for (const auto &alias : aliases) {
                std::string name = alias.first;
                std::pair<std::string,char> val = alias.second;
                std::cout << name << "=" << val.second << val.first << val.second << std::endl;
            }
            return;
        }
        
        std::string name;
        std::string command;
        char quotationMarks;
        bool validFormat = isValidAliasFormat(cmd_s, name, command, quotationMarks);
        if (!validFormat) {
            std::cerr << "smash error: alias: invalid alias format" << std::endl;
            return;
        }
        else {
            // Validate and add alias
            auto& aliases = SmallShell::getInstance().getAliases();
            if (isReservedCommand(name) || aliases.find(name) != aliases.end()) {
                std::cerr << "smash error: alias: " << name << " already exists or is a reserved command";
                std::cerr << std::endl;
                return;
            }
            // Add alias
            aliases[name] = std::pair<std::string, char>(command, quotationMarks);
        }
    }

private:
    std::string m_cmd_line;

    bool isReservedCommand(const std::string &name) {
        // List of reserved commands
        auto reserved = SmallShell::getInstance().getReservedWords();
        return reserved.find(name) != reserved.end();
    }

    static bool isValidAliasFormat(const std::string &cmdStr, std::string& name, std::string& command, char& quotationMarks) {
        std::regex aliasRegex(R"(^alias\s+([a-zA-Z0-9_]+)=(['`])(.*)\2$)");
        std::smatch match;

        if (std::regex_match(cmdStr, match, aliasRegex)) {
            // captured groups: name, quotation marks, and command
            name = match[1];
            std::string quotationMarkStr = match[2];
            quotationMarks = quotationMarkStr.at(0);
            command = match[3];
            return true;
        }
        return false;
    }
};

#endif // ALIAS_COMMAND_H_