#ifndef __DATA_h
#define __DATA_h
#pragma once

#define HOST "34.241.4.235"
#define PROTOCOL_VERSION  "HTTP/1.1"
#define PORT 8080

namespace Constants
{
    const typedef enum {
        UNRECOGNIZED_COMMAND = -1,
        REGISTER_ACCOUNT = 0,
        AUTHENTICATE = 1,
        GET_LIBRARY_ACCESS = 2,
        VIEW_ALL_BOOKS = 3,
        VIEW_SPECIFIC_BOOK = 4,
        ADD_BOOK_TO_LIBRARY = 5,
        REMOVE_BOOK_FROM_LIBRARY = 6,
        LOGOUT = 7,
        EXIT = 8
    } ACTIONS;
}

#endif // !__DATA_h