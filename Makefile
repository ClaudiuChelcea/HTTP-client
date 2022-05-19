# Chelcea Claudiu-Marian
# claudiu.chelcea@stud.acs.upb.ro
# Makefile

# ********************************************************
# ******************** COMPILER SETUP ******************** 
CC = g++
CFLAGS = -Wall -Wextra -g -std=c++17
SRC = client.cpp helpers.cpp buffer.cpp input_analyzer.cpp aux.cpp interface.cpp
HEADERS = helpers.h buffer.h interface.h input_analyzer.h aux.h data.h
NAME = client

# ********************************************************
# ******************** EXECUTE BUILD *********************
build:
	$(CC) -o $(NAME) $(CFLAGS) $^ $(SRC) -I "HEADERS/"

run:
	./$(NAME)
	
clean: 
	rm $(NAME)
	
.PHONY: clean build run

# ******************** END ******************************
# *******************************************************
