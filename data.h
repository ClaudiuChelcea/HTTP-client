#include <iostream>

namespace Constants
{
    static inline const char* HOST = "34.241.4.235";
    static inline const char* PROTOCOL_VERSION = "HTTP/1.1";
    static inline const int PORT = 8080;

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

