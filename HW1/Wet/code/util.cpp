#include "util.h"

std::string _ltrim(const std::string &s) {
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string _rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string _trim(const std::string &s)
{
    // remove maximum whitespace suffix & prefix of s
	return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char *cmd_line, char **args)
{
    FUNC_ENTRY()
	int i = 0;
	std::istringstream iss(_trim(std::string(cmd_line)).c_str());
	for (std::string s; iss >> s;) {
		args[i] = (char *) malloc(s.length() + 1);
		memset(args[i], 0, s.length() + 1);
		strcpy(args[i], s.c_str());
		args[++i] = NULL;
	}
	return i;

	FUNC_EXIT()
}

void _freeArgs(char **args, int words)
{
    for (int i = 0; i < words; ++i) {
		free(args[i]);
	}
	free(args);
}

bool _isBackgroundComamnd(const char *cmd_line)
{
    const std::string str(cmd_line);
	return str[str.find_last_not_of(WHITESPACE)] == '&';
}

void _removeBackgroundSign(char *cmd_line)
{
    const std::string str(cmd_line);
	// find last character other than spaces
	unsigned int idx = str.find_last_not_of(WHITESPACE);
	// if all characters are spaces then return
	if (idx == std::string::npos) {
		return;
	}
    // if the command line does not end with & then return
	if (cmd_line[idx] != '&') {
		return;
	}
	// replace the & (background sign) with space and then remove all tailing spaces.
	cmd_line[idx] = ' ';
	// truncate the command line string up to the last non-space character
	cmd_line[str.find_last_not_of(WHITESPACE, idx) + 1] = '\0';
}

bool _isDigitsOnly(const char *str)
{
    for (const char* c = str; *c != '\0'; ++c) {
        if (!std::isdigit(*c)) {
            return false;
        }
    }
return true;
}

std::pair<bool,std::pair<bool,std::string>> _redirectIO(std::string& cmd_s, bool aliasCmd) {
	if (aliasCmd) return {false, {false, ""}};
    size_t pos = cmd_s.find('>');
	bool redirectIO = pos != std::string::npos;
	bool append = false;
    if (redirectIO) {
        // Check if it's a >> redirection
        if (pos + 1 < cmd_s.length() && cmd_s[pos + 1] == '>') {
            // operand >>
			append = true;
        }
    }
	std::pair<bool,std::pair<bool,std::string>> result = std::pair<bool,std::pair<bool,std::string>>();
	result.first = redirectIO;
	if (redirectIO) {
		result.second.first = append;
		result.second.second = _trim(cmd_s.substr(pos+(append ? 2 : 1), cmd_s.size()-(pos+(append ? 2 : 1))));

		cmd_s = cmd_s.substr(0, pos);
	}
    return result;
}