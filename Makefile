NAME        = codexion

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -pthread

SRCS_DIR    = srcs
INCS_DIR    = includes
OBJS_DIR    = objs

SRCS_FILES  = main.c init.c simulation.c resources.c scheduler.c heap.c monitor.c utils.c cleanup.c

# Advanced mapping: Prepend directories to files
SRCS        = $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))
OBJS        = $(addprefix $(OBJS_DIR)/, $(SRCS_FILES:.c=.o))

# Main rule
all: $(NAME)

# Linking the final executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Compiling object files
# The '-I $(INCS_DIR)' is what tells the compiler where your headers are!
# The '| $(OBJS_DIR)' ensures the objs folder exists before compiling.
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -I $(INCS_DIR) -c $< -o $@

# Create the objects directory
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Cleanup rules
clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re