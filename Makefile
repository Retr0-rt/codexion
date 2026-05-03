NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread

SRCS_DIR    = srcs
INCS_DIR    = includes
OBJS_DIR    = objs

SRCS_FILES  = main.c init.c simulation.c resources.c utils.c cleanup.c heap.c monitor.c parser.c ft_trim.c heap_utils.c

SRCS        = $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))
OBJS        = $(addprefix $(OBJS_DIR)/, $(SRCS_FILES:.c=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INCS_DIR)/codexion.h | $(OBJS_DIR)
	$(CC) $(CFLAGS) -I $(INCS_DIR) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re