NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address -fsanitize=undefined

SOURCES_DIR = src/
PROMPT_DIR = src/prompt/
PARSER_DIR = src/parser/
EXECUTOR_DIR = src/executor/
BUILTINS_DIR = src/builtins/
ENV_DIR = src/env/
UTILS_DIR = src/utils/
SIGNALS_DIR = src/signals/
HEADERS_DIR = include/
LIBFT_DIR = lib/libft/
LIBFT = $(LIBFT_DIR)libft.a

BIN_DIR = bin
BUILD_DIR = build

SRCS = \
	$(SOURCES_DIR)main.c \
	$(PROMPT_DIR)prompt.c \
	#$(PARSER_DIR)parser.c \
	#$(EXECUTOR_DIR)executor.c \
	#$(EXECUTOR_DIR)pipeline.c \
	#$(BUILTINS_DIR)cd.c \
	#$(BUILTINS_DIR)echo.c \
	#$(ENV_DIR)env.c \
	#$(ENV_DIR)export.c \
	#$(UTILS_DIR)str_utils.c \
	#$(PARSER_DIR)tokenizer.c \
	#$(SIGNALS_DIR)signals.c

HDRS = \
	$(HEADERS_DIR)minishell.h \
	#$(HEADERS_DIR)parser/parser.h \
	#$(HEADERS_DIR)executor/executor.h \
	#$(HEADERS_DIR)builtins/cd.h \
	#$(HEADERS_DIR)env/env.h \
	#$(HEADERS_DIR)signals/signals.h \
	#$(HEADERS_DIR)utils/str_utils.h

OBJS = $(SRCS:src/%.c=build/%.o)

INCLUDES = -I$(HEADERS_DIR) \
			-I$(LIBFT_DIR) \
			-I$(HEADERS_DIR)prompt \
			-I$(HEADERS_DIR)parser \
			-I$(HEADERS_DIR)executor \
			-I$(HEADERS_DIR)builtins \
			-I$(HEADERS_DIR)env \
			-I$(HEADERS_DIR)signals \
			-I$(HEADERS_DIR)utils

all: $(BIN_DIR) $(BUILD_DIR) $(LIBFT) $(BIN_DIR)/$(NAME)

$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR)/$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) -lreadline $(OBJS) $(LIBFT) -o $@

$(BUILD_DIR)/%.o: src/%.c $(HDRS)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) all bonus

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(BIN_DIR)/$(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re