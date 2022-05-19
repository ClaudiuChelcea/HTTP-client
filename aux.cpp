#include <iostream>
#include "HEADERS/helpers.h"
#include <sys/socket.h> /* socket, connect */

// Return the message to be sent to the server
std::string get_request(int& sockfd, std::string access_route_given, std::string content_type_given) {
    // Open socket
    sockfd = open_connection(const_cast<char*>(HOST), PORT, AF_INET, SOCK_STREAM, 0);

    // Set parameters
    std::string access_route = access_route_given;
    std::string content_type = content_type_given;

    // Get input
    UI_printer my_printer = UI_printer::getInstance();
    std::string username;
    std::string password;
    std::string message;
    std::string end = "\r\n";
    my_printer.get_input("Username:", username);
    my_printer.get_input("Password:", password);

    message = access_route + " " + PROTOCOL_VERSION + end;
    message.append("Host: ");
    message.append(HOST);
    message.append(":");
    message.append(std::to_string(PORT));
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

    return message;
}