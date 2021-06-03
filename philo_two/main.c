#include "./philo_two.h"

long long	gettime(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)(1000 * tp.tv_sec + tp.tv_usec / 1000));
}

int	print_error(char *s)
{
	printf("ERROR\n%s\n", s);
	return (1);
}

int	is_digit_str(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	fix_sleep(long long n, t_all *all)
{
	long long	t;

	t = gettime();
	while (gettime() - t < n && !all->done)
	{
		usleep(1);
	}
}

void	display_message(t_philo *philo, char *s)
{
	long long	t;

	t = gettime() - philo->all->t_start;
	if (philo->all->done)
		return ;
	sem_wait(philo->all->message);
	if (!philo->all->done && !philo->full)
		printf("%-8lli philo %-4i %s\n", t, philo->id + 1, s);
	sem_post(philo->all->message);
}

void	eat(t_philo *philo)
{
	philo->eating = 1;
	sem_wait(philo->eat_sem);
	display_message(philo, "is eating");
	philo->cur_ttodie = gettime();
	fix_sleep(philo->all->ttoeat, philo->all);
	philo->eat++;
	sem_post(philo->eat_sem);
	philo->eating = 0;
}

void	philo_sleep(t_philo *philo)
{
	display_message(philo, "is sleeping");
	fix_sleep(philo->all->ttosleep, philo->all);
}

void	get_ticket(t_philo *philo)
{
	sem_wait(philo->all->ticket_sem);
}

void	return_ticket(t_philo *philo)
{
	sem_post(philo->all->ticket_sem);
}

void	is_full(t_philo *philo)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (++i < philo->all->phl_num)
	{
		if (philo->all->philo[i].eat == philo->all->eat_num)
			count++;
	}
	if (count == philo->all->phl_num)
	{
		philo->all->done = 1;
	}
}

void	life_cycle(t_philo *philo)
{
	get_ticket(philo);
	sem_wait(philo->all->fork);
	display_message(philo, "has taken a fork");
	sem_wait(philo->all->fork);
	display_message(philo, "has taken a fork");
	eat(philo);
	sem_post(philo->all->fork);
	sem_post(philo->all->fork);
	return_ticket(philo);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->eating = 0;
	philo->cur_ttodie = philo->all->t_start;
	pthread_create(&philo->death, NULL, death, (void *)philo);
	pthread_detach(philo->death);
	while (!philo->all->done && !philo->full)
	{
		life_cycle(philo);
		if (philo->eat >= philo->all->eat_num && philo->all->eat_num)
		{
			philo->full = 1;
			break ;
		}
		philo_sleep(philo);
		display_message(philo, "is thinking");
	}
	is_full(philo);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int		i;
	t_all	all;

	if (argc > 6 || argc < 5)
		return (print_error("incorrect num of arguments"));
	i = 1;
	while (argv[i])
	{
		if (!is_digit_str(argv[i]))
			return (print_error("Argument is not digit"));
		i++;
	}
	if (init_args(&all, argc, argv))
		return (1);
	philo_init(&all);
	clear_all(&all);
	return (0);
}
