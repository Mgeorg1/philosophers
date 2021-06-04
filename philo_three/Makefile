SRCS	=	main.c \
			utils.c \
			utils2.c \
			eat_sleep.c \
			philo_utils.c \
			death.c \
			routine.c \
			init.c \

NAME	=	philo_three

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
