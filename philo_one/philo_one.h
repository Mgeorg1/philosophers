#ifndef PHILO_ONE_H
# define PHILO_ONE_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>

typedef struct s_philo
{
	int			id;
	t_all		*all;
	int			l_fork;
	int			r_fork;
	pthread_t	m_philo;
}	t_philo;

typedef struct s_all
{
	int				ttodie;
	int				ttoeat;
	int				ttosleep;
	int				eat_num;
	int				phl_num;
	pthread_mutex_t	*fork;
	pthread_mutex_t	message;
	int				inc;
	t_philo			*philo;
}		t_all;

int	ft_atoi(const char *str);
int	ft_strlen(char *s);
int	ft_isdigit(int c);

#endif