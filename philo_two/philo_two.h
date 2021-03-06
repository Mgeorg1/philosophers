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
	int				done;
	long long		t_start;
}		t_all;

int			ft_atoi(const char *str);
int			ft_strlen(char *s);
int			ft_isdigit(int c);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_itoa(int n);
void		*routine(void *arg);
int			philo_init(t_all *all);
void		clear_all(t_all *all);
int			init_args(t_all *all, int argc, char **argv);
void		*death(void *arg);
void		fix_sleep(long long n, t_all *all);
long long	gettime(void);
void		display_message(t_philo *philo, char *s);
int			print_error(char *s);
void		philo_sleep(t_philo *philo);
void		eat(t_philo *philo);
int			is_digit_str(char *s);
void		get_ticket(t_philo *philo);
void		return_ticket(t_philo *philo);
void		is_full(t_philo *philo);
void		life_cycle(t_philo *philo);
void		*routine(void *arg);

#endif