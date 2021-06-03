#include "./philo_one.h"

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

void	fix_sleep(long long n)
{
	long long	t;

	t = gettime();
	while (gettime() - t < n)
	{
		usleep(1);
	}
}

int	init_args(t_all *all, int argc, char **argv)
{
	int	i;

	if (argc == 6)
		all->eat_num = ft_atoi(argv[5]);
	else
		all->eat_num = 0;
	all->phl_num = ft_atoi(argv[1]);
	all->ttodie = ft_atoi(argv[2]);
	all->ttoeat = ft_atoi(argv[3]);
	all->ttosleep = ft_atoi(argv[4]);
	if (all->phl_num < 2 || all->ttodie < 1 || all->ttoeat < 1
		|| all->ttosleep < 1)
		return (print_error("wrong argument"));
	all->philo = malloc(sizeof(t_philo) * (all->phl_num));
	if (!all->philo)
		return (print_error("malloc error"));
	all->fork = malloc(sizeof(pthread_mutex_t) * (all->phl_num));
	if (!all->fork)
		return (print_error("malloc error"));
	all->t_start = gettime();
	i = -1;
	while (++i < all->phl_num)
		all->philo[i].all = all;
	all->philo->all = all;
	return (0);
}

void	display_message(t_philo *philo, char *s)
{
	long long	t;

	t = gettime() - philo->all->t_start;
	if (philo->all->done)
		return ;
	pthread_mutex_lock(&philo->all->message);
	if (!philo->all->done)
		printf("%-8lli philo %-4i %s\n", t, philo->id + 1, s);
	pthread_mutex_unlock(&philo->all->message);
}

void	grab_fork(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->all->fork[i]);
	display_message(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->all->fork[i]);
}

void	get_ticket(t_philo *philo)
{
	int	done;

	done = 0;
	while (!done)
	{
		pthread_mutex_lock(&philo->all->ticket_mutex);
		if (philo->all->ticket > 0)
		{
			philo->all->ticket--;
			done = 1;
		}
		else
			done = 0;
		pthread_mutex_unlock(&philo->all->ticket_mutex);
	}
}

void	return_ticket(t_philo *philo)
{
	pthread_mutex_lock(&philo->all->ticket_mutex);
	philo->all->ticket++;
	pthread_mutex_unlock(&philo->all->ticket_mutex);
}

void	eat(t_philo *philo)
{
	philo->eating = 1;
	pthread_mutex_lock(&philo->eat_mutex);
	philo->cur_ttodie = gettime();
	display_message(philo, "is eating");
	fix_sleep(philo->all->ttoeat);
	philo->eat++;
	pthread_mutex_unlock(&philo->eat_mutex);
	philo->eating = 0;
}

void	philo_sleep(t_philo *philo)
{
	display_message(philo, "is sleeping");
	fix_sleep(philo->all->ttosleep);
}

void	*death(void *arg)
{
	t_philo		*philo;
	long long	t;

	philo = (t_philo *)arg;
	while (!philo->all->done)
	{
		pthread_mutex_lock(&philo->eat_mutex);
		t = gettime() - philo->cur_ttodie;
		if (t > philo->all->ttodie && !philo->eating)
		{
			pthread_mutex_lock(&philo->all->status);
			display_message(philo, "is died");
			philo->all->done = 1;
			pthread_mutex_unlock(&philo->eat_mutex);
			pthread_mutex_unlock(&philo->all->status);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->eat_mutex);
	}
	return (NULL);
}

void	down_forks(t_philo *philo, int l_fork, int r_fork)
{
	pthread_mutex_unlock(&philo->all->fork[l_fork]);
	pthread_mutex_unlock(&philo->all->fork[r_fork]);
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		r_fork;
	int		l_fork;

	philo = (t_philo *)arg;
	r_fork = philo->id;
	l_fork = philo->id + 1;
	if (l_fork == philo->all->phl_num)
		l_fork = 0;
	pthread_create(&philo->death, NULL, death, (void *)philo);
	pthread_detach(philo->death);
	while (!philo->all->done)
	{
		get_ticket(philo);
		grab_fork(philo, r_fork);
		grab_fork(philo, l_fork);
		eat(philo);
		down_forks(philo, l_fork, r_fork);
		return_ticket(philo);
		if (philo->eat >= philo->all->eat_num && philo->all->eat_num)
			break ;
		philo_sleep(philo);
		display_message(philo, "is thinking");
	}
	return (NULL);
}

int	init_mutexes(t_all *all)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&all->message, NULL))
		return (print_error("FATAL: create mutex error"));
	if (pthread_mutex_init(&all->ticket_mutex, NULL))
		return (print_error("FATAL: create mutex error"));
	if (pthread_mutex_init(&all->death, NULL))
		return (print_error("FATAL: create mutex error"));
	if (pthread_mutex_init(&all->status, NULL))
		return (print_error("FATAL: create mutex error"));
	pthread_mutex_lock(&all->death);
	while (++i < all->phl_num)
	{
		if (pthread_mutex_init(&all->fork[i], NULL))
			return (print_error("FATAL: create mutex error"));
		if (pthread_mutex_init(&all->philo[i].eat_mutex, NULL))
			return (print_error("FATAL: create mutex error"));
	}
	return (0);
}

int philo_init(t_all *all)
{
	int	i;

	init_mutexes(all);
	all->ticket = all->phl_num - 1;
	all->done = 0;
	i = -1;
	while (++i < all->phl_num)
	{	
		all->philo[i].id = i;
		all->philo[i].eat = 0;
		all->philo[i].cur_ttodie = all->t_start;
		all->philo[i].eating = 0;
		if (pthread_create(&all->philo[i].m_philo, NULL, &routine,
				(void *)&all->philo[i]))
			return (print_error("FATAL: can't create thread"));
	}
	i = -1;
	while (++i < all->phl_num)
	{
		if (pthread_join(all->philo[i].m_philo, NULL))
			return (print_error("FATAL: can't join thread"));
	}
	all->done = 1;
	return (0);
}

void	clear_all(t_all *all)
{
	int	i;

	i = -1;
	while (++i < all->phl_num)
	{
		pthread_mutex_destroy(&all->philo[i].eat_mutex);
		pthread_mutex_destroy(&all->fork[i]);
	}
	free(all->philo);
	free(all->fork);
	pthread_mutex_destroy(&all->message);
	pthread_mutex_destroy(&all->death);
	pthread_mutex_destroy(&all->ticket_mutex);
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