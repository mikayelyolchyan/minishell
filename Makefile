NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address -fsanitize=undefined

SOURCES_DIR = src/
LEXER_DIR = src/lexer/
PARSER_DIR = src/parser/
PROMPT_DIR = src/prompt/
HEADERS_DIR = include/
LIBFT_DIR = lib/libft/
LIBFT = $(LIBFT_DIR)libft.a
BIN_DIR = bin
BUILD_DIR = build

# Основные исходники (для minishell)
SRCS = \
    src/main.c \
    $(PROMPT_DIR)prompt.c \
    $(LEXER_DIR)tokenizer.c \
    $(LEXER_DIR)token_set.c \
    $(LEXER_DIR)token_free.c \
    $(LEXER_DIR)utils.c \
    $(PARSER_DIR)parsing.c \
    $(PARSER_DIR)syntax_analyze.c \
    $(PARSER_DIR)syntax_analyze_utils.c \
	$(PARSER_DIR)token_syntax.c \
	$(PARSER_DIR)subshell_syntax.c \
	$(PARSER_DIR)control_operator_syntax.c \
	$(PARSER_DIR)quote_syntax.c \
	$(PARSER_DIR)redirection_syntax.c \
    tests/lexer/print_tokens.c

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Тестовые исходники (своё main)
TEST_SRCS = \
    tests/lexer/lexer.c \
    $(LEXER_DIR)tokenizer.c \
    $(LEXER_DIR)token_set.c \
    $(LEXER_DIR)token_free.c \
    $(LEXER_DIR)utils.c \
    $(PARSER_DIR)parsing.c\
    $(PARSER_DIR)syntax_analyze.c \
    $(PARSER_DIR)syntax_analyze_utils.c \
    tests/lexer/syntax_analyze.c\

TEST_OBJS = $(TEST_SRCS:%.c=$(BUILD_DIR)/%.o)

HDRS = \
    $(HEADERS_DIR)minishell.h \
    $(HEADERS_DIR)lexer/lexer.h \
    $(HEADERS_DIR)lexer/utils.h \
    $(HEADERS_DIR)prompt/prompt.h

INCLUDES = -I$(HEADERS_DIR) -I$(LIBFT_DIR)

all: $(BIN_DIR) $(BUILD_DIR) $(LIBFT) $(BIN_DIR)/$(NAME)

$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR)/$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -lreadline -lhistory -o $@

# Компиляция всех .c в build/
$(BUILD_DIR)/%.o: %.c $(HDRS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) all bonus

clean:
	rm -f $(OBJS) $(TEST_OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(BIN_DIR)/$(NAME) $(BIN_DIR)/lexer_test
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# Собираем тест без src/main.c
lexer_test: $(BIN_DIR) $(BUILD_DIR) $(LIBFT) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(TEST_OBJS) $(LIBFT) -lreadline -o $(BIN_DIR)/lexer_test

#heto_kjnjes
valgrind_lexer:
	$(MAKE) fclean
	$(MAKE) lexer_test CFLAGS="-Wall -Wextra -Werror -g3"
	valgrind --suppressions=lreadline.supp --leak-check=full $(BIN_DIR)/lexer_test


.PHONY: all clean fclean re lexer_test
