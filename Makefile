# ==========================================================
#                       Variables générales
# ==========================================================

NAME     = philo
SRC_PATH = src

SRC = \
	$(SRC_PATH)/create_thread.c \
	$(SRC_PATH)/free.c \
	$(SRC_PATH)/geterseter.c \
	$(SRC_PATH)/philosopher.c \
	$(SRC_PATH)/routine.c \
	$(SRC_PATH)/routine2.c \
	$(SRC_PATH)/utils.c  \
	$(SRC_PATH)/utils2.c \


OBJS = $(SRC:.c=.o)
DEPS = $(OBJS:.o=.d)

CC     = cc
CFLAGS = -Wall -Wextra -Werror -g3 -pthread -MMD -MP 
RM     = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

help:
	@echo "Usage : ./$(NAME) number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]"
	@echo "Variables : N_PHILO T_DIE T_EAT T_SLEEP MUST_EAT"

clean:
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re run help asan

-include $(DEPS)
