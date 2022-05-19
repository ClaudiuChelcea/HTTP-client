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

#define DEBUG false

// Return an id for a command
int get_command_id(std::string command)
{
    if(strcmp(command.c_str(), "register") == 0) {
        return Constants::REGISTER_ACCOUNT;
    } else if(strcmp(command.c_str(), "login") == 0) {
        return Constants::AUTHENTICATE;
    } else if(strcmp(command.c_str(), "enter_library") == 0) {
        return Constants::GET_LIBRARY_ACCESS;
    } else if(strcmp(command.c_str(), "get_books") == 0) {
        return Constants::VIEW_ALL_BOOKS;
    } else if(strcmp(command.c_str(), "get_book") == 0) {
        return Constants::VIEW_SPECIFIC_BOOK;
    } else if(strcmp(command.c_str(), "add_book") == 0) {
        return Constants::ADD_BOOK_TO_LIBRARY;
    } else if(strcmp(command.c_str(), "delete_book") == 0) {
        return Constants::REMOVE_BOOK_FROM_LIBRARY;
    } else if(strcmp(command.c_str(), "logout") == 0) {
        return Constants::LOGOUT;
    } else if(strcmp(command.c_str(), "exit") == 0) {
        return Constants::EXIT;
    } else {
        return Constants::UNRECOGNIZED_COMMAND;
    }
}

// Execute the received command by id
void execute_command(const int command_id, int& exit_status)
{
    // Variables
    std::string access_route;
    std::string username;
    std::string password;
    std::string message;
    std::string content_type;
    std::string end = "\r\n";
    char* server_retriever;
    UI_printer my_printer = UI_printer::getInstance();
    int sockfd;

    switch(command_id) {
        case Constants::UNRECOGNIZED_COMMAND:
        {
            break;
        }
        case Constants::REGISTER_ACCOUNT:
        {
            // Open socket
            sockfd = open_connection(const_cast<char*>(Constants::HOST), Constants::PORT, AF_INET, SOCK_STREAM, 0);

            // Set parameters
            access_route = "POST /api/v1/tema/auth/register";
            content_type = "application/json";

            // Get input
            my_printer.get_input("Username:", username);
            my_printer.get_input("Password:", password);

            message = access_route + " " + Constants::PROTOCOL_VERSION + end;
            message.append( "Host: ");
            message.append(Constants::HOST);
            message.append(":");
            message.append(std::to_string(Constants::PORT));
            message.append(end);
            message.append("Content-Type: ");
            message.append(content_type);
            message.append(end);
            message.append("Content-Length: ");
            std::string data;
            data.append("{\n    \"username\": \"");
            data.append(username);
            data.append("\",\n");
            data.append("   \"password\":\"");
            data.append(password);
            data.append("\"\n}");
            data.append(end);

            int len = data.length();
            message.append(std::to_string(len));
            message.append(end);
            message.append(end);
            message.append(data);
           

            #if DEBUG == false
                // Send to server
                send_to_server(sockfd, const_cast<char*>(message.c_str()));

                // See what we receive from server
                server_retriever = strstr(receive_from_server(sockfd), "error");
                if (server_retriever != NULL) {
                    my_printer.put_message("The username is already taken");
                } else {
                    my_printer.put_message("Register with success");
                }

            #endif

            #if DEBUG == true
                my_printer.debug_printer(message);
            #endif

            // Close everything
            close(sockfd);
            break;
        }
        case Constants::GET_LIBRARY_ACCESS:
        {
            break;
        }
        case Constants::VIEW_ALL_BOOKS:
        {
            break;
        }
        case Constants::VIEW_SPECIFIC_BOOK:
        {
            break;
        }
        case Constants::ADD_BOOK_TO_LIBRARY:
        {
            break;
        }
        case Constants::REMOVE_BOOK_FROM_LIBRARY:
        {
            break;
        }
        case Constants::LOGOUT:
        {
            break;
        }
        case Constants::EXIT:
        {
            exit_status = true;
            break;
        }
        default:
        {
            break;
        }
    }
}