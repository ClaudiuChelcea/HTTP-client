#include "HEADERS/interface.h"

// Implement functions for interface
void UI_printer::put_message_in_bulletlist(std::string message)
{
    std::cout << "|      * " << message;
    int line_len = 9 + message.length();
    int remaining_len = 120 - line_len;
    for(int i = 0; i < remaining_len - 1; ++i)
        std::cout << " ";
    std::cout << "|\n";
}

void UI_printer::initial_display()
{
    std::cout << " ______________________________________________________________________________________________________________________\n";
    std::cout << "|                                                     HELLO USER!                                                      |\n";
    repeat(put_blank_line, 3);
    put_message("If the received command is not in the commands bellow, weird things might happen, so be careful!");
    put_message("To interact with this server, use one of the commands below:");
    put_message_in_bulletlist("register");
    put_message_in_bulletlist("login");
    put_message_in_bulletlist("enter_library");
    put_message_in_bulletlist("get_books");
    put_message_in_bulletlist("get_book");
    put_message_in_bulletlist("add_book");
    put_message_in_bulletlist("delete_book");
    put_message_in_bulletlist("logout");
    put_message_in_bulletlist("exit");
    repeat(put_blank_line, 2);
}

void UI_printer::put_message(std::string message)
{
    std::cout << "|  " << message;
    int line_len = 3 + message.length();
    int remaining_len = 120 - line_len;
    for(int i = 0; i < remaining_len - 1; ++i)
        std::cout << " ";
    std::cout << "|\n";
}

void UI_printer::get_input(std::string message, std::string& out_message) {
    std::cout << "|  " << message;
    std::getline(std::cin, out_message);
}

void UI_printer::debug_printer(std::string message) {
    put_blank_line();
    std::cout << "|                                                       DEBUGGER                                                       |\n";
    std::cout << message;
    repeat(put_blank_line, 2);
}

void UI_printer::put_error_message(std::string message) {
    std::cout << "|                                                        ERROR                                                         |\n";
    std::cout << "|  " << message;
    int line_len = 3 + message.length();
    int remaining_len = 120 - line_len;
    for(int i = 0; i < remaining_len - 1; ++i)
        std::cout << " ";
    std::cout << "|\n";
}

void UI_printer::put_blank_line() {
    std::cout << "|                                                                                                                      |\n";
}