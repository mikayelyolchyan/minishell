NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address -fsanitize=undefined

SOURCES_DIR = src/
LEXER_DIR = src/lexer/
PARSER_DIR = src/parser/
PROMPT_DIR = src/prompt/
HEADERS_DIR = include/
EXECUTOR_DIR = src/executor/
BUILTINS_DIR = src/builtins/
EXPANSION_DIR = src/expansion/
SIGNALS_DIR = src/signals/
LIBFT_DIR = lib/libft/
LIBFT = $(LIBFT_DIR)libft.a
BIN_DIR = ./
BUILD_DIR = build

SRCS = \
	src/main.c \
	$(PROMPT_DIR)prompt.c \
	$(LEXER_DIR)tokenizer.c \
	$(LEXER_DIR)token_set.c \
	$(LEXER_DIR)token_free.c \
	$(LEXER_DIR)token_utils.c \
	$(LEXER_DIR)utils.c \
	$(PARSER_DIR)parsing.c \
	$(PARSER_DIR)syntax_analyze.c \
	$(PARSER_DIR)syntax_analyze_utils.c \
	$(PARSER_DIR)syntax_checks.c \
	$(PARSER_DIR)syntax_quotes.c \
	$(PARSER_DIR)ast.c \
	$(PARSER_DIR)ast_logical.c \
	$(PARSER_DIR)ast_command_helper.c \
	$(PARSER_DIR)ast_free.c \
	$(PARSER_DIR)ast_redir.c \
	$(PARSER_DIR)print_ast.c \
	$(PARSER_DIR)heredoc.c \
	$(PARSER_DIR)heredoc_utils.c \
	$(PARSER_DIR)heredoc_io.c \
	$(EXECUTOR_DIR)executor.c \
	$(EXECUTOR_DIR)ast_interpretor.c \
	$(EXECUTOR_DIR)command_utils.c \
	$(EXECUTOR_DIR)command_empty.c \
	$(EXECUTOR_DIR)redir_utils.c \
	$(EXECUTOR_DIR)builtin_redir.c \
	$(EXECUTOR_DIR)logical_ops.c \
	$(EXECUTOR_DIR)subshell.c \
	$(EXECUTOR_DIR)pipe_utils.c \
	$(EXECUTOR_DIR)path_utils.c \
	$(EXECUTOR_DIR)path_checks.c \
	$(BUILTINS_DIR)builtins.c \
	$(BUILTINS_DIR)echo.c \
	$(BUILTINS_DIR)pwd.c \
	$(BUILTINS_DIR)cd.c \
	$(BUILTINS_DIR)env_builtin.c \
	$(BUILTINS_DIR)exit.c \
	$(BUILTINS_DIR)export.c \
	$(BUILTINS_DIR)export_utils.c \
	$(BUILTINS_DIR)export_print.c \
	$(BUILTINS_DIR)unset.c \
	$(EXPANSION_DIR)expansion.c \
	$(EXPANSION_DIR)expansion_utils.c \
	$(EXPANSION_DIR)expansion_helpers.c \
	$(EXPANSION_DIR)expansion_quotes.c \
	$(SIGNALS_DIR)signals.c \
	src/env/env.c \
	src/env/env_utils.c \
	src/env/env_cleanup.c

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

HDRS = \
    $(HEADERS_DIR)minishell.h \
    $(HEADERS_DIR)lexer/lexer.h \
    $(HEADERS_DIR)lexer/utils.h \
    $(HEADERS_DIR)prompt/prompt.h

INCLUDES = -I$(HEADERS_DIR) -I$(LIBFT_DIR)

all: $(BUILD_DIR) $(LIBFT) $(BIN_DIR)$(NAME)

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR)$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -lreadline -lhistory -o $@

$(BUILD_DIR)/%.o: %.c $(HDRS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) all bonus

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(BIN_DIR)$(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re