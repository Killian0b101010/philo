# ==========================================================
# 											Variables générales
# ==========================================================

EXEC = philo
SRC_PATH = src
SRC = $(SRC_PATH)/philosopher.c 

OBJS = $(SRC:.c=.o)
DEPS = $(OBJS:.o=.d)

CC = cc 
CFLAGS = -Wall -Wextra -Werror -g3 -pthread -MMD -MP 
RM = rm -f

# ==========================================================
# 													Règles
# ==========================================================

all: $(EXEC)
#Creation de philo avec les dep 
$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) -o $@ $^

#Convertis .c en .o
%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@
# ==========================================================
# 													Nettoyage
# ==========================================================
#
# Supprime tous l'environnement 
clean : 
	$(RM) $(OBJS) $(DEPS)
	
# Supprime juste l'exectuable philoa.a et philo 
fclean : 
	$(RM) $(EXEC)

-include $(DEPS)

re : fclean all
