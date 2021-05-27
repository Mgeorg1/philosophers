#ifndef PHILO_TWO_H
# define PHILO_TWO_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <string.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>

struct	s_all;

typedef struct s_philo
{
	int				id;
	struct s_all	*all;
	int				l_fork;
	int				r_fork;
	pthread_t		m_philo;
	int				ph_ttodie;
	long long		cur_ttodie;
	int				eat;
	pthread_t		death;
	int				eating;
	sem_t			*eat_sem;
	int				full;
	int				pid;
}	t_philo;

typedef struct s_all
{
	int				ttodie;
	int				ttoeat;
	int				ttosleep;
	int				eat_num;
	int				phl_num;
	sem_t			*fork;
	sem_t			*message;
	int				inc;
	t_philo			*philo;
	sem_t			*ticket_sem;
	sem_t			*status;
	sem_t			*die;
	pthread_t		is_die;
	int				done;
	long long		t_start;
}		t_all;

int		ft_atoi(const char *str);
int		ft_strlen(char *s);
int		ft_isdigit(int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_itoa(int n);

#endif