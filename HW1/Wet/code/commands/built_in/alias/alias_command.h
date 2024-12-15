#ifndef ALIAS_COMMAND_H_
#define ALIAS_COMMAND_H_

#include <iostream>
#include <regex>
#include <string>
#include "built_in_command.h"
#include "smash.h"

class AliasCommand : public BuiltInCommand {
public:
    AliasCommand(std::string& cmd_line) : m_cmd_line(cmd_line) {}

    void execute() override {
        std::string cmd_s(m_cmd_line);
        if (_trim(cmd_s).compare("alias") == 0) {
            // print alias list
            SmallShell& smash = SmallShell::getInstance();
            auto& aliases = smash.getAliases();
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
            if (isReservedCommand(name) || existsAsAlias(name)) {
                std::cerr << "smash error: alias: " << name << " already exists or is a reserved command";
                std::cerr << std::endl;
                return;
            }

            AliasVal val = std::pair<std::string, char>(command, quotationMarks);
            Alias alias = std::pair<std::string, AliasVal>(name, val);

            // Add alias
            auto& aliases = SmallShell::getInstance().getAliases();
            aliases.emplace_back(alias);
        }
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

private:
    std::string m_cmd_line;

    bool existsAsAlias(const std::string &newName) {
        auto& aliases = SmallShell::getInstance().getAliases();
        for (auto it = aliases.begin(); it != aliases.end(); ++it) {
            auto alias = *it;
            const std::string& name = alias.first;
            if (newName.compare(name) == 0) {
                return true;
            }
        }
        return false;
    }

    bool isReservedCommand(const std::string &name) {
        // List of reserved commands
        auto reserved = SmallShell::getInstance().getReservedWords();
        return reserved.find(name) != reserved.end();
    }
};

#endif // ALIAS_COMMAND_H_