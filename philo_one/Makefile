SRCS	=	main.c \
			utils.c \
			philo_utils.c \
			routine.c \
			eat_sleep.c \
			init.c \
			death.c \

NAME	=	philo_one

OBJS	=	$(SRCS:.c=.o)

all:	$(NAME)

$(NAME):	$(OBJS)
			gcc -Wall -Wextra -Werror -lpthread $(SRCS) -o $(NAME)

clean: 
		rm -f $(OBJS)

fclean:	clean
		rm -f $(NAME)

re:	fclean all

.PHONY: re clean fclean all
