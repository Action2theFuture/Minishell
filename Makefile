# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: junsan <junsan@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/11 19:03:01 by junsan            #+#    #+#              #
#    Updated: 2024/06/26 09:32:11 by junsan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 	= minishell
OS		= $(shell uname)

CC		= cc
LIBFT 	= libft/libft.a
IFLAGS 	:= -I ./includes/ -I ./libft/includes/

CFLAGS 	:= -Wall -Wextra -Werror

SRC_DIR = src
PARSING_DIR = $(SRC_DIR)/parsing
BUILT_IN_DIR = $(SRC_DIR)/built_in
UTILS_DIR = $(SRC_DIR)/utils
INIT_DIR = $(SRC_DIR)/init
SIGNAL_DIR = $(SRC_DIR)/signal
EXECUTE_DIR = $(SRC_DIR)/exec
PRINT_DIR = $(SRC_DIR)/print
OBJ_DIR = obj

SRC 	= minishell.c process_input.c
INIT	= init_minishell.c env_init.c increment_shlvl.c env_utils.c
PARSING	= parsing.c arg_parse.c parse_subshell.c									\
		parse_logical.c parse_pipe.c parse_phrase.c parse_redirection.c				\
		/tokenize/tokenize.c /tokenize/handler_operators_and_spaces.c				\
		/tokenize/handle_quotes.c /tokenize/handle_subshell.c						\
		/utils/parsing_utils.c /utils/tokenize_utils.c /utils/tokenize_utils_2.c	\
		/utils/type_redir_functions.c /utils/type_functions.c /utils/get_type.c 	\
		/utils/subshell_utils.c /utils/valid_token.c
UTILS	= string_utils.c string_utils_2.c error_utils.c
SIGNAL	= handler_signal.c
EXECUTE = execute.c redir.c get_file_list.c execute_process.c cmd.c	launch_process.c\
		/utils/redir_utils.c /utils/info_utils.c /utils/get_file_list_utils.c		\
		/utils/stdio_redirector.c /utils/list_to_array.c /utils/args_utils.c 		\
		/utils/get_absolute_path.c /utils/var_expansion_with_args.c					\
		/utils/get_path_type.c /utils/find_cmd_in_path.c
BUILT_IN = built_in.c file_dir_operations.c	ft_cd.c  ft_env.c ft_export.c ft_unset.c\
		ft_echo.c ft_exit.c ft_pwd.c
PRINT	= prints.c prints_2.c

SRCS := $(addprefix $(SRC_DIR)/, $(SRC))
SRCS += $(addprefix $(PRINT_DIR)/, $(PRINT))
SRCS += $(addprefix $(UTILS_DIR)/, $(UTILS))
SRCS += $(addprefix $(PARSING_DIR)/, $(PARSING))
SRCS += $(addprefix $(INIT_DIR)/, $(INIT))
SRCS += $(addprefix $(SIGNAL_DIR)/, $(SIGNAL))
SRCS += $(addprefix $(EXECUTE_DIR)/, $(EXECUTE))
SRCS += $(addprefix $(BUILT_IN_DIR)/, $(BUILT_IN))

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

ifeq ($(OS), Linux)
	LD_FLAGS = -lreadline
else ifeq ($(OS), Darwin)
	LD_FLAGS = -L$(shell brew --prefix readline)/lib -lreadline
	IFLAGS += -I $(shell brew --prefix readline)/include
endif

vpath %.c ./src/

$(NAME) : $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(LD_FLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(LIBFT): 
	@make -C libft/

all: $(NAME)

debug: CFLAGS += -g3 -fsanitize=address
debug: $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C libft/ fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug
