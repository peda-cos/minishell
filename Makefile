# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlacerda <jlacerda@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 19:01:24 by peda-cos          #+#    #+#              #
#    Updated: 2025/03/18 00:33:45 by jlacerda         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
NAME = minishell
LDFLAGS = -lreadline
LIBFT_DIR = ./libft
OBJS_DIR = ./objects
INCS_DIR = ./includes

PARSER_SRCS = sources/parser/main.c sources/parser/utils.c
HANDLER_SRCS = sources/handler/main.c
BUILTIN_SRCS = sources/builtin/main.c
EXECUTOR_SRCS = sources/executor/main.c
TOKENIZER_SRCS = sources/tokenizer/utils.c sources/tokenizer/main.c 

SRCS = \
	sources/main.c sources/utils.c \
	$(EXECUTOR_SRCS) $(TOKENIZER_SRCS) \
	$(PARSER_SRCS) $(HANDLER_SRCS) $(BUILTIN_SRCS) \

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)
LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror -g -I$(INCS_DIR) -I$(LIBFT_DIR)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)
	@echo "Compiling $(NAME)..."

$(LIBFT):
	@make -C $(LIBFT_DIR) all
	@echo "Libft build complete."

$(OBJS_DIR)/%.o: %.c 
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiling $<..."

clean:
	@rm -rf $(OBJS_DIR)
	@make clean -C $(LIBFT_DIR)
	@echo "Cleaning objects..."

fclean: clean
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@echo "Cleaning $(NAME)..."

re: fclean all

.PHONY: all clean fclean re
