# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: junsan <junsan@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/11 19:03:01 by junsan            #+#    #+#              #
#    Updated: 2024/05/12 18:46:15 by junsan           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
BONUS = minishell_bonus

LIBFT = libft/libft.a
IFLAGS = -I ./includes/ -I ./libft/

CFLAGS = -Wall -Wextra -Werror

SRC = 

SRC_DIR = ./src/
OBJ_DIR = ./obj/

SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJS = $(addprefix $(OBJ_DIR), $(SRC:.c=.o))

$(NAME) : $(LIBFT) $(OBJS)
	cc $(CFALGS) -o $@ $(B_OBJS) $(LIBFT)

vpath %.c ./src/

$(OBJ_DIR)%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(LIBFT): 
	@make -C libft/

all: $(TARGET)

clean:
	@make -C libft/ clean
	rm -rf $(OBJ_DIR) $(BONUS_OBJ_DIR)

fclean: clean
	@make -C libft/ fclean
	rm -f $(TARGET) $(BONUS)

re: fclean all

.PHONY: all clean fclean re bonus
