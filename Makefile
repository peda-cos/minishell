# **************************************************************************** #
#                                                                              #
#    Makefile                                             :::      ::::::::    #
#                                                       :+:      :+:    :+:    #
#    By: peda-cos <peda-cos@student.42sp.org.br>      +:+ +:+         +:+      #
#    By: jlacerda <jlacerda@student.42sp.org.br>    +#+  +:+       +#+         #
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

MAIN_SRCS = sources/main.c \
	sources/no_interactive.c sources/utils.c sources/free.c sources/gay.c sources/fd_manager.c
PARSER_SRCS = sources/parser/main.c \
	sources/parser/utils.c sources/parser/redirect.c sources/parser/cmd_utils.c
HANDLER_SRCS = sources/handler/main.c sources/handler/utils.c
BUILTIN_SRCS = sources/builtin/utils.c \
	sources/builtin/env.c sources/builtin/pwd.c sources/builtin/unset.c \
	sources/builtin/cd_utils.c sources/builtin/cd.c sources/builtin/echo.c \
	sources/builtin/exit.c sources/builtin/export.c sources/builtin/export_utils.c
EXECUTOR_SRCS = sources/executor/main.c \
	sources/executor/process_utils.c sources/executor/path_utils.c \
	sources/executor/redirection.c sources/executor/redirection_utils.c \
	sources/executor/command_utils.c sources/executor/process.c \
	sources/executor/heredoc.c sources/executor/heredoc_utils.c
TOKENIZER_SRCS = sources/tokenizer/main.c \
	sources/tokenizer/utils.c sources/tokenizer/redirect.c sources/tokenizer/token.c \
	sources/tokenizer/word.c	sources/tokenizer/word_utils.c	sources/tokenizer/word_content.c
EXPANSION_SRCS = sources/expansion/main.c sources/expansion/utils.c \
	sources/expansion/advanced.c
VALIDATOR_SRCS = sources/validator/main.c sources/validator/utils.c
HISTORY_SRCS = sources/history/main.c sources/history/read.c \
	sources/history/file.c sources/history/write.c

SRCS = $(MAIN_SRCS) \
	$(EXECUTOR_SRCS) $(TOKENIZER_SRCS) \
	$(PARSER_SRCS) $(HANDLER_SRCS) $(BUILTIN_SRCS) \
	$(EXPANSION_SRCS) $(VALIDATOR_SRCS) $(HISTORY_SRCS)

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)
LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror -g -I$(INCS_DIR) -I$(LIBFT_DIR) -O2

RESET = \033[0m
BOLD = \033[1m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
MAGENTA = \033[35m
CYAN = \033[36m
WHITE = \033[37m

TOTAL_FILES := $(words $(SRCS))
COMPILED_COUNT = 0
BAR_LENGTH = $(TOTAL_FILES)

all: header $(NAME)

header:
	@if [ ! -f .header_lock ]; then \
		touch .header_lock; \
		echo; \
		echo "$(MAGENTA)$(BOLD)🔥  M I N I S H E L L$(RESET)"; \
		echo "$(GREEN)$(BOLD)👥  Autores:$(RESET) $(YELLOW)peda-cos$(RESET), $(YELLOW)jonnathan-ls$(RESET)"; \
		echo; \
	fi

$(NAME): $(LIBFT) $(OBJS)
	@printf "\n$(BLUE)$(BOLD)🔗  Linkando arquivos ...$(RESET)\n"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)
	@echo "$(GREEN)$(BOLD)🚀  Minishell compilado com sucesso!$(RESET)"
	@echo
	@rm -f .header_lock

$(LIBFT):
	@printf "$(BLUE)$(BOLD)🏗️   Compilando libft $(RESET)\n"
	@echo -n "🔄  "
	@make --no-print-directory -C $(LIBFT_DIR) > /dev/null 2>&1 & \
	pid=$$!; \
	i=0; \
	while kill -0 $$pid 2>/dev/null; do \
		printf "$(YELLOW)▓$(RESET)"; \
		sleep 0.1; \
		i=$$((i+1)); \
		if [ $$i -gt 30 ]; then \
			break; \
		fi; \
	done; \
	wait $$pid
	@printf "\033[1A\r"
	@printf "$(BLUE)$(BOLD)🏗️   Compilando libft $(GREEN)$(BOLD)✓ Concluído!$(RESET)\n"
	@printf "🔄  $(YELLOW)%s$(RESET) \n" "$(shell printf '▓%.0s' $$(seq 1 30))"

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(eval COMPILED_COUNT=$(shell echo $$(($(COMPILED_COUNT)+1))))
	@$(eval SHORT_PATH=$(shell echo $< | sed 's|sources/||'))
	@$(CC) $(CFLAGS) -c $< -o $@

	@if [ $(COMPILED_COUNT) -eq 1 ]; then \
		echo ""; \
		printf "$(BLUE)$(BOLD)🏗️   Compilando minishell $(RESET)\n"; \
	fi

	@if [ $(COMPILED_COUNT) -gt 1 ]; then \
		printf "\033[2A\r"; \
	fi

	@printf "$(CYAN)$(BOLD)📄  Processando $(RESET)($(COMPILED_COUNT)/$(TOTAL_FILES)) $(CYAN)$(SHORT_PATH)$(RESET)%*s\r\n" $$(( 60 - $${#SHORT_PATH} )) " "
	@$(eval FILLED=$(shell echo $$(($(COMPILED_COUNT)*$(BAR_LENGTH)/$(TOTAL_FILES)))))
	@printf "$(YELLOW)$(BOLD)🔄  $(RESET)$(YELLOW)%s$(RESET) \n" "$(shell printf '▓%.0s' $$(seq 1 $(FILLED)))"

	@if [ $(COMPILED_COUNT) -eq $(TOTAL_FILES) ]; then \
		sleep 0.3; \
		printf "\033[3A\r"; \
		printf "                                                              \r"; \
		printf "$(BLUE)$(BOLD)🏗️   Compilando minishell $(GREEN)$(BOLD)✓ Concluído!$(RESET)\n"; \
		printf "$(CYAN)$(BOLD)📄  Processando $(RESET)($(COMPILED_COUNT)/$(TOTAL_FILES)) $(CYAN)$(SHORT_PATH)$(RESET)%*s\n" $$(( 60 - $${#SHORT_PATH} )) " "; \
		printf "$(YELLOW)$(BOLD)🔄  $(RESET)$(YELLOW)%s$(RESET) \n" "$(shell printf '▓%.0s' $$(seq 1 $(FILLED)))"; \
	fi

clean: header
	@printf "$(MAGENTA)$(BOLD)🗑️   Limpando arquivos objeto $(RESET)"
	@rm -rf $(OBJS_DIR)
	@make --no-print-directory -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@echo "$(GREEN)$(BOLD)✓ Objetos removidos!$(RESET)"
	@rm -f .header_lock

fclean: header
	@printf "$(MAGENTA)$(BOLD)🗑️   Limpando arquivos objeto $(RESET)"
	@rm -rf $(OBJS_DIR)
	@make --no-print-directory -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@echo "$(GREEN)$(BOLD)✓ Concluído!$(RESET)"
	@printf "$(MAGENTA)$(BOLD)🗑️   Limpando executável $(RESET)"
	@rm -f $(NAME)
	@make --no-print-directory -C $(LIBFT_DIR) fclean > /dev/null 2>&1
	@echo "$(GREEN)$(BOLD)✓ Concluído!$(RESET)"
	@rm -f .header_lock
	@echo

re:
	@rm -f .header_lock
	@$(MAKE) --no-print-directory -s fclean all

valgrind: all
	@rm	-f .header_lock
	@valgrind --leak-check=full \
	--show-reachable=yes \
	--track-fds=yes \
	--show-leak-kinds=all -s \
	--track-origins=yes \
	--suppressions=./leaks.supp \
	./$(NAME)

.PHONY: all clean fclean re header
