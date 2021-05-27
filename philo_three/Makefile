SRCS	=	main.c \
			utils.c \

NAME	=	philo_two

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
