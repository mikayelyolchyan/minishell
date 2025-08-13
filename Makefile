NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address -fsanitize=undefined

SOURCES_DIR = sources/
PARSER_DIR = sources/parser/
EXECUTOR_DIR = sources/executor/
BUILTINS_DIR = sources/builtins/
ENV_DIR = sources/env/
UTILS_DIR = sources/utils/
SIGNALS_DIR = sources/signals/
HEADERS_DIR = includes/
LIBFT_DIR = libraries/libft/
LIBFT = $(LIBFT_DIR)libft.a

SRCS = \
    $(SOURCES_DIR)main.c
    #$(PARSER_DIR)parser.c \
    #$(PARSER_DIR)tokenizer.c \
    #$(EXECUTOR_DIR)executor.c \
    #$(EXECUTOR_DIR)pipeline.c \
    #$(BUILTINS_DIR)cd.c \
    #$(BUILTINS_DIR)echo.c \
    #$(ENV_DIR)env.c \
    #$(ENV_DIR)export.c \
    #$(UTILS_DIR)str_utils.c \
    #$(SIGNALS_DIR)signals.c

HDRS = \
    $(HEADERS_DIR)minishell.h
    #$(HEADERS_DIR)parser/parser.h \
    #$(HEADERS_DIR)executor/executor.h \
    #$(HEADERS_DIR)builtins/cd.h \
    #$(HEADERS_DIR)env/env.h \
    #$(HEADERS_DIR)signals/signals.h \
    #$(HEADERS_DIR)utils/str_utils.h

OBJS = $(SRCS:.c=.o)

INCLUDES = -I$(HEADERS_DIR) -I$(LIBFT_DIR) -I$(HEADERS_DIR)parser -I$(HEADERS_DIR)executor -I$(HEADERS_DIR)builtins -I$(HEADERS_DIR)env -I$(HEADERS_DIR)signals -I$(HEADERS_DIR)utils

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBFT) -o $(NAME)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) all bonus

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re