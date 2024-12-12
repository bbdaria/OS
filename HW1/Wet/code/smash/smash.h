#ifndef SMASH_H_
#define SMASH_H_

#include <iostream>
#include <set>
#include <map>
#include "code/commands/Commands.h"
#include "code/joblist/joblist.h"

class SmallShell {
private:
    SmallShell() =default;
    ~SmallShell()=default;

    static const std::string DEFAULT_START_PROMPT;

    JobsList::JobEntry* m_forground = nullptr;
    JobsList m_jobsList;
    std::string m_start_prompt = DEFAULT_START_PROMPT;
    char **m_plastPwd = nullptr;
    bool m_quitCalled = false;
    std::map<std::string, std::pair<std::string,char>> m_aliases;

public:
    SmallShell(SmallShell const &) = delete; // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator

    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Command* createCommand(const char *original_cmd_line, std::string& cmd_s);
    void executeCommand(const char *cmd_line);

    void setStartPrompt(std::string& s) {
        m_start_prompt = (s.size() ? s : DEFAULT_START_PROMPT);
    }

    std::string getStartPrompt() const {
        return m_start_prompt;
    }

    JobsList& getJobsList() {
        return m_jobsList;
    }

    char* plastPwd() {
        return *m_plastPwd;
    }

    void updatePlastPwd(char* cwd) {
        delete *m_plastPwd;
        *m_plastPwd = cwd;
    }

    void quit() {
        m_quitCalled = true;
    }

    JobsList::JobEntry* getForeground() {
        return m_forground;
    }

    void setForeGround(JobsList::JobEntry* foreground) {
        m_forground = foreground;
    }

    void launch() {
        while (!m_quitCalled) {
            std::cout << m_start_prompt << "> ";
            std::string cmd_line;
            std::getline(std::cin, cmd_line);
            executeCommand(cmd_line.c_str());
            setForeGround(nullptr);
        }
    }

    std::set<std::string> getReservedWords() {
        return {
            // built-in
            "alias",
            "unalias",
            "cd",
            "chprompt",
            "fg",
            "pwd",
            "jobs",
            "kill",
            "quit",
            "showpid",

            // special
            "listdir",
            "netinfo",
            "whoami",

            // external
            "sleep",
            "ls",
        };
    }

    std::map<std::string, std::pair<std::string,char>>& getAliases() {
        return m_aliases;
    }
};

const std::string SmallShell::DEFAULT_START_PROMPT = "smash";

#endif //SMASH_H_