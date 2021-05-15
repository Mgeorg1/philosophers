#include "philo_one.h"

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

int	init_args(t_all *all, int argc, char **argv)
{
	if (argc == 6)
		all->eat_num = ft_atoi(argv[5]);
	else
		all->eat_num = 1;
	all->phl_num = ft_atoi(argv[1]);
	all->ttodie = ft_atoi(argv[2]);
	all->ttoeat = ft_atoi(argv[3]);
	all->ttosleep = ft_atoi(argv[4]);
	all->philo = malloc(sizeof(t_philo) * (all->phl_num));
	if (!all->philo)
		return (1);
	all->fork = malloc(sizeof(pthread_mutex_t) * (all->phl_num));
	if (!all->fork)
		return (1);
	all->philo->all = all;
	return (0);
}

void	display_message(t_all *all, int philo, char *s, int tstmp)
{
	pthread_mutex_lock(&all->message);
	printf("%i philo %i %s\n", tstmp, philo, s);
	pthread_mutex_unlock(&all->message);
}

void	grab_fork(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo->all->fork[i]);
	display_message(philo->all, philo->id, "has taken fork", tmpsp); // получить время 
}

void	*routine(void *arg)
{
	t_philo	*philo;
	int		i;

	philo = arg;
	philo->r_fork = philo->id;
	philo->l_fork = philo->id + 1;
	if (philo->l_fork == philo->all->phl_num)
		philo->l_fork = 0;
	i = 0;
	while (i < philo->all->eat_num)
	{
		grab_
	}
}

int	init_philos(t_all *all)
{
	int	i;

	i = -1;
	pthread_mutex_init(&all->message, NULL);
	while (++i < all->phl_num)
		pthread_mutex_init(&all->fork[i], NULL);
	i = -1;
	while (++i < all->phl_num)
	{	
		all->philo[i].id = i;
		pthread_create(&all->philo[i].m_philo, NULL, &routine, (void *)all->philo[i]);
	}
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
	init_args(&all, argc, argv);
}