#ifndef __AUX_H
#define __AUX_H
#pragma once

#include <iostream>

// Return the message to be sent to the server
std::string get_request(int& sockfd, std::string access_route_given, std::string content_type_given);

#endif // __AUX_H