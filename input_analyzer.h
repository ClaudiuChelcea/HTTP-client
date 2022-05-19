#ifndef __INPUT_ANALYZER_H
#define __INPUT_ANALYZER_H

#include <iostream>

// Return an id for a command
int get_command_id(std::string command);

// Execute the received command by id
void execute_command(const int command_id, int& exit_status);





#endif //  __INPUT_ANALYZER_H



