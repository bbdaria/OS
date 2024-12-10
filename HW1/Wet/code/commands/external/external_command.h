#ifndef EXTERNAL_COMMAND_H_
#define EXTERNAL_COMMAND_H_

#include "Commands.h"
#include <iostream> // std::cout and std::cerr

class ExternalCommand : public Command {
public:
    ExternalCommand(const char *cmd_line) : Command(), m_cmd_line(cmd_line) {}
    ~ExternalCommand()=default;
private:
    std::string m_cmd_line;
};

#endif // EXTERNAL_COMMAND_H_