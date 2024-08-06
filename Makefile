# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: junsan <junsan@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    CreateCd: 2024/05/11 19:03:01 by junsan            #+#    #+#             #
#    Updated: 2024/08/06 09:36:35 by junsan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

COLOR_RESET = \033[0m
COLOR_RED = \033[1;31m
COLOR_GREEN = \033[1;32m
COLOR_YELLOW = \033[1;33m
COLOR_BLUE = \033[1;34m
COLOR_CYAN = \033[1;36m

NAME 	= minishell
OS		= $(shell uname)

SPINNER_SCRIPT = assets/spinner.sh

CC		= cc
LIBFT 	= libft/libft.a
IFLAGS 	:= -I ./includes/ -I ./libft/includes/

CFLAGS 	:= -Wall -Wextra -Werror -g3

SRC_DIR = src
PARSING_DIR = $(SRC_DIR)/parsing
TOKENIZE_DIR = $(PARSING_DIR)/tokenize
VALIIDATION_DIR = $(PARSING_DIR)/utils/validation
BUILT_IN_DIR = $(SRC_DIR)/built_in
UTILS_DIR = $(SRC_DIR)/utils
INIT_DIR = $(SRC_DIR)/init
SIGNAL_DIR = $(SRC_DIR)/signal
EXECUTE_DIR = $(SRC_DIR)/exec
EXPANSION_AND_QUOTES_DIR = $(EXECUTE_DIR)/utils/expansion_and_quotes
PRINT_DIR = $(SRC_DIR)/print

OBJ_DIR = obj

SRC 	= minishell.c process_input.c
INIT	= init_minishell.c env_init.c increment_shlvl.c env_utils.c env_utils_2.c
PARSING	= parsing.c arg_parse.c parse_subshell.c parse_logical.c parse_pipe.c 		\
		parse_phrase.c parse_redirection.c parse_phrase_parts.c						\
		/utils/parsing_utils.c /utils/tokenize_utils.c /utils/tokenize_utils_2.c	\
		/utils/type_redir_functions.c /utils/type_functions.c /utils/get_type.c 	\
		/utils/subshell_utils.c /utils/parsing_utils_2.c 							\
		/utils/collect_data_until_subshell.c /utils/parsing_quotes_in_cmd.c
TOKENIZE = tokenize.c handler_operators_and_spaces.c handle_quotes.c 				\
		handle_subshell.c
VALIIDATION = valid_token.c valid_token_utils.c valid_token_err.c 					\
		valid_token_err_2.c
UTILS	= string_utils.c string_utils_2.c quotes_str.c error_utils.c 				\
		subshell_and_quote_str.c
SIGNAL	= handler_signal.c handler_heredoc_signal.c
EXECUTE = execute.c redir.c get_file_list.c dispatch_cmd.c launch_process_cmd.c 	\
		launch_process_pipe.c execution_pipe.c subshell.c							\
		process_execute.c process_redir.c process_execute_pipe.c					\
		/utils/redir_utils.c /utils/init_info.c /utils/info_utils.c 				\
		/utils/get_file_list_utils.c												\
		/utils/fd_utils.c  /utils/list_to_array.c /utils/args_utils.c 				\
		/utils/get_absolute_path.c /utils/get_path_type.c /utils/find_cmd_in_path.c	\
		/utils/quotes_utils.c /utils/get_bin_path.c /utils/here_doc.c				\
		/utils/redirect_to_null.c													
EXPANSION_AND_QUOTES = var_expansion_with_args.c replace_env_vars.c					\
		replace_env_vars_utils.c replace_env_vars_utils_2.c							\
		handler_replace_env_vars.c handler_under_score.c							\
		process_expand_strip_quotes.c handler_replace_env_vars_without_quotes.c		\
		init_utils_for_expansion.c	init_utils_for_expansion_2.c					\
		expand_wildcard.c expand_wildcard_utils.c normalize_spaces.c
BUILT_IN = built_in.c ft_cd.c  ft_env.c ft_export.c ft_unset.c						\
		ft_echo.c ft_exit.c ft_pwd.c												\
		utils/cd_utils.c utils/ft_export_utils.c utils/builtin_node_utils.c			\
		utils/handle_pwd_oldpwd.c
PRINT	= prints.c prints_2.c

SRCS := $(addprefix $(SRC_DIR)/, $(SRC))
SRCS += $(addprefix $(PRINT_DIR)/, $(PRINT))
SRCS += $(addprefix $(UTILS_DIR)/, $(UTILS))
SRCS += $(addprefix $(PARSING_DIR)/, $(PARSING))
SRCS += $(addprefix $(INIT_DIR)/, $(INIT))
SRCS += $(addprefix $(SIGNAL_DIR)/, $(SIGNAL))
SRCS += $(addprefix $(EXECUTE_DIR)/, $(EXECUTE))
SRCS += $(addprefix $(BUILT_IN_DIR)/, $(BUILT_IN))
SRCS += $(addprefix $(EXPANSION_AND_QUOTES_DIR)/, $(EXPANSION_AND_QUOTES))
SRCS += $(addprefix $(VALIIDATION_DIR)/, $(VALIIDATION))
SRCS += $(addprefix $(TOKENIZE_DIR)/, $(TOKENIZE))

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))

ifeq ($(OS), Linux)
	LD_FLAGS = -lreadline
else ifeq ($(OS), Darwin)
	LD_FLAGS = -L$(shell brew --prefix readline)/lib -lreadline
	IFLAGS += -I $(shell brew --prefix readline)/include
endif

vpath %.c ./src/

all: $(NAME)

$(NAME) : $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBFT) $(LD_FLAGS) > /dev/null 2>&1 & COMPILER_PID=$$!; \
	./$(SPINNER_SCRIPT) $$COMPILER_PID; \
	wait $$COMPILER_PID
	@echo "$(COLOR_GREEN)Program Name : $(NAME)$(COLOR_RESET)"

OBJ_FILES_SPINNER_PID=

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	#@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@ > /dev/null 2>&1
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(LIBFT): 
	@echo "$(COLOR_YELLOW)Compliling $(NAME)...$(COLOR_RESET)"
	@chmod +x $(SPINNER_SCRIPT)
	@$(MAKE) -s -C libft/ > /dev/null 2>&1 & COMPILER_PID=$$!; \
	./$(SPINNER_SCRIPT) $$COMPILER_PID; \
	wait $$COMPILER_PID
	@echo "$(COLOR_BLUE)Compliling Obj files...$(COLOR_RESET)"

debug: CFLAGS += -g3 -fsanitize=address
debug: fclean $(NAME)
		@echo "$(COLOR_GREEN)Start Debugging! 🛠️$(COLOR_RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(COLOR_RED)Cleaning completed successfully 🧹$(COLOR_RESET)"

fclean:
	@make -s -C libft/ fclean
	@rm -f $(NAME)
	@rm -rf $(OBJ_DIR)
	@echo "$(COLOR_RED)Full Cleaning completed successfully 🧹$(COLOR_RESET)"

re: fclean all
	@echo "$(COLOR_GREEN)Recompleted successfully 🎉$(COLOR_RESET)"

.PHONY: all clean fclean re debug
