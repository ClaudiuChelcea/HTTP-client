#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h> /* socket, connect */
#include "HEADERS/helpers.h"
#define DEBUG_REGISTER false
#define DEBUG_LOGIN false
#define DEBUG_ENTER_LIBRARY false
#define DEBUG_VIEW_ALL false
#define DEBUG_ADD_BOOK false

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
void execute_command(int& command_id, int& exit_status, bool& userConnected, char* cookie,
                     bool& jwt_Token_Access, char* jwt_Token, UI_printer my_printer)
{
    // Variables
    std::string message;
    char* server_retriever = NULL;
    int sockfd;
    std::string end = "\r\n";
    std::string error_message;
    std::string server_answer;
    char* session_cookie = NULL, *last_session_cookie = NULL, *last_session_token = NULL, *session_token = NULL;
    char* parse_output = NULL;
    int id = 0, page_count = 0;
    std::string title, author, publisher, genre;
    std::string data;

    switch(command_id) {
        case Constants::UNRECOGNIZED_COMMAND:
            error_message = "Case not found! Check your input! Command id: " + std::to_string(command_id) + " not recognized!";
            my_printer.put_error_message(error_message);
            my_printer.put_message("");
            break;
        case Constants::REGISTER_ACCOUNT:
            message = get_request(sockfd, "POST /api/v1/tema/auth/register", "application/json");

            #if DEBUG == false
                // Send to server
                send_to_server(sockfd, const_cast<char*>(message.c_str()));

                // See what we receive from server
                server_retriever = strstr(receive_from_server(sockfd), "error");
                if (server_retriever != NULL) {
                    my_printer.put_message("The account is already registered!");
                } else {
                    my_printer.put_message("Account registered with success!");
                }
                my_printer.put_message("");

            #endif

            #if DEBUG_REGISTER == true
                my_printer.debug_printer(message);
            #endif

            // Close everything
            close(sockfd);
            break;
        case Constants::AUTHENTICATE:
            message = get_request(sockfd, "POST /api/v1/tema/auth/login", "application/json");

            #if DEBUG == false
                // Send to server
                send_to_server(sockfd, const_cast<char*>(message.c_str()));

                // See what we receive from server
                server_answer = receive_from_server(sockfd);
                server_retriever = strstr(const_cast<char*>(server_answer.c_str()), "error");
                if (server_retriever != NULL) {
                    my_printer.put_message("Username or password is incorrect!");
                    my_printer.put_message("");
                } else { // Get cookies
                    session_cookie = strstr(const_cast<char*>(server_answer.c_str()), "connect.sid");
                    if(session_cookie == NULL) {
                        goto end;
                    } else {
                        last_session_cookie = strstr(const_cast<char*>(server_answer.c_str()), "Date:");
                        *(last_session_cookie - 20) = '\0';
                        strncpy(cookie, session_cookie, 200);
                        userConnected = true;
                        my_printer.put_message("Successfull login!");
                        my_printer.put_message("");
                        #if DEBUG_LOGIN == true
                            my_printer.debug_printer(server_answer);
                            my_printer.debug_printer(cookie);
                        #endif    
                    }
                }

            #endif

            end:

            // Close everything
            close(sockfd);
            break;
        case Constants::GET_LIBRARY_ACCESS:
            if (userConnected == false) {
                my_printer.put_error_message("You are not connected!");
                my_printer.put_error_message("");
                goto exit;
            }

            // Open port
            message.clear();
            sockfd = open_connection(const_cast<char*>(HOST), PORT, AF_INET, SOCK_STREAM, 0);

            // Create message
            message.append(const_cast<char*>("GET /api/v1/tema/library/access HTTP/1.1"));
            message.append(end);
            message.append("Host: ");
            message.append(HOST);
            message.append(":");
            message.append(std::to_string(PORT));
            message.append(end);
            message.append("Cookie: ");
            message.append(cookie);
            message.append(end);
            message.append(end);

            // Send message
            send_to_server(sockfd, const_cast<char*>(message.c_str()));

            // See what we receive from server
            server_answer = receive_from_server(sockfd);
           
            server_retriever = strstr(const_cast<char*>(server_answer.c_str()), "error");

            #if DEBUG_ENTER_LIBRARY == true
                my_printer.debug_printer(message);
                my_printer.debug_printer(server_answer);
            #endif

            if(server_retriever != NULL) {
                my_printer.put_error_message("Error getting access to library!");
            } else {
                last_session_token = strstr(const_cast<char*>(server_answer.c_str()), "\"}");
                *(last_session_token) = '\0';
                session_token = strstr(const_cast<char*>(server_answer.c_str()), "{\"token\":") + 10;
                strncpy(jwt_Token, session_token, 300);
                jwt_Token_Access = true;
                my_printer.put_message("Entered library successfully!");
            }
            my_printer.put_message("");

            // Close everything
            exit:
            close(sockfd);
            break;
        case Constants::VIEW_ALL_BOOKS:
            if ((userConnected & jwt_Token_Access) != true) {
                my_printer.put_message("You are not connected to the library / account!");
                my_printer.put_message("");
                goto leave;
            }

            // Open port
            message.clear();
            sockfd = open_connection(const_cast<char*>(HOST), PORT, AF_INET, SOCK_STREAM, 0);

            // Create message
            message.append(const_cast<char*>("GET /api/v1/tema/library/books HTTP/1.1"));
            message.append(end);
            message.append("Host: ");
            message.append(HOST);
            message.append(":");
            message.append(std::to_string(PORT));
            message.append(end);
            message.append("Authorization: Bearer ");
            message.append(jwt_Token);
            message.append(end);
            message.append("Cookie: ");
            message.append(cookie);
            message.append(end);
            message.append(end);

            // Send message
            send_to_server(sockfd, const_cast<char*>(message.c_str()));

            // See what we receive from server
            server_answer = receive_from_server(sockfd);
           
            server_retriever = strstr(const_cast<char*>(server_answer.c_str()), "error");

            #if DEBUG_VIEW_ALL == true
                my_printer.debug_printer(message);
                my_printer.debug_printer(server_answer);
            #endif

            if(server_retriever != NULL) {
                my_printer.put_error_message("Error getting access to viewing all books!");
            } else {
                // Print books in a JSON way
                parse_output = strstr(const_cast<char*>(server_answer.c_str()), "[{\"");
                if(!parse_output) {
                    my_printer.put_error_message("No books!");
                    my_printer.put_message("");
                    goto leave;
                }
                for(int i = 0; i < (int) strlen(parse_output); ++i) {
                    if(!parse_output[i]) {
                        my_printer.put_error_message("No books!");
                        my_printer.put_message("");
                        break;
                    }
                    if(parse_output[i] == '{') {
                        std::cout << '\t' << parse_output[i] << std::endl << '\t';
                    } else if(parse_output[i] == '}')  {
                        std::cout << std::endl << '\t' << parse_output[i];
                    } else if(parse_output[i] == '[') {
                        std::cout << parse_output[i] << std::endl;
                    } else if(parse_output[i] == ']') {
                        std::cout << std::endl << parse_output[i];
                    } else if(parse_output[i] == ',') {
                        if(parse_output[i+1] == '{') {
                            std::cout << parse_output[i] << std::endl;
                        } else {
                            std::cout << parse_output[i] << std::endl << '\t';
                        }
                    } else if(parse_output[i] == '"' && ((parse_output[i+1] == 'i' && parse_output[i+2] == 'd') || (parse_output[i+1] == 't' && parse_output[i+2] == 'i'))) {
                        std::cout << '\t' << parse_output[i];
                    } else {
                        std::cout << parse_output[i];
                    }
                }
            }
            my_printer.put_message("");

            // Close everything
            leave:
            close(sockfd);
            break;
        case Constants::VIEW_SPECIFIC_BOOK:
            if ((userConnected & jwt_Token_Access) != true) {
                my_printer.put_message("You are not connected to the library / account!");
                my_printer.put_message("");
                goto leave;
            }

            my_printer.get_input("Id:", id);

            // Open port
            message.clear();
            sockfd = open_connection(const_cast<char*>(HOST), PORT, AF_INET, SOCK_STREAM, 0);

            // Create message
            message.append(const_cast<char*>("GET /api/v1/tema/library/books/"));
            message.append(std::to_string(id));
            message.append(" HTTP/1.1");
            message.append(end);
            message.append("Host: ");
            message.append(HOST);
            message.append(":");
            message.append(std::to_string(PORT));
            message.append(end);
            message.append("Authorization: Bearer ");
            message.append(jwt_Token);
            message.append(end);
            message.append("Cookie: ");
            message.append(cookie);
            message.append(end);
            message.append(end);

            // Send message
            send_to_server(sockfd, const_cast<char*>(message.c_str()));

            // See what we receive from server
            server_answer = receive_from_server(sockfd);
           
            server_retriever = strstr(const_cast<char*>(server_answer.c_str()), "error");

            if(server_retriever != NULL) {
                my_printer.put_error_message("A book with this ID doesn't exist!");
                my_printer.put_message("");
            } else {
                // Print books in a JSON way
                parse_output = strstr(const_cast<char*>(server_answer.c_str()), "[{\"");
                for(int i = 0; i < (int) strlen(parse_output); ++i) {
                    if(parse_output[i] == '{') {
                        std::cout << parse_output[i] << std::endl;
                    } else if(parse_output[i] == '}')  {
                        std::cout << std::endl << parse_output[i];
                    } else if(parse_output[i] == '[') {
                        continue;
                    } else if(parse_output[i] == ']') {
                        continue;
                    } else if(parse_output[i] == ',') {
                        if(parse_output[i+1] == '{') {
                            std::cout << parse_output[i] << std::endl;
                        } else {
                            std::cout << parse_output[i] << std::endl;
                        }
                    } else if(parse_output[i] == '"' && ((parse_output[i+1] == 'i' && parse_output[i+2] == 'd') ||
                                                         (parse_output[i+1] == 't' && parse_output[i+2] == 'i') ||
                                                         (parse_output[i+1] == 'a' && parse_output[i+2] == 'u') ||
                                                         (parse_output[i+1] == 'p' && parse_output[i+2] == 'u') ||
                                                         (parse_output[i+1] == 'g' && parse_output[i+2] == 'e') ||
                                                         (parse_output[i+1] == 'p' && parse_output[i+2] == 'a'))) {
                        std::cout << '\t' << parse_output[i];
                    } else {
                        std::cout << parse_output[i];
                    }
                }
                my_printer.put_message("");
            }

            // Close everything
            close(sockfd);
            break;
        case Constants::ADD_BOOK_TO_LIBRARY:
            if ((userConnected & jwt_Token_Access) != true) {
                my_printer.put_message("You are not connected to the library / account!");
                my_printer.put_message("");
                break;
            }

            my_printer.get_input("Title:", title);
            my_printer.get_input("Author:", author);
            my_printer.get_input("Genre:", genre);
            my_printer.get_input("Publisher:", publisher);
            my_printer.get_input("Page count:", page_count);

            // Create message
            data.clear();
            message.clear();
            data.append("{\n\t\"title\": \"");
            data.append(title);
            data.append("\",\n");
            data.append("\t\"author\":\"");
            data.append(author);
            data.append("\",\n\t\"genre\":\"");
            data.append(genre);
            data.append("\",\n\t\"publisher\":\"");
            data.append(publisher);
            data.append("\",\n\t\"page_count\":\"");
            data.append(std::to_string(page_count));
            data.append("\"\n}");

            message.append(const_cast<char*>("POST /api/v1/tema/library/books HTTP/1.1"));
            message.append(end);
            message.append("Host: ");
            message.append(HOST);
            message.append(":");
            message.append(std::to_string(PORT));
            message.append(end);
            message.append("Content-Type: application/json");
            message.append(end);
            message.append("Authorization: Bearer ");
            message.append(jwt_Token);
            message.append(end);
            message.append("Cookie: ");
            message.append(cookie);
            message.append(end);
            message.append("Content-Length: ");
            message.append(std::to_string(data.length()));
            message.append(end);
            message.append(end);
            message.append(data);
            message.append(end);

            // Open port
            sockfd = open_connection(const_cast<char*>(HOST), PORT, AF_INET, SOCK_STREAM, 0);

            #if DEBUG_ADD_BOOK == true
                my_printer.debug_printer(message);
            #endif

            // Send message
            send_to_server(sockfd, const_cast<char*>(message.c_str()));

            // See what we receive from server
            server_answer = receive_from_server(sockfd);
           
            server_retriever = strstr(const_cast<char*>(server_answer.c_str()), "error");

            if(server_retriever != NULL) {
                my_printer.put_message("Wrong data inserted!");
                my_printer.put_message("");
            } else {
                my_printer.put_message("Book added successfully!");
                my_printer.put_message("");
            }

            // Close everything
            close(sockfd);
            break;
        case Constants::REMOVE_BOOK_FROM_LIBRARY:
            if ((userConnected & jwt_Token_Access) != true) {
                my_printer.put_message("You are not connected to the library / account!");
                my_printer.put_message("");
                break;
            }

            my_printer.get_input("Id:", id);

            // Open port
            message.clear();
            sockfd = open_connection(const_cast<char*>(HOST), PORT, AF_INET, SOCK_STREAM, 0);

            // Create message
            message.append(const_cast<char*>("DELETE /api/v1/tema/library/books/"));
            message.append(std::to_string(id));
            message.append(" HTTP/1.1");
            message.append(end);
            message.append("Host: ");
            message.append(HOST);
            message.append(":");
            message.append(std::to_string(PORT));
            message.append(end);
            message.append("Authorization: Bearer ");
            message.append(jwt_Token);
            message.append(end);
            message.append("Cookie: ");
            message.append(cookie);
            message.append(end);
            message.append(end);

            // Send message
            send_to_server(sockfd, const_cast<char*>(message.c_str()));

            // See what we receive from server
            server_answer = receive_from_server(sockfd);
           
            server_retriever = strstr(const_cast<char*>(server_answer.c_str()), "error");

            if(server_retriever != NULL) {
                my_printer.put_message("A book with this ID doesn't exist!");
                my_printer.put_message("");
            } else {
                my_printer.put_message("The book with the specified ID was deleted!");
                my_printer.put_message("");
            }

            // Close everything
            close(sockfd);
            break;
        case Constants::LOGOUT:
            if (userConnected != true) {
                my_printer.put_message("You are not connected to the library / account!");
                my_printer.put_message("");
                break;
            }

            // Open port
            message.clear();
            sockfd = open_connection(const_cast<char*>(HOST), PORT, AF_INET, SOCK_STREAM, 0);

            // Create message
            message.append(const_cast<char*>("GET /api/v1/tema/auth/logout"));
            message.append(" HTTP/1.1");
            message.append(end);
            message.append("Host: ");
            message.append(HOST);
            message.append(":");
            message.append(std::to_string(PORT));
            message.append(end);
            message.append("Authorization: Bearer ");
            message.append(jwt_Token);
            message.append(end);
            message.append("Cookie: ");
            message.append(cookie);
            message.append(end);
            message.append(end);

            // Send message
            send_to_server(sockfd, const_cast<char*>(message.c_str()));

            // See what we receive from server
            server_answer = receive_from_server(sockfd);
           
            server_retriever = strstr(const_cast<char*>(server_answer.c_str()), "error");

            if(server_retriever != NULL) {
                my_printer.put_message("Error logging out!");
                my_printer.put_message("");
            } else {
                my_printer.put_message("Successfull logout! See you soon!");
                my_printer.put_message("");
                jwt_Token_Access = false;
                userConnected = false;
            }

            // Close everything
            close(sockfd);
            break;
        case Constants::EXIT:
            exit_status = true;
            jwt_Token_Access = false;
            userConnected = false;
            break;
        default:
            error_message = "Case not found! Check your input! Command id: " + std::to_string(command_id) + "not recognized!";
            my_printer.put_error_message(error_message);
            my_printer.put_message("");
            my_printer.put_message("");
            break;
    }
}