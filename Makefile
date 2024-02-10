# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbenes <sbenes@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/08 11:05:29 by sbenes            #+#    #+#              #
#    Updated: 2024/02/10 13:52:07 by sbenes           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Color and formatting definitions:
BOLD =  \033[1m
GREEN = \033[32m
RED =   \033[31m
MAGEN = \033[35m
YELLOW =\033[33m
CYAN =  \033[36m
BLUE =  \033[34m
WHITE = \033[37m
NC =    \033[0m

# Name of the executable:
NAME =      webserv

# Compiler and flags:
CC =        c++
CFLAGS =    -Wall -Wextra -Werror -std=c++98 -g
DEPFLAGS =  -MMD -MP

# Includes:
INC =       -I ./include/

# Source and object files:
SRC_PATH =  src/
SRC =       webserv.cpp utils.cpp Parser.cpp Server.cpp ServerManager.cpp \
            Client.cpp HttpRequest.cpp Response.cpp Location.cpp Mime.cpp Error.cpp \
            Interface.cpp Log.cpp Cgi.cpp
SRCS =      $(addprefix $(SRC_PATH), $(SRC))

OBJ_PATH =  obj/
OBJ =       $(SRC:.cpp=.o)
OBJS =      $(addprefix $(OBJ_PATH), $(OBJ))
DEPS =      $(OBJS:.o=.d)

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(OBJ_PATH)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ $(INC)
	@echo "$(GREEN)Compiled "$<" successfully!$(NC)"


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
	@echo "                                              "
	@echo " run ./webserv <config_file> <flag>  "
	@echo "                                              $(NC)"
	@echo "$(BOLD)$(GREEN) optional flag: "
	@echo "  "
	@echo "  --debug :    debug logs"
	@echo "  "
	@echo "                                       $(NC)"

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

.PHONY: all re clean fclean bonus

