#ifndef UTIL_H_
#define UTIL_H_

#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>

const std::string WHITESPACE = " \n\r\t\f\v";

#if 0
#define FUNC_ENTRY()  \
	cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
	cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

std::string _ltrim(const std::string &s);

std::string _rtrim(const std::string &s);

std::string _trim(const std::string &s);

int _parseCommandLine(const char *cmd_line, char **args);

void _freeArgs(char **args, int words);

bool _isBackgroundComamnd(const char *cmd_line);

void _removeBackgroundSign(char *cmd_line);

bool _isDigitsOnly(const char* str);

std::pair<bool,std::pair<bool,std::string>> _redirectIO(std::string& cmd_s);

#endif // UTIL_H_