#ifndef __INPUT_ANALYZER_H
#define __INPUT_ANALYZER_H
#pragma once
#include <iostream>
#include "interface.h"

// Return an id for a command
int get_command_id(std::string command);

// Execute the received command by id
void execute_command(int& command_id, int& exit_status, bool& userConnected, char* cookie,
                     bool& jwt_Token_Access, char* jwt_Token, UI_printer my_printer);

#endif //  __INPUT_ANALYZER_H



