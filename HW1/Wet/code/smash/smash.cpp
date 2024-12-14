#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "smash/smash.h"

#include "util.h"
#include "commands/built_in/chprompt/chprompt_command.h"
#include "commands/built_in/show_pid/show_pid_command.h"
#include "commands/built_in/get_current_dir/get_current_dir_command.h"
#include "commands/built_in/change_dir/change_dir_command.h"
#include "commands/built_in/jobs/jobs_command.h"
#include "commands/built_in/foreground/foreground_command.h"
#include "commands/built_in/quit/quit_command.h"
#include "commands/built_in/kill/kill_command.h"
#include "commands/built_in/alias/alias_command.h"
#include "commands/built_in/alias/unalias_command.h"
#include "commands/built_in/special/listdir/listdir_command.h"
#include "commands/built_in/special/net_info/net_info_command.h"
#include "commands/built_in/special/whoami/who_am_i_command.h"

#include "commands/external/external_command.h"
#include "commands/external/simple/simple_command.h"
#include "commands/external/complex/complex_command.h"
#include "commands/external/special/pipe/pipe_command.h"
#include "smash.h"

BuiltInCommand* _createBuiltInCommand(char** args, int words, std::string cmd_s) {
	std::string firstWord = _trim(args[0]);

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
		return new AliasCommand(cmd_s);
	}
	else if (firstWord.compare("unalias") == 0) {
		return new UnaliasCommand(args, words);
	}
	else if (firstWord.compare("listdir") == 0) {
		return new ListDirCommand(args, words);
	}
	else if (firstWord.compare("netinfo") == 0) {
		return new NetinfoCommand(args, words);
	}
	else if (firstWord.compare("whoami") == 0) {
		return new WhoamiCommand();
	}
	return nullptr;
}

ExternalCommand* _createExternalCommand(const char *original_cmd_line, std::string& cmd_s, bool background) {
	if (ExternalCommand::isComplexCommand(cmd_s)) {
		return new ComplexExternalCommand(original_cmd_line, cmd_s, background);
	} else {
		return new SimpleExternalCommand(original_cmd_line, cmd_s, background);
	}
	return nullptr;
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command* SmallShell::createCommand(const char *original_cmd_line, std::string& cmd_s, bool background) {
    char** args = (char**) malloc(sizeof(char*) * 100);  // Assuming a maximum of 20 arguments
	int words = _parseCommandLine(cmd_s.c_str(), args);
	Command* result = nullptr;
	if (words > 0) {
		if (cmd_s.find('|') != std::string::npos || cmd_s.find("|&") != std::string::npos) {
			// If it contains a pipe, create a PipeCommand
			result = new PipeCommand(original_cmd_line, cmd_s, background);
		}
		result = (result!=nullptr ? result : _createBuiltInCommand(args, words, cmd_s));
		result = (result!=nullptr ? result : _createExternalCommand(original_cmd_line, cmd_s, background));
	}
	_freeArgs(args, words);
	return result;
}

void SmallShell::applyAlias(std::string& cmd_s) {
	size_t pos = cmd_s.find_first_of(WHITESPACE);
	if (pos == std::string::npos) pos = cmd_s.length();
	std::string firstWord = cmd_s.substr(0, pos);

    std::map<std::string, AliasVal>& aliases = getAliases();
    for (auto it = aliases.begin(); it != aliases.end(); ++it) {
		auto alias = *it;
        const std::string& name = alias.first;
        const std::pair<std::string, char>& value = alias.second;
		std::string str = value.first;

        if (firstWord.compare(name) == 0) {
			cmd_s = _trim(cmd_s.replace(0, pos, str));
			return;
		}
    }
}

void SmallShell::executeCommand(const char *original_cmd_line) {
	std::string cmd_s;
	bool background = preProcessCmdLine(original_cmd_line, cmd_s);

	// I/O redirection: (cuts cmd_s)
	auto redirectResultIO = _redirectIO(cmd_s);
	
    Command* cmd = createCommand(original_cmd_line, cmd_s, background);
	if (cmd != nullptr) {
		cmd->redirectIO(redirectResultIO);
		m_jobsList.removeFinishedJobs(); // removing finished jobs before executing cmd
        cmd->execute();
        delete cmd; // Prevent memory leaks
    }
}

bool SmallShell::preProcessCmdLine(const char *original_cmd_line, std::string& out) {
	std::string originalCmd = strdup(original_cmd_line);
	out = _trim(originalCmd);
	if (out.length() == 0) return false;
	bool background = out.at(out.length()-1) == '&';
	if (background) {
		out = out.substr(0, out.size()-1);
		out = _trim(out);
	}
	applyAlias(out);
	return background;
}