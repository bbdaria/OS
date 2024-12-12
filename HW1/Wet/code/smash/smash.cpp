#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "Smash.h"
#include "../util.cpp"
#include "../built_in/chprompt/chprompt_command.h"
#include "../built_in/show_pid/show_pid_command.h"
#include "../built_in/get_current_dir/get_current_dir_command.h"
#include "../built_in/change_dir/change_dir_command.h"
#include "../built_in/jobs/jobs_command.h"
#include "../built_in/foreground/foreground_command.h"
#include "../built_in/quit/quit_command.h"
#include "../built_in/kill/kill_command.h"
#include "../built_in/alias/alias_command.h"
#include "../built_in/alias/unalias_command.h"
#include "../built_in/special/listdir/listdir_command.h"
#include "../built_in/special/net_info/net_info_command.h"
#include "../built_in/special/whoami/who_am_i_command.h"

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command* SmallShell::createCommand(const char *original_cmd_line, std::string& cmd_s) {
	char** args;
	int words = _parseCommandLine(cmd_s.c_str(), args);
	Command* result = nullptr;
	if (words > 0) {
		result = _createBuiltInCommand(args, words);
		result = (result!=nullptr ? result : _createExternalCommand(original_cmd_line, cmd_s, args, words));
	}
	_freeArgs(args, words);
	return result;
}

BuiltInCommand* _createBuiltInCommand(char** args, int words, char* original_command) {
	std::string firstWord = args[0];
	_removeBackgroundSign(firstWord);

	if (firstWord.compare("chprompt") == 0) {
		return new Chprompt(args, words);
	}
	else if (firstWord.compare("showpid") == 0) {
		return new ShowPidCommand();
	}
	else if (firstWord.compare("pwd") == 0) {
		return new GetCurrDirCommand();
	}
	else if (firstWord.compare("cd") == 0) {
		return new ChangeDirCommand(args, words);
	}
	else if (firstWord.compare("jobs") == 0) {
		return new JobsCommand();
	}
	else if (firstWord.compare("fg") == 0) {
		return new ForegroundCommand(args, words);
	}
	else if (firstWord.compare("quit") == 0) {
		return new QuitCommand(args, words);
	}
	else if (firstWord.compare("kill") == 0) {
		return new KillCommand(args, words);
	}
	else if (firstWord.compare("alias") == 0) {
		return new AliasCommand(original_command);
	}
	else if (firstWord.compare("unalias") == 0) {
		return new UnaliasCommand(args, words);
	}

	else if (firstWord.compare("listdir") == 0) {
		return new ListDirCommand();
	}
	else if (firstWord.compare("netinfo") == 0) {
		return new NetInfoCommand();
	}
	else if (firstWord.compare("whoami") == 0) {
		return new WhoamiCommand();
	}
	return nullptr;
}

ExternalCommand* _createExternalCommand(char *original_cmd_line, std::string& cmd_s, char** args, int words) {
	bool background = _isBackgroundComamnd(cmd_s);
	if (ExternalCommand::isComplexCommand(cmd_s)) {
		return new ComplexExternalCommand(cmd_s);
	} else {
		return new SimpleExternalCommand(cmd_s);
	}
	return nullptr;
}

void _redirectIO(Command* cmd, const char *cmd_line) {
	// check whether > or >> is in cmd_line and apply cmd->setRedirectionFile accordingly.
	std::string commandStr(cmd_line);
    size_t redirectionPos = commandStr.find_first_of('>');
    if (redirectionPos != std::string::npos) {
        bool append = (redirectionPos + 1 < commandStr.size() && commandStr[redirectionPos + 1] == '>');
        size_t fileStart = redirectionPos + (append ? 2 : 1);
		std::string secHalf = commandStr.substr(fileStart, commandStr.size()-fileStart);
        std::string file = _trim(secHalf);

        cmd->setRedirectionFile(file, append);
    }
}

std::string applyAlias(const std::string& cmd_line) {
    std::string cmd_s = cmd_line;
    std::map<std::string, std::string>& alias = getAliases();
    for (const auto& [key, value] : alias) {
        size_t pos = cmd_s.find(key);
        while (pos != std::string::npos) {
            cmd_s.replace(pos, key.length(), value);  // Replace alias with its value
            pos = cmd_s.find(key, pos + value.length());
        }
    }
    return cmd_s;
}

void SmallShell::executeCommand(const char *original_cmd_line) {
	std::string appliedAliasCmd = applyAlias();
	std::string cmd_s =  _trim(appliedAliasCmd);

    Command* cmd = createCommand(original_cmd_line, cmd_s);
	m_jobsList.removeFinishedJobs(); // removing finished jobs before executing cmd
	if (cmd != nullptr) {
		_redirectIO(cmd, cmd_s);
        cmd->execute();
        delete cmd; // Prevent memory leaks
    }
}