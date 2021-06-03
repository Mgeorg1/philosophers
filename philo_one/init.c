#include "./philo_one.h"

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

int	philo_init(t_all *all)
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
