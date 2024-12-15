#ifndef SMASH_H_
#define SMASH_H_

#include <iostream>
#include <cstring>
#include <set>
#include <vector>
#include "Commands.h"
#include "joblist.h"

typedef std::pair<std::string, char> AliasVal;
typedef std::pair<std::string, AliasVal> Alias;

class SmallShell {
private:
    SmallShell() =default;
    ~SmallShell()=default;

    const std::string DEFAULT_START_PROMPT = "smash";
    
    JobsList::JobEntry* m_forground = nullptr;
    JobsList m_jobsList;
    std::string m_start_prompt = DEFAULT_START_PROMPT;
    char *m_plastPwd = nullptr;
    bool m_quitCalled = false;

    std::vector<Alias> m_aliases;

    void applyAlias(std::string& cmd_s);

public:
    SmallShell(SmallShell const &) = delete; // disable copy ctor
    void operator=(SmallShell const &) = delete; // disable = operator

    static SmallShell &getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Command* createCommand(const char *original_cmd_line, std::string& cmd_s, bool background, bool aliasCommand);
    void executeCommand(const char *cmd_line);
    bool preProcessCmdLine(const char *original_cmd_line, std::string& out);

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
        return m_plastPwd;
    }

    void updatePlastPwd(char* cwd) {
        if (m_plastPwd) free(m_plastPwd);
        m_plastPwd = strdup(cwd);
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
            if (m_forground != nullptr) {
                delete m_forground;
            }
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
            "cat",
            "ls",
        };
    }

    std::vector<Alias>& getAliases() {
        return m_aliases;
    }
};

#endif //SMASH_H_