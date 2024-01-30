# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tkajanek <tkajanek@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/08 11:05:29 by sbenes            #+#    #+#              #
#    Updated: 2024/01/27 16:53:58 by sbenes           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#color and formatting definitions:
BOLD =	\033[1m
GREEN =	\033[32m
RED =	\033[31m
MAGEN =	\033[35m
YELLOW =\033[33m
CYAN =	\033[36m
BLUE =	\033[34m
WHITE =	\033[37m
NC =	\033[0m

#name
NAME = 		webserv

# compiler
CC =		c++
CFLAGS =	-Wall -Wextra -Werror -std=c++98 -g

#Includes
INC =	-I ./include/

#sources
SRC_PATH =	src/

SRC = 	webserv.cpp utils.cpp Parser.cpp Server.cpp ServerManager.cpp \
		Client.cpp HttpRequest.cpp Response.cpp Location.cpp Mime.cpp Error.cpp\
		Interface.cpp Log.cpp Cgi.cpp
	
SRCS =	$(addprefix $(SRC_PATH), $(SRC))

#objects
OBJ_PATH =	obj/
OBJ =		$(SRC:.cpp=.o)
OBJS =		$(addprefix $(OBJ_PATH), $(OBJ))


all: $(NAME)

$(OBJ_PATH)%.o :$(SRC_PATH)%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(OBJS): $(OBJ_PATH)

$(OBJ_PATH):
	@mkdir $(OBJ_PATH)

$(NAME): $(OBJS)
	@echo "$(BOLD)$(GREEN)[ 🙊 Compiling ... ]$(NC)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(INC)
	@if [ ! -d "logs" ]; then \
		echo "$(BOLD)$(GREEN)[ Creating logs directory ]$(NC)"; \
		mkdir logs; \
	fi
	@echo "$(BOLD)$(GREEN)[ 🪄 Program ready! ]$(NC)"
	@echo "$(BOLD)$(GREEN)                      "
	@echo "    _____________________                    " 
	@echo "   / _/ ___/_  __/ __/  /__ _____  _____ ____ "
	@echo "  / // |_ / / / _\ \ / / -_| __/ |/ / -_| __/ " 
	@echo " / / \___/ /_/ /___// /\__/_/  |___/\__/_/    " 
	@echo "/__/              /__/                        " 
	@echo "									    			"
	@echo " run ./webserv <config_file> <flag1> <flag2>          "
	@echo "									    		$(NC)"
	@echo "$(BOLD)$(GREEN) optional flags: "
	@echo "  "
	@echo "	--if :		interface mode"
	@echo "	--debug :	debug logs"
	@echo "	 "
	@echo "                             				   $(NC)"

bonus: all

clean:
	@echo "$(BOLD)$(RED)[ 🔥 Purging object files ]$(NC)"
	@rm -Rf $(OBJ_PATH)
	@echo "$(BOLD)$(RED)[ 🔥 Removing logs directory ]$(NC)"
	@rm -Rf logs

fclean: clean
	@echo "$(BOLD)$(RED)[ 🔥 Removing program ]$(NC)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean
