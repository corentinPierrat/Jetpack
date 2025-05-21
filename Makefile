##
## EPITECH PROJECT, 2023
## makefile
## File description:
## Makefile
##

SRC_SERVER = $(wildcard Server/*.cpp)
OBJ_SERVER = $(SRC_SERVER:.cpp=.o)
NAME_SERVER = jetpack_server

SRC_CLIENT = $(wildcard Client/*.cpp)
OBJ_CLIENT = $(SRC_CLIENT:.cpp=.o)
NAME_CLIENT = jetpack_client

CFLAGS = -Wall -Wextra -g

SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: $(NAME_SERVER) $(NAME_CLIENT)

$(NAME_SERVER): $(OBJ_SERVER)
	g++ $(CFLAGS) -o $(NAME_SERVER) $(OBJ_SERVER) $(SFML_FLAGS)

$(NAME_CLIENT): $(OBJ_CLIENT)
	g++ $(CFLAGS) -o $(NAME_CLIENT) $(OBJ_CLIENT) $(SFML_FLAGS)

clean_server:
	rm -f $(OBJ_SERVER)

clean_client:
	rm -f $(OBJ_CLIENT)

clean: clean_server clean_client

fclean: clean
	rm -f $(NAME_SERVER) $(NAME_CLIENT)

re: fclean all
