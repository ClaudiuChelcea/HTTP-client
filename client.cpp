#include <iostream>     /* IO */
#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "data.h"
#include "interface.h"
#include "input_analyzer.h"

int main(void)
{
    // Variables
    auto my_printer = UI_printer::getInstance();
    std::string command;
    int exit_status = false;

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

        // Execute command
        execute_command(command_id, exit_status);
    }
    
    return 0;
}