#ifndef PHILO_ONE_H
# define PHILO_ONE_H
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <string.h>

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
	pthread_mutex_t	eat_mutex;
	int				full;
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
	t_philo			*philo;
	pthread_mutex_t	death;
	pthread_mutex_t	ticket_mutex;
	pthread_mutex_t	status;
	int				ticket;
	int				done;
	long long		t_start;
}		t_all;

int			ft_atoi(const char *str);
int			ft_strlen(char *s);
int			ft_isdigit(int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			init_args(t_all *all, int argc, char **argv);
int			init_mutexes(t_all *all);
int			philo_init(t_all *all);
void		*routine(void *arg);
long long	gettime(void);
int			print_error(char *s);
int			is_digit_str(char *s);
void		fix_sleep(long long n, t_all *all);
void		display_message(t_philo *philo, char *s);
void		*death(void *arg);
void		eat(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		clear_all(t_all *all);
#endif