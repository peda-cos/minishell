# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/19 19:01:24 by peda-cos          #+#    #+#              #
#    Updated: 2025/04/13 15:24:26 by peda-cos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
NAME = minishell
LDFLAGS = -lreadline
LIBFT_DIR = ./libft
OBJS_DIR = ./objects
INCS_DIR = ./includes

PARSER_SRCS = sources/parser/main.c sources/parser/utils.c
HANDLER_SRCS = sources/handler/main.c sources/handler/utils.c
BUILTIN_SRCS = sources/builtin/env.c sources/builtin/cd_utils.c \
				sources/builtin/cd.c sources/builtin/echo.c \
				sources/builtin/pwd.c sources/builtin/unset.c \
				sources/builtin/exit.c sources/builtin/export.c sources/builtin/export_utils.c
EXECUTOR_SRCS = sources/executor/main.c sources/executor/path_utils.c \
				sources/executor/heredoc.c sources/executor/redirection.c \
				sources/executor/command_utils.c sources/executor/process.c \
				sources/executor/process_utils.c
TOKENIZER_SRCS = sources/tokenizer/utils.c sources/tokenizer/main.c \
				sources/tokenizer/word.c sources/tokenizer/redirect.c \
				sources/tokenizer/token.c
EXPANSION_SRCS = sources/expansion/main.c sources/expansion/utils.c
VALIDATOR_SRCS = sources/validator/main.c sources/validator/utils.c
HISTORY_SRCS = sources/history/main.c sources/history/read.c \
				sources/history/file.c sources/history/write.c

SRCS = \
	sources/main.c sources/utils.c sources/free.c sources/gay.c \
	$(EXECUTOR_SRCS) $(TOKENIZER_SRCS) \
	$(PARSER_SRCS) $(HANDLER_SRCS) $(BUILTIN_SRCS) \
	$(EXPANSION_SRCS) $(VALIDATOR_SRCS) $(HISTORY_SRCS)

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