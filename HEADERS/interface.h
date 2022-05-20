#ifndef __interface_H
#define __interface_H
#pragma once
#include <iostream>

#define repeat(fct,cnt)                            \
for(int i = 0; i < cnt; ++i) {                     \
    fct();                                         \
}

class UI_printer {
    private:
        UI_printer() = default; // Make constructor private to not instantiate
        void put_blank_line();
        void put_message_in_bulletlist(std::string message);
    public:
        static UI_printer& getInstance()
        {
            static UI_printer instance; // Created only once
            return instance;
        }
        void initial_display();
        void put_message(std::string message);
        void put_error_message(std::string message);
        void get_input(std::string message, std::string& out_message);
        void get_input(std::string message, int& id);
        void debug_printer(std::string message);
};

#endif // !__interface_H
