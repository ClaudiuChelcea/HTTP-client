#include <iostream>
#include "HEADERS/interface.h"
#include "HEADERS/input_analyzer.h"

#define LINELEN 1000
#define DEBUG false

int main(void)
{
    // Variables
    auto my_printer = UI_printer::getInstance();
    std::string command;
    int exit_status = false;
    bool userConnected = false;
    char* cookie = new char[LINELEN];
    bool jwt_Token_Access = false;
    char* jwt_Token = new char[LINELEN];

    // Display UI
    my_printer.initial_display();

    // Start receiving commands
    while(true) {
        // Check if we still want to continue
        if(exit_status == true) {
            break;
        }

        // Get input
        my_printer.get_input("Insert command: ", command);
        
        // Get command id
        int command_id = get_command_id(command);

        #if DEBUG == true
            my_printer.put_error_message("Received command: " + command);
            my_printer.put_error_message("Command id: " + std::to_string(command_id));
        #endif

        // Execute command
        execute_command(command_id, exit_status, userConnected, cookie, jwt_Token_Access, jwt_Token, my_printer);
    }
    
    return 0;
}